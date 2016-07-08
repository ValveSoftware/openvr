//===================== Copyright (c) Valve Corporation. All Rights Reserved. ======================
//==================================================================================================

#ifndef TRACKED_CAMERA_OPENVR_SAMPLE_H
#define TRACKED_CAMERA_OPENVR_SAMPLE_H

#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>
#include <openvr.h>

enum ELogLevel
{
    LogError,
    LogWarning,
    LogInfo,
};

#define MESSAGE_COLOR( r, g, b )	( ( (r) << 16 ) | ( (g) << 8 ) | (b) )
#define MESSAGE_COLOR_NORMAL		MESSAGE_COLOR( 0, 0, 0 )
#define MESSAGE_COLOR_WARNING		MESSAGE_COLOR( 255, 255, 0 )
#define MESSAGE_COLOR_ERROR			MESSAGE_COLOR( 255, 0, 0 )

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class CQCameraPreviewImage : public QWidget
{
    Q_OBJECT

public:
    CQCameraPreviewImage( QWidget *pParent = NULL );
    ~CQCameraPreviewImage();

    void SetFrameImage( const uint8_t *pFrameImage, uint32_t nFrameWidth, uint32_t nFrameHeight, const vr::CameraVideoStreamFrameHeader_t *pFrameHeader );

protected:
    virtual void paintEvent( QPaintEvent *pEvent );

private:
    QImage		*m_pSourceImage;
    vr::CameraVideoStreamFrameHeader_t m_CurrentFrameHeader;
};

class CQTrackedCameraOpenVRTest : public QMainWindow
{
    Q_OBJECT

public:
    CQTrackedCameraOpenVRTest( QWidget *pParent = NULL );
    ~CQTrackedCameraOpenVRTest();

protected:
    virtual void	showEvent( QShowEvent *pShowEvent );
    virtual void	closeEvent( QCloseEvent *pCloseEvent );

private slots:
    void	OnDisplayRefreshTimeout();
    void	OnToggleStreamingAction();
    void	OnExitAction();

private:
    bool	InitOpenVR();
    void	SetSplitterPosition( float flSplitFactor );
    void	CreatePrimaryWindows();
    bool	StartVideoPreview();
    void	StopVideoPreview();
    void	LogMessage( ELogLevel nLogLevel, const char *pMessage, ... );

    vr::IVRSystem					*m_pVRSystem;
    vr::IVRTrackedCamera			*m_pVRTrackedCamera;

    vr::TrackedCameraHandle_t	m_hTrackedCamera;

    QTimer					*m_pDisplayRefreshTimer;
    QVBoxLayout				*m_pRootLayout;
    QTextEdit				*m_pMessageText;
    QSplitter				*m_pSplitter;
    CQCameraPreviewImage	*m_pCameraPreviewImage;
    QMenu					*m_pMainMenu;
    QAction					*m_pExitAction;
    QAction					*m_pToggleStreamingAction;

    QString					m_HMDSerialNumberString;

    QTime					m_VideoSignalTime;

    uint32_t				m_nCameraFrameWidth;
    uint32_t				m_nCameraFrameHeight;
    uint32_t				m_nCameraFrameBufferSize;
    uint8_t					*m_pCameraFrameBuffer;

    uint32_t				m_nLastFrameSequence;
};

#endif // TRACKED_CAMERA_OPENVR_SAMPLE_H
