//====== Copyright Valve Corporation, All rights reserved. =======

#ifndef OPENVROVERLAYCONTROLLER_H
#define OPENVROVERLAYCONTROLLER_H

#ifdef _WIN32
#pragma once
#endif

#include "openvr.h"

#include <QtCore/QtCore>
// because of incompatibilities with QtOpenGL and GLEW we need to cherry pick includes
#include <QVector2D>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QtGui/QOpenGLContext>
#include <QtWidgets/QGraphicsScene>
#include <QOffscreenSurface>

class COpenVROverlayController : public QObject
{
	Q_OBJECT
	typedef QObject BaseClass;

public:
    static COpenVROverlayController *SharedInstance();

public:
    COpenVROverlayController();
    virtual ~COpenVROverlayController();

	bool Init();
	void Shutdown();
	void EnableRestart();

	bool BHMDAvailable();
    vr::IVRSystem *GetVRSystem();
	vr::HmdError GetLastHmdError();

	QRect GetHMDGeometry();
	QSize GetHMDRenderTargetSize();
	QString GetVRDriverString();
	QString GetVRDisplayString();
	QString GetName() { return m_strName; }

	bool BCompositorAvailable();
	vr::IVRCompositor *GetCompositor();
	vr::HmdError GetCompositorError();
	QString GetCompositorErrorString();

	void SetWidget( QWidget *pWidget );

public:
	QMatrix4x4 GetHMDMatrixProjectionEye( vr::Hmd_Eye nEye, float flNear, float flFar  );
	QMatrix4x4 GetHMDMatrixPoseEye( vr::Hmd_Eye nEye );

public slots:
	void OnSceneChanged( const QList<QRectF>& );
	void OnTimeoutPumpEvents();

protected:

private:
	bool ConnectToVRRuntime();
	void DisconnectFromVRRuntime();

	bool ConnectToCompositor();
	void DisconnectFromCompositor();

	vr::TrackedDevicePose_t m_rTrackedDevicePose[ vr::k_unMaxTrackedDeviceCount ];
	QString m_strVRDriver;
	QString m_strVRDisplay;
	QString m_strName;

	vr::IVRSystem *m_pVRSystem;
	vr::HmdError m_eLastHmdError;

	QRect m_HMDRect;
	QSize m_HMDRenderTargetSize;

private:
	vr::IVRCompositor *m_pCompositor;
	vr::IVROverlay *m_pOverlay;
	vr::HmdError m_eCompositorError;
	vr::HmdError m_eOverlayError;
	vr::Compositor_OverlaySettings m_overlaySettings;
	vr::VROverlayHandle_t m_ulOverlayHandle;

	QOpenGLContext *m_pOpenGLContext;
	QGraphicsScene *m_pScene;
	QOpenGLFramebufferObject *m_pFbo;
	QOffscreenSurface *m_pOffscreenSurface;

	QTimer *m_pPumpEventsTimer;

	// the widget we're drawing into the texture
	QWidget *m_pWidget;

	QPointF m_ptLastMouse;
	Qt::MouseButtons m_lastMouseButtons;
	bool m_bManualMouseHandling;
};


#endif // OPENVROVERLAYCONTROLLER_H
