//====== Copyright Valve Corporation, All rights reserved. =======


#include "openvroverlaycontroller.h"


#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QCursor>

using namespace vr;

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
COpenVROverlayController *s_pSharedVRController = NULL;

COpenVROverlayController *COpenVROverlayController::SharedInstance()
{
	if ( !s_pSharedVRController )
	{
        s_pSharedVRController = new COpenVROverlayController();
	}
	return s_pSharedVRController;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
COpenVROverlayController::COpenVROverlayController()
	: BaseClass()
	, m_pVRSystem( NULL )
	, m_eLastHmdError( vr::HmdError_None )
	, m_pCompositor( NULL )
    , m_pOverlay( NULL )
	, m_eCompositorError( vr::HmdError_None )
	, m_eOverlayError( vr::HmdError_None )
	, m_strVRDriver( "No Driver" )
	, m_strVRDisplay( "No Display" )
	, m_pOpenGLContext( NULL )
	, m_pScene( NULL )
	, m_pOffscreenSurface ( NULL )
	, m_pFbo( NULL )
	, m_pWidget( NULL )
	, m_pPumpEventsTimer( NULL )
	, m_lastMouseButtons( 0 )
	, m_ulOverlayHandle( vr::k_ulOverlayHandleInvalid )
	, m_bManualMouseHandling( false )
{
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
COpenVROverlayController::~COpenVROverlayController()
{
}


//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a QString
//-----------------------------------------------------------------------------
QString GetTrackedDeviceString( vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop )
{
	char buf[128];
	vr::TrackedPropertyError err;
	pHmd->GetStringTrackedDeviceProperty( unDevice, prop, buf, sizeof( buf ), &err );
	if( err != vr::TrackedProp_Success )
	{
		return QString( "Error Getting String: " ) + pHmd->GetPropErrorNameFromEnum( err );
	}
	else
	{
		return buf;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool COpenVROverlayController::Init()
{
	bool bSuccess = true;

    m_strName = "systemoverlay";

	QStringList arguments = qApp->arguments();

	int nNameArg = arguments.indexOf( "-name" );
	if( nNameArg != -1 && nNameArg + 2 <= arguments.size() )
	{
		m_strName = arguments.at( nNameArg + 1 );
	}

	QSurfaceFormat format;
	format.setMajorVersion( 4 );
	format.setMinorVersion( 1 );
	format.setProfile( QSurfaceFormat::CompatibilityProfile );

	m_pOpenGLContext = new QOpenGLContext();
	m_pOpenGLContext->setFormat( format );
	bSuccess = m_pOpenGLContext->create();
	if( !bSuccess )
		return false;

	// create an offscreen surface to attach the context and FBO to
	m_pOffscreenSurface = new QOffscreenSurface();
	m_pOffscreenSurface->create();
	m_pOpenGLContext->makeCurrent( m_pOffscreenSurface );

	m_pScene = new QGraphicsScene();
	connect( m_pScene, SIGNAL(changed(const QList<QRectF>&)), this, SLOT( OnSceneChanged(const QList<QRectF>&)) );

	// Loading the OpenVR Runtime
	bSuccess = ConnectToVRRuntime();

	bSuccess = bSuccess && ConnectToCompositor(); 

	if( m_pOverlay )
	{
        std::string sKey = std::string( "sample." ) + m_strName.toStdString();
		vr::VROverlayError overlayError = m_pOverlay->CreateOverlay( sKey.c_str(), m_strName.toStdString().c_str(), &m_ulOverlayHandle );
		bSuccess = bSuccess && overlayError == vr::VROverlayError_None;
	}

	if( bSuccess )
	{
		m_pOverlay->SetOverlayWidthInMeters( m_ulOverlayHandle, 1.5f );
        m_pOverlay->SetOverlayInputMethod( m_ulOverlayHandle, vr::VROverlayInputMethod_Mouse );
        m_pOverlay->SetOverlayVisibility( m_ulOverlayHandle, vr::VROverlayVisibility_SystemOverlay );
	
		// flip V to account for GL vs. OpenVR texture origin
        vr::VRTextureBounds_t overlayTextureBounds = { 0, 1.f, 1.f, 0.f };
		m_pOverlay->SetOverlayTextureBounds( m_ulOverlayHandle, &overlayTextureBounds );

		m_pPumpEventsTimer = new QTimer( this );
		connect(m_pPumpEventsTimer, SIGNAL( timeout() ), this, SLOT( OnTimeoutPumpEvents() ) );
		m_pPumpEventsTimer->setInterval( 20 );
		m_pPumpEventsTimer->start();

	}
	return true;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void COpenVROverlayController::Shutdown()
{
	DisconnectFromCompositor();
	DisconnectFromVRRuntime();

	delete m_pScene;
	delete m_pFbo;
	delete m_pOffscreenSurface;

	if( m_pOpenGLContext )
	{
//		m_pOpenGLContext->destroy();
		delete m_pOpenGLContext;
		m_pOpenGLContext = NULL;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void COpenVROverlayController::OnSceneChanged( const QList<QRectF>& )
{
	// skip rendering if the overlay isn't visible
	if( !m_pOverlay->IsOverlayVisible( m_ulOverlayHandle ) )
		return;

	m_pOpenGLContext->makeCurrent( m_pOffscreenSurface );
	m_pFbo->bind();
	
	QOpenGLPaintDevice device( m_pFbo->size() );
	QPainter painter( &device );

	m_pScene->render( &painter );

	m_pFbo->release();

	GLuint unTexture = m_pFbo->texture();
	if( unTexture != 0 )
	{
		m_pOverlay->SetOverlayTexture( m_ulOverlayHandle, (void*)unTexture );
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void COpenVROverlayController::OnTimeoutPumpEvents()
{
	if( !m_pVRSystem )
		return;


	if( m_bManualMouseHandling )
	{
		// tell OpenVR to make some events for us
		for( vr::TrackedDeviceIndex_t unDeviceId = 1; unDeviceId < vr::k_unControllerStateAxisCount; unDeviceId++ )
		{
			if( m_pOverlay->HandleControllerOverlayInteractionAsMouse( m_ulOverlayHandle, unDeviceId ) )
			{
				break;
			}
		}

	}

	vr::VREvent_t vrEvent;
	while( m_pOverlay->PollNextOverlayEvent( m_ulOverlayHandle, &vrEvent ) )
	{
		switch( vrEvent.eventType )
		{
		case vr::VREvent_MouseMove:
			{
				QPointF ptNewMouse( vrEvent.data.mouse.x, vrEvent.data.mouse.y );
				QPoint ptGlobal = ptNewMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent( QEvent::GraphicsSceneMouseMove );
				mouseEvent.setWidget( NULL );
				mouseEvent.setPos( ptNewMouse );
				mouseEvent.setScenePos( ptGlobal );
				mouseEvent.setScreenPos( ptGlobal );
				mouseEvent.setLastPos( m_ptLastMouse );
				mouseEvent.setLastScenePos( m_pWidget->mapToGlobal( m_ptLastMouse.toPoint() ) );
				mouseEvent.setLastScreenPos( m_pWidget->mapToGlobal( m_ptLastMouse.toPoint() ) );
				mouseEvent.setButtons( m_lastMouseButtons );
				mouseEvent.setButton( Qt::NoButton );
				mouseEvent.setModifiers( 0 );
				mouseEvent.setAccepted( false );

				m_ptLastMouse = ptNewMouse;
				QApplication::sendEvent( m_pScene, &mouseEvent );

				OnSceneChanged( QList<QRectF>() );
			}
			break;

		case vr::VREvent_MouseButtonDown:
			{
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;

				m_lastMouseButtons |= button;

				QPoint ptGlobal = m_ptLastMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent( QEvent::GraphicsSceneMousePress );
				mouseEvent.setWidget( NULL );
				mouseEvent.setPos( m_ptLastMouse );
				mouseEvent.setButtonDownPos( button, m_ptLastMouse );
				mouseEvent.setButtonDownScenePos( button, ptGlobal);
				mouseEvent.setButtonDownScreenPos( button, ptGlobal );
				mouseEvent.setScenePos( ptGlobal );
				mouseEvent.setScreenPos( ptGlobal );
				mouseEvent.setLastPos( m_ptLastMouse );
				mouseEvent.setLastScenePos( ptGlobal );
				mouseEvent.setLastScreenPos( ptGlobal );
				mouseEvent.setButtons( m_lastMouseButtons );
				mouseEvent.setButton( button );
				mouseEvent.setModifiers( 0 );
				mouseEvent.setAccepted( false );

				QApplication::sendEvent( m_pScene, &mouseEvent );
			}
			break;

		case vr::VREvent_MouseButtonUp:
			{
				Qt::MouseButton button = vrEvent.data.mouse.button == vr::VRMouseButton_Right ? Qt::RightButton : Qt::LeftButton;
				m_lastMouseButtons &= ~button;

				QPoint ptGlobal = m_ptLastMouse.toPoint();
				QGraphicsSceneMouseEvent mouseEvent( QEvent::GraphicsSceneMouseRelease );
				mouseEvent.setWidget( NULL );
				mouseEvent.setPos( m_ptLastMouse );
				mouseEvent.setScenePos( ptGlobal );
				mouseEvent.setScreenPos( ptGlobal );
				mouseEvent.setLastPos( m_ptLastMouse );
				mouseEvent.setLastScenePos( ptGlobal );
				mouseEvent.setLastScreenPos( ptGlobal );
				mouseEvent.setButtons( m_lastMouseButtons );
				mouseEvent.setButton( button );
				mouseEvent.setModifiers( 0 );
				mouseEvent.setAccepted( false );

				QApplication::sendEvent(  m_pScene, &mouseEvent );
			}
			break;

		case vr::VREvent_OverlayShown:
			{
				m_pWidget->repaint();
			}
			break;
		}
	}

}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void COpenVROverlayController::SetWidget( QWidget *pWidget )
{
	if( m_pScene )
	{
		// all of the mouse handling stuff requires that the widget be at 0,0
		pWidget->move( 0, 0 );
		m_pScene->addWidget( pWidget );
	}
	m_pWidget = pWidget;

	m_pFbo = new QOpenGLFramebufferObject( pWidget->width(), pWidget->height(), GL_TEXTURE_2D );

    if( m_pOverlay )
    {
        vr::HmdVector2_t vecWindowSize =
        {
            (float)pWidget->width(),
            (float)pWidget->height()
        };
        m_pOverlay->SetOverlayMouseScale( m_ulOverlayHandle, &vecWindowSize );
    }

}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool COpenVROverlayController::ConnectToVRRuntime()
{
	m_eLastHmdError = vr::HmdError_None;
	m_pVRSystem = vr::VR_Init( &m_eLastHmdError );

	if ( m_eLastHmdError != vr::HmdError_None )
	{
		m_pVRSystem = NULL;
		m_strVRDriver = "No Driver";
		m_strVRDisplay = "No Display";
		m_HMDRect.setCoords( 0, 0, 0, 0 );
		m_HMDRenderTargetSize.setWidth( 0 );
		m_HMDRenderTargetSize.setHeight( 0 );
		return false;
	}

	m_strVRDriver = GetTrackedDeviceString(m_pVRSystem, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	m_strVRDisplay = GetTrackedDeviceString(m_pVRSystem, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

	int32_t nWindowX, nWindowY;
	uint32_t nWindowWidth, nWindowHeight;

	m_pVRSystem->GetWindowBounds(&nWindowX, &nWindowY, &nWindowWidth, &nWindowHeight);
	m_HMDRect.setLeft(nWindowX);
	m_HMDRect.setTop(nWindowY);
	m_HMDRect.setWidth(nWindowWidth);
	m_HMDRect.setHeight(nWindowHeight);

	uint32_t nRenderTargetWidth, nRenderTargetHeight;
	m_pVRSystem->GetRecommendedRenderTargetSize(&nRenderTargetWidth, &nRenderTargetHeight);
	m_HMDRenderTargetSize.setWidth(nRenderTargetWidth);
	m_HMDRenderTargetSize.setHeight(nRenderTargetHeight);

	return true;
}


void COpenVROverlayController::DisconnectFromVRRuntime()
{
	vr::VR_Shutdown();
	m_pVRSystem = NULL;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool COpenVROverlayController::ConnectToCompositor()
{
	if ( m_pCompositor )
		return true;

	m_pCompositor = (vr::IVRCompositor *)vr::VR_GetGenericInterface( vr::IVRCompositor_Version, &m_eCompositorError );

	if ( m_eCompositorError != vr::HmdError_None )
	{
		m_pCompositor = NULL;
		return false;
	}

	m_pOverlay = (vr::IVROverlay *)vr::VR_GetGenericInterface( vr::IVROverlay_Version, &m_eOverlayError );

	if ( m_eOverlayError != vr::HmdError_None )
	{
		m_pOverlay = NULL;
		return false;
	}

	// make the GL context the active device
	m_pCompositor->SetGraphicsDevice( vr::Compositor_DeviceType_OpenGL, NULL );

	return true;
}


void COpenVROverlayController::DisconnectFromCompositor()
{
	m_pCompositor = NULL;
    m_pOverlay = NULL;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
QString COpenVROverlayController::GetVRDriverString()
{
	return m_strVRDriver;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
QString COpenVROverlayController::GetVRDisplayString()
{
	return m_strVRDisplay;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool COpenVROverlayController::BHMDAvailable()
{
	return m_pVRSystem != NULL;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
vr::IVRSystem *COpenVROverlayController::GetVRSystem()
{
	return m_pVRSystem;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------

vr::HmdError COpenVROverlayController::GetLastHmdError()
{
	return m_eLastHmdError;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
QRect COpenVROverlayController::GetHMDGeometry()
{
	return m_HMDRect;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
QSize COpenVROverlayController::GetHMDRenderTargetSize()
{
	return m_HMDRenderTargetSize;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
QMatrix4x4 COpenVROverlayController::GetHMDMatrixProjectionEye( vr::Hmd_Eye nEye, float flNear, float flFar )
{
	if ( !m_pVRSystem )
		return QMatrix4x4();

	vr::HmdMatrix44_t mat = m_pVRSystem->GetProjectionMatrix( nEye, flNear, flFar, vr::API_OpenGL);

	return QMatrix4x4(
		mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3],
		mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3],
		mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3],
		mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]
	);
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
QMatrix4x4 COpenVROverlayController::GetHMDMatrixPoseEye( vr::Hmd_Eye nEye )
{
	if ( !m_pVRSystem )
		return QMatrix4x4();

	vr::HmdMatrix34_t matEyeRight = m_pVRSystem->GetEyeToHeadTransform( nEye );
	QMatrix4x4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[0][1], matEyeRight.m[0][2], matEyeRight.m[0][3],
		matEyeRight.m[1][0], matEyeRight.m[1][1], matEyeRight.m[1][2], matEyeRight.m[1][3],
		matEyeRight.m[2][0], matEyeRight.m[2][1], matEyeRight.m[2][2], matEyeRight.m[2][3],
		0.0, 0.0, 0.0, 1.0
		);

	return matrixObj.inverted();
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
vr::IVRCompositor *COpenVROverlayController::GetCompositor()
{ 
	return m_pCompositor; 
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool COpenVROverlayController::BCompositorAvailable()
{
	return m_pCompositor != NULL;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
vr::HmdError COpenVROverlayController::GetCompositorError()
{
	return m_eCompositorError;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
QString COpenVROverlayController::GetCompositorErrorString()
{
	if ( m_eCompositorError != vr::HmdError_None )
	{
        return QString( VR_GetStringForHmdError( m_eCompositorError ) );
	}
	
	if ( m_pCompositor )
		return QString( "Running" );
	
	return QString( "Compositor failed to start, but no error" );
}


