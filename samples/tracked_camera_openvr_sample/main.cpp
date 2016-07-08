//===================== Copyright (c) Valve Corporation. All Rights Reserved. ======================
//==================================================================================================

#include "tracked_camera_openvr_sample.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CQTrackedCameraOpenVRTest w;
    w.show();

    return a.exec();
}
