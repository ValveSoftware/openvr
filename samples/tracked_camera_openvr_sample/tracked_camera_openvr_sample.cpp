//===================== Copyright (c) Valve Corporation. All Rights Reserved. ======================
//==================================================================================================

#include "tracked_camera_openvr_sample.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CQCameraPreviewImage::CQCameraPreviewImage( QWidget *pParent ) : QWidget( pParent )
{
    m_pSourceImage = nullptr;

    memset( &m_CurrentFrameHeader, 0, sizeof( m_CurrentFrameHeader ) );

    setContentsMargins( 0, 0, 0, 0 );

    // the image fully paints all of its pixels, qt does not need to do it
    setAttribute( Qt::WA_OpaquePaintEvent, true );
    setAttribute( Qt::WA_NoSystemBackground, true );
    setAutoFillBackground( false );

    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CQCameraPreviewImage::~CQCameraPreviewImage()
{
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQCameraPreviewImage::paintEvent( QPaintEvent *pEvent )
{
    QPainter painter( this );

    // determine the allowable painting area properly inscribed by any border widgets
    QRect paintRect = contentsRect().intersected( pEvent->rect() );
    if ( paintRect.isEmpty() )
    {
        // nothing to do
        return;
    }

    painter.fillRect( contentsRect(), QColor( 180, 180, 180 ) );

    if ( m_pSourceImage && !m_pSourceImage->isNull() )
    {
        painter.drawImage( QPoint( 0, 0 ), *m_pSourceImage );
    }

    QFont drawFont = painter.font();
    drawFont.setBold( true );
    painter.setFont( drawFont );

    int nLabelY = 0;
    painter.setPen( QColor( 0, 255, 255 ) );
    painter.drawText( 0, nLabelY, contentsRect().width(), contentsRect().height(), Qt::AlignRight|Qt::AlignTop, QString( "Frame Size: %1x%2" ).arg( m_CurrentFrameHeader.nWidth ).arg( m_CurrentFrameHeader.nHeight ) );
    nLabelY += 20;

    painter.drawText( 0, nLabelY, contentsRect().width(), contentsRect().height(), Qt::AlignRight|Qt::AlignTop, QString( "Frame Sequence: %1" ).arg( m_CurrentFrameHeader.nFrameSequence ) );
    nLabelY += 30;

    if ( m_CurrentFrameHeader.standingTrackedDevicePose.bPoseIsValid )
    {
        painter.setPen( QColor( 0, 255, 0 ) );
    }
    else
    {
        painter.setPen( QColor( 255, 255, 0 ) );
    }

    painter.drawText( 0, nLabelY, contentsRect().width(), contentsRect().height(), Qt::AlignRight|Qt::AlignTop, QString( "Pose: %1" ).arg( m_CurrentFrameHeader.standingTrackedDevicePose.bPoseIsValid ? "Valid" : "Invalid" ) );
    nLabelY += 20;

    for ( int i = 0; i < 3; i++ )
    {
        // emit the matrix
        vr::HmdMatrix34_t *pMatrix = &m_CurrentFrameHeader.standingTrackedDevicePose.mDeviceToAbsoluteTracking;
        painter.drawText(
            0,
            nLabelY,
            contentsRect().width(),
            contentsRect().height(),
             Qt::AlignRight|Qt::AlignTop,
            QString( "%1 %2 %3 %4" ).arg( pMatrix->m[i][0], 2, 'f', 2 ).arg( pMatrix->m[i][1], 2, 'f', 2 ).arg( pMatrix->m[i][2], 2, 'f', 2 ).arg( pMatrix->m[i][3], 2, 'f', 2 ) );
        nLabelY += 20;
    }
    nLabelY += 10;

    painter.drawText( 0, nLabelY, contentsRect().width(), contentsRect().height(), Qt::AlignRight|Qt::AlignTop, QString( "Pose Velocity:" ) );
    nLabelY += 20;

    vr::HmdVector3_t *pVelocity = &m_CurrentFrameHeader.standingTrackedDevicePose.vVelocity;
    painter.drawText(
            0,
            nLabelY,
            contentsRect().width(),
            contentsRect().height(),
             Qt::AlignRight|Qt::AlignTop,
            QString( "%1 %2 %3" ).arg( pVelocity->v[0], 2, 'f', 2 ).arg( pVelocity->v[1], 2, 'f', 2 ).arg( pVelocity->v[2], 2, 'f', 2 ) );
    nLabelY += 30;

    painter.drawText( 0, nLabelY, contentsRect().width(), contentsRect().height(), Qt::AlignRight|Qt::AlignTop, QString( "Pose Angular Velocity:" ) );
    nLabelY += 20;

    vr::HmdVector3_t *pAngularVelocity = &m_CurrentFrameHeader.standingTrackedDevicePose.vVelocity;
    painter.drawText(
            0,
            nLabelY,
            contentsRect().width(),
            contentsRect().height(),
             Qt::AlignRight|Qt::AlignTop,
            QString( "%1 %2 %3" ).arg( pAngularVelocity->v[0], 2, 'f', 2 ).arg( pAngularVelocity->v[1], 2, 'f', 2 ).arg( pAngularVelocity->v[2], 2, 'f', 2 ) );
    nLabelY += 20;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQCameraPreviewImage::SetFrameImage( const uint8_t *pFrameImage, uint32_t nFrameWidth, uint32_t nFrameHeight, const vr::CameraVideoStreamFrameHeader_t *pFrameHeader )
{
    if ( pFrameHeader )
    {
        m_CurrentFrameHeader = *pFrameHeader;
    }

    if ( pFrameImage && nFrameWidth && nFrameHeight )
    {
        if ( m_pSourceImage && ( (uint32_t)m_pSourceImage->width() != nFrameWidth || (uint32_t)m_pSourceImage->height() != nFrameHeight ) )
        {
            // dimension changed
            delete m_pSourceImage;
            m_pSourceImage = nullptr;
        }

        if ( !m_pSourceImage )
        {
            // allocate to expected dimensions
            m_pSourceImage = new QImage( nFrameWidth, nFrameHeight, QImage::Format_RGB32 );
        }

        for ( uint32_t y = 0; y < nFrameHeight; y++ )
        {
            for ( uint32_t x = 0; x < nFrameWidth; x++ )
            {
                m_pSourceImage->setPixel( x, y, QColor( pFrameImage[0], pFrameImage[1], pFrameImage[2] ).rgba() );
                pFrameImage += 4;
            }
        }
    }

    // schedule a repaint
    update();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CQTrackedCameraOpenVRTest::CQTrackedCameraOpenVRTest( QWidget *pParent ) : QMainWindow( pParent )
{
    m_pVRSystem = nullptr;
    m_pVRTrackedCamera = nullptr;

    m_hTrackedCamera = INVALID_TRACKED_CAMERA_HANDLE;

    m_nCameraFrameWidth = 0;
    m_nCameraFrameHeight = 0;
    m_nCameraFrameBufferSize = 0;
    m_pCameraFrameBuffer = nullptr;

    setWindowTitle( "Tracked Camera OpenVR Test" );

    CreatePrimaryWindows();

    setFocusPolicy( Qt::StrongFocus );

    LogMessage( LogInfo, "Build:%s %s\n", __DATE__, __TIME__ );

    bool bValidOpenVR = InitOpenVR();

    m_pExitAction = new QAction( QString( "Exit" ), this );
    connect( m_pExitAction, SIGNAL( triggered() ), this, SLOT( OnExitAction() ) );

    m_pToggleStreamingAction = new QAction( "Toggle Streaming", this );
    m_pToggleStreamingAction->setCheckable( true );
    connect( m_pToggleStreamingAction, SIGNAL( triggered() ), this, SLOT( OnToggleStreamingAction() ) );

    m_pMainMenu = menuBar()->addMenu( "&Main" );
    m_pMainMenu->addAction( m_pToggleStreamingAction );
    m_pMainMenu->addSeparator();
    m_pMainMenu->addAction( m_pExitAction );

    if ( !bValidOpenVR )
    {
        // No valid HMD, inhibit any expected user options
        m_pMainMenu->setEnabled( false );
    }

    // create an update timer
    m_pDisplayRefreshTimer = new QTimer( this );
    m_pDisplayRefreshTimer->setInterval( 16 );
    connect( m_pDisplayRefreshTimer, SIGNAL( timeout() ), this, SLOT( OnDisplayRefreshTimeout() ) );
    m_pDisplayRefreshTimer->start();

    resize( 900, 700 );

    show();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CQTrackedCameraOpenVRTest::~CQTrackedCameraOpenVRTest()
{
    m_pVRSystem = nullptr;
    m_pVRTrackedCamera = nullptr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::showEvent( QShowEvent *pShowEvent )
{
    pShowEvent = pShowEvent;

    SetSplitterPosition( 0.75f );

    // auto start streaming, same as user triggering
    m_pToggleStreamingAction->trigger();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::closeEvent( QCloseEvent *pCloseEvent )
{
    if ( m_pVRTrackedCamera )
    {
        m_pVRTrackedCamera->ReleaseVideoStreamingService( m_hTrackedCamera );
    }

    pCloseEvent->accept();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::SetSplitterPosition( float flSplitFactor )
{
    QList< int > splitterSizes = m_pSplitter->sizes();

    int nTotalHeight = 0;
    for ( int i = 0; i < splitterSizes.count(); i++ )
    {
        nTotalHeight += splitterSizes.at( i );
    }

    if ( !nTotalHeight )
    {
        // This occurs when the window hasn't been shown yet at all, app is still starting up.
        // The splitter split factor will get forced down again during the initial show event of the parent.
        return;
    }

    int nTopHeight = flSplitFactor * (float)nTotalHeight;
    int nBottomHeight = nTotalHeight - nTopHeight;
    if ( nTopHeight != splitterSizes[0] || nBottomHeight != splitterSizes[1] )
    {
        splitterSizes.clear();
        splitterSizes.append( nTopHeight );
        splitterSizes.append( nBottomHeight );
        m_pSplitter->setSizes( splitterSizes );
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::LogMessage( ELogLevel nLogLevel, const char *pMessage, ... )
{
    va_list args;
    char formattedMessage[2048];

    va_start( args, pMessage );
    _vsnprintf_s( formattedMessage, sizeof( formattedMessage ), pMessage, args );
    formattedMessage[ sizeof( formattedMessage ) - 1 ] = 0;
    va_end( args );

    if ( m_pMessageText )
    {
        uint32_t rgbaColor = MESSAGE_COLOR_NORMAL;
        if ( nLogLevel == LogWarning )
            rgbaColor = MESSAGE_COLOR_WARNING;
        else if ( nLogLevel == LogError )
            rgbaColor = MESSAGE_COLOR_ERROR;

        QColor textColor( ( rgbaColor >> 16 ) & 0xFF, ( rgbaColor >> 8 ) & 0xFF, ( rgbaColor & 0xFF ), 0xFF );
        m_pMessageText->setTextColor( textColor );
        m_pMessageText->insertPlainText( formattedMessage );
        m_pMessageText->moveCursor( QTextCursor::End, QTextCursor::MoveAnchor );
    }

    fprintf(stderr, "%s", formattedMessage);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::CreatePrimaryWindows()
{
    m_pCameraPreviewImage = new CQCameraPreviewImage( this );

    m_pMessageText = new QTextEdit();
    m_pMessageText->setLineWrapMode( QTextEdit::NoWrap );
    m_pMessageText->setReadOnly( true );
    m_pMessageText->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    QPalette palette = m_pMessageText->palette();
    palette.setColor( QPalette::Base, QColor( 180, 180, 180 ) );
    m_pMessageText->setPalette( palette );
    m_pMessageText->setAutoFillBackground( true );

    m_pSplitter = new QSplitter( Qt::Vertical, this );
    m_pSplitter->setHandleWidth( 8 );

    m_pSplitter->setChildrenCollapsible( false );
    m_pCameraPreviewImage->setMinimumHeight( 100 );
    m_pMessageText->setMinimumHeight( 100 );

    m_pSplitter->addWidget( m_pCameraPreviewImage );
    m_pSplitter->addWidget( m_pMessageText );

    setCentralWidget( m_pSplitter );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::OnExitAction()
{
    close();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::OnDisplayRefreshTimeout()
{
    if ( !m_pVRTrackedCamera || !m_hTrackedCamera )
        return;

    if ( m_VideoSignalTime.elapsed() >= 2000 )
    {
        // No frames after 2 seconds...
        LogMessage( LogError, "No Video Frames Arriving!\n" );
        m_VideoSignalTime.restart();
    }

    // get the frame header only
    vr::CameraVideoStreamFrameHeader_t frameHeader;
    vr::EVRTrackedCameraError nCameraError = m_pVRTrackedCamera->GetVideoStreamFrameBuffer( m_hTrackedCamera, vr::VRTrackedCameraFrameType_Undistorted, nullptr, 0, &frameHeader, sizeof( frameHeader ) );
    if ( nCameraError != vr::VRTrackedCameraError_None )
        return;

    if ( frameHeader.nFrameSequence == m_nLastFrameSequence )
    {
        // frame hasn't changed yet, nothing to do
        return;
    }

    m_VideoSignalTime.restart();

    // Frame has changed, do the more expensive frame buffer copy
    nCameraError = m_pVRTrackedCamera->GetVideoStreamFrameBuffer( m_hTrackedCamera, vr::VRTrackedCameraFrameType_Undistorted, m_pCameraFrameBuffer, m_nCameraFrameBufferSize, &frameHeader, sizeof( frameHeader ) );
    if ( nCameraError != vr::VRTrackedCameraError_None )
        return;

    m_nLastFrameSequence = frameHeader.nFrameSequence;

    m_pCameraPreviewImage->SetFrameImage( m_pCameraFrameBuffer, m_nCameraFrameWidth, m_nCameraFrameHeight, &frameHeader );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::OnToggleStreamingAction()
{
    if ( m_pToggleStreamingAction->isChecked() )
    {
        StartVideoPreview();
    }
    else
    {
        StopVideoPreview();
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CQTrackedCameraOpenVRTest::StartVideoPreview()
{
    LogMessage( LogInfo, "StartVideoPreview()\n" );

    // Allocate for camera frame buffer requirements
    uint32_t nCameraFrameBufferSize = 0;
    if ( m_pVRTrackedCamera->GetCameraFrameSize( vr::k_unTrackedDeviceIndex_Hmd, vr::VRTrackedCameraFrameType_Undistorted, &m_nCameraFrameWidth, &m_nCameraFrameHeight, &nCameraFrameBufferSize ) != vr::VRTrackedCameraError_None )
    {
        LogMessage( LogError, "GetCameraFrameBounds() Failed!\n" );
        return false;
    }

    if ( nCameraFrameBufferSize && nCameraFrameBufferSize != m_nCameraFrameBufferSize )
    {
        delete [] m_pCameraFrameBuffer;
        m_nCameraFrameBufferSize = nCameraFrameBufferSize;
        m_pCameraFrameBuffer = new uint8_t[m_nCameraFrameBufferSize];
        memset( m_pCameraFrameBuffer, 0, m_nCameraFrameBufferSize );
    }

    m_nLastFrameSequence = 0;
    m_VideoSignalTime.start();

    m_pVRTrackedCamera->AcquireVideoStreamingService( vr::k_unTrackedDeviceIndex_Hmd, &m_hTrackedCamera );
    if ( m_hTrackedCamera == INVALID_TRACKED_CAMERA_HANDLE )
    {
        LogMessage( LogError, "AcquireVideoStreamingService() Failed!\n" );
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CQTrackedCameraOpenVRTest::StopVideoPreview()
{
    LogMessage( LogInfo, "StopVideoPreview()\n" );

    m_pVRTrackedCamera->ReleaseVideoStreamingService( m_hTrackedCamera );
    m_hTrackedCamera = INVALID_TRACKED_CAMERA_HANDLE;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CQTrackedCameraOpenVRTest::InitOpenVR()
{
    // Loading the SteamVR Runtime
    LogMessage( LogInfo, "\nStarting OpenVR...\n" );
    vr::EVRInitError eError = vr::VRInitError_None;
    m_pVRSystem = vr::VR_Init( &eError, vr::VRApplication_Scene );
    if ( eError != vr::VRInitError_None )
    {
        m_pVRSystem = nullptr;
        char buf[1024];
        sprintf_s( buf, sizeof( buf ), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsSymbol( eError ) );
        LogMessage( LogError, "%s\n", buf );
        return false;
    }
    else
    {
        char systemName[1024];
        char serialNumber[1024];
        m_pVRSystem->GetStringTrackedDeviceProperty( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, systemName, sizeof( systemName ) );
        m_pVRSystem->GetStringTrackedDeviceProperty( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String, serialNumber, sizeof( serialNumber ) );

        m_HMDSerialNumberString = serialNumber;

        LogMessage( LogInfo, "VR HMD: %s %s\n", systemName, serialNumber );
    }

    m_pVRTrackedCamera = vr::VRTrackedCamera();
    if ( !m_pVRTrackedCamera )
    {
        LogMessage( LogError, "Unable to get Tracked Camera interface.\n" );
        return false;
    }

    bool bHasCamera = false;
    vr::EVRTrackedCameraError nCameraError = m_pVRTrackedCamera->HasCamera( vr::k_unTrackedDeviceIndex_Hmd, &bHasCamera );
    if ( nCameraError != vr::VRTrackedCameraError_None || !bHasCamera )
    {
        LogMessage( LogError, "No Tracked Camera Available! (%s)\n", m_pVRTrackedCamera->GetCameraErrorNameFromEnum( nCameraError ) );
        return false;
    }

    // Accessing the FW description is just a further check to ensure camera communication is valid as expected.
    vr::ETrackedPropertyError propertyError;
    char buffer[128];
    m_pVRSystem->GetStringTrackedDeviceProperty( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_CameraFirmwareDescription_String, buffer, sizeof( buffer ), &propertyError );
    if ( propertyError != vr::TrackedProp_Success )
    {
        LogMessage( LogError, "Failed to get tracked camera firmware description!\n" );
        return false;
    }

    LogMessage( LogInfo, "Camera Firmware: %s\n\n", buffer );

    return true;
}
