#include "device_provider.h"

#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C" 
#else
#error "Unsupported Platform."
#endif

//Global, single interface of our device provider.
DeviceProvider device_provider;

HMD_DLL_EXPORT void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
	if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName)) {
		return &device_provider;
	}

	if (pReturnCode) {
		*pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
	}

	return nullptr;
}