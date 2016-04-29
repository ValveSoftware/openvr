// A test program to use 'overlay' in OpenVR

// STD Headers
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

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
	vr::VROverlayHandle_t handleOverlay = 0;
	if (pVROverlay != nullptr)
	{
		vr::HmdMatrix34_t matTransform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,-3 };

		// create overlay
		if (pVROverlay->CreateOverlay("The-Key", "The-Name", &handleOverlay) == vr::VROverlayError_None)
		{
			// Size and position
			pVROverlay->SetOverlayWidthInMeters(handleOverlay, 3.0f);
			//pVROverlay->SetOverlayTransformAbsolute(handleOverlay, vr::TrackingUniverseSeated, &matTransform);
			pVROverlay->SetOverlayTransformTrackedDeviceRelative(handleOverlay, 1, &matTransform);

			// non-used
			pVROverlay->SetOverlayColor(handleOverlay, 1.0f, 1.0f, 1.0f);
			pVROverlay->SetOverlayAlpha(handleOverlay, 1.0f);
			pVROverlay->SetOverlayInputMethod(handleOverlay, vr::VROverlayInputMethod_Mouse);

			// show it!
			pVROverlay->ShowOverlay(handleOverlay);
		}
	}

	if(false)
	{
		cv::Mat imgFile = cv::imread("E:\\testb.jpg");
		cv::Mat imgImage;
		cv::cvtColor(imgFile, imgImage, cv::COLOR_BGR2RGBA);

		// This code faile if image too large
		// Should be the bug of OpenVR: https://github.com/ValveSoftware/openvr/issues/46
		pVROverlay->SetOverlayRaw(handleOverlay, imgImage.data, imgImage.cols, imgImage.rows, imgImage.channels());
	}
	else
	{
		pVROverlay->SetOverlayFromFile(handleOverlay, "E:\\testb.jpg");
	}

	while (true)
	{
		// tell OpenVR to make some events for us
		for (vr::TrackedDeviceIndex_t uDeviceIdx = 1; uDeviceIdx < vr::k_unControllerStateAxisCount; ++uDeviceIdx)
		{
			if (pVROverlay->HandleControllerOverlayInteractionAsMouse(handleOverlay, uDeviceIdx))
				break;
		}

		vr::VREvent_t eVREvent;
		while (pVROverlay->PollNextOverlayEvent(handleOverlay, &eVREvent, sizeof(eVREvent)))
		{
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	
	//vr::IVRCompositor* pCompositor = vr::VRCompositor();
	vr::VR_Shutdown();
}
