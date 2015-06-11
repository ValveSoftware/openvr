#include "overlaywidget.h"
#include "openvroverlaycontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OverlayWidget *pOverlayWidget = new OverlayWidget;

    COpenVROverlayController::SharedInstance()->Init();

    COpenVROverlayController::SharedInstance()->SetWidget( pOverlayWidget );

    // don't show widgets that you're going display in an overlay
    //w.show();

    return a.exec();
}
