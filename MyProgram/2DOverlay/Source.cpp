// A test program to use 'overlay' in OpenVR

// STD Headers
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

// OpenVR
#include <openvr.h>
#pragma comment(lib,"openvr_api.lib") 

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#ifdef _DEBUG
#pragma comment(lib,"opencv_world310d.lib") 
#else
#pragma comment(lib,"opencv_world310.lib") 
#endif

std::string GetInformation(vr::IVRSystem* pVRSystem, vr::ETrackedDeviceProperty eProp)
{
	char aText[128];
	vr::TrackedPropertyError eTPErr;
	pVRSystem->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, eProp, aText, sizeof(aText), &eTPErr);
	if (eTPErr == vr::TrackedProp_Success)
	{
		return std::string(aText);
	}
	return "";
}

int main(int argc, char** argv)
{
	vr::EVRInitError eError = vr::VRInitError_None;

	// Initial VR System
	vr::IVRSystem* pVRSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);
	if (eError != vr::VRInitError_None)
	{
		std::cerr << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
		return 1;
	}

	// Get some information
	{
		std::cout << "\nDevice Information\n";
		std::cout << " System Name:       " << GetInformation(pVRSystem, vr::Prop_TrackingSystemName_String) << "\n";
		std::cout << " Manufacturer Name: " << GetInformation(pVRSystem, vr::Prop_ManufacturerName_String) << "\n";
		std::cout << "  Model Number:     " << GetInformation(pVRSystem, vr::Prop_ModelNumber_String) << "\n";
		std::cout << "  Serial Number:    " << GetInformation(pVRSystem, vr::Prop_SerialNumber_String) << "\n";
		std::cout << " Render Model Name: " << GetInformation(pVRSystem, vr::Prop_RenderModelName_String) << "\n";
	}

	// Get VR Overlay
	vr::IVROverlay* pVROverlay = vr::VROverlay();
	vr::VROverlayHandle_t hdlBigImage = 0, hdlInfoImage = 0;
	cv::Mat imInfo(100, 300, CV_8UC4);
	if (pVROverlay != nullptr)
	{
		// create overlay 1
		if (pVROverlay->CreateOverlay("BigImage", "A Big Image wall", &hdlBigImage) == vr::VROverlayError_None)
		{
			// Size and position
			pVROverlay->SetOverlayWidthInMeters(hdlBigImage, 3.0f);
			vr::HmdMatrix34_t matTransform = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,-3 };
			pVROverlay->SetOverlayTransformAbsolute(hdlBigImage, vr::TrackingUniverseSeated, &matTransform);

			// non-used
			pVROverlay->SetOverlayColor(hdlBigImage, 1.0f, 1.0f, 1.0f);
			pVROverlay->SetOverlayAlpha(hdlBigImage, 1.0f);
			pVROverlay->SetOverlayInputMethod(hdlBigImage, vr::VROverlayInputMethod_Mouse);

			// show it!
			pVROverlay->ShowOverlay(hdlBigImage);

			// set image, load from file
			pVROverlay->SetOverlayFromFile(hdlBigImage, "E:\\testb.jpg");
		}


		// create overlay 2
		if (pVROverlay->CreateOverlay("InfoImage", "Information Image", &hdlInfoImage) == vr::VROverlayError_None)
		{
			// Size and position
			pVROverlay->SetOverlayWidthInMeters(hdlInfoImage, 0.1f);
			vr::HmdMatrix34_t matTransform = {
				1,0,0,0,
				0,1,0,0.1,
				0,0,1,0 };
			pVROverlay->SetOverlayTransformTrackedDeviceRelative(hdlInfoImage, 1, &matTransform);

			// non-used
			pVROverlay->SetOverlayColor(hdlInfoImage, 1.0f, 1.0f, 1.0f);
			pVROverlay->SetOverlayAlpha(hdlInfoImage, 1.0f);
			pVROverlay->SetOverlayInputMethod(hdlInfoImage, vr::VROverlayInputMethod_None);

			// show it!
			pVROverlay->ShowOverlay(hdlInfoImage);

			// set image image
			// This code faile if image too large
			// Should be the bug of OpenVR: https://github.com/ValveSoftware/openvr/issues/46
			pVROverlay->SetOverlayRaw(hdlInfoImage, imInfo.data, imInfo.cols, imInfo.rows, imInfo.channels());
		}
	}

	vr::VRControllerState_t mCState;
	vr::TrackedDevicePose_t mCPose;
	while (true)
	{
		imInfo.setTo(128);
		for (vr::TrackedDeviceIndex_t uDeviceIdx = 1; uDeviceIdx < vr::k_unControllerStateAxisCount; ++uDeviceIdx)
		{
			if (pVRSystem->IsTrackedDeviceConnected(uDeviceIdx))
			{
				// tell OpenVR to make some events for us
				pVROverlay->HandleControllerOverlayInteractionAsMouse(hdlBigImage, uDeviceIdx);

				// get information of controller
				pVRSystem->GetControllerStateWithPose(vr::TrackingUniverseStanding, uDeviceIdx, &mCState, &mCPose);

				std::ostringstream oss;
				oss << uDeviceIdx << " (" << mCPose.eTrackingResult << ") - " << mCState.ulButtonPressed;
				cv::putText(imInfo, oss.str(), cv::Point( 2, 50 * uDeviceIdx - 10), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar( 0,0,0,255), 5 );
			}
			else
			{
				break;
			}
		}

		// update hdlInfoImage, may flick
		if (pVROverlay->IsOverlayVisible(hdlInfoImage))
		{
			pVROverlay->SetOverlayRaw(hdlInfoImage, imInfo.data, imInfo.cols, imInfo.rows, imInfo.channels());
		}

		vr::VREvent_t eVREvent;
		while (pVROverlay->PollNextOverlayEvent(hdlBigImage, &eVREvent, sizeof(eVREvent)))
		{
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	//vr::IVRCompositor* pCompositor = vr::VRCompositor();
	vr::VR_Shutdown();
}
