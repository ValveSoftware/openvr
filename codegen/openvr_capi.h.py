import sys
import api_shared
import operator
from functools import reduce

print ("""//======= Copyright (c) Valve Corporation, All rights reserved. ===============
//
// Purpose: Header for flatted SteamAPI. Use this for binding to other languages.
// This file is auto-generated, do not edit it.
//
//=============================================================================

#ifndef __OPENVR_API_FLAT_H__
#define __OPENVR_API_FLAT_H__
#if defined( _WIN32 ) || defined( __clang__ )
#pragma once
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#if defined( _WIN32 )
#define OPENVR_FNTABLE_CALLTYPE __stdcall
#else
#define OPENVR_FNTABLE_CALLTYPE 
#endif

// OPENVR API export macro
#if defined( _WIN32 ) && !defined( _X360 )
	#if defined( OPENVR_API_EXPORTS )
	#define S_API EXTERN_C __declspec( dllexport )
	#elif defined( OPENVR_API_NODLL )
	#define S_API EXTERN_C
	#else
	#define S_API extern "C" __declspec( dllimport ) 
	#endif // OPENVR_API_EXPORTS
#elif defined( __GNUC__ )
	#if defined( OPENVR_API_EXPORTS )
	#define S_API EXTERN_C __attribute__ ((visibility("default")))
	#else
	#define S_API EXTERN_C
	#endif // OPENVR_API_EXPORTS
#else // !WIN32
	#if defined( OPENVR_API_EXPORTS )
	#define S_API EXTERN_C
	#else
	#define S_API EXTERN_C
	#endif // OPENVR_API_EXPORTS
#endif

#include <stdint.h>

#if defined( __WIN32 )
typedef char bool;
#else
#include <stdbool.h>
#endif

typedef uint64_t PropertyContainerHandle_t;
typedef uint32_t PropertyTypeTag_t;
typedef uint64_t VRActionHandle_t;
typedef uint64_t VRActionSetHandle_t;
typedef uint64_t VRInputValueHandle_t;
typedef uint64_t PathHandle_t;
""")



data = api_shared.loadfile('../headers/openvr_api.json', 'vr')
converttype = api_shared.converttype
striparraysuffix = api_shared.striparraysuffix
structlist = api_shared.structlist
structparents = api_shared.structparents
structindices = api_shared.structindices
typedeflist = api_shared.typedeflist
enumlist = api_shared.enumlist
getnamespace = api_shared.getnamespace
getclasswithoutnamespace = api_shared.getclasswithoutnamespace
unmanagedtype = api_shared.unmanagedtype
ctype = api_shared.ctype
namespace = 'vr'

def OutputStructFields(struct):

	# recursively add base class fields first
	basename = getclasswithoutnamespace(struct['struct'])
	if basename in structparents:
		parentname = structparents[basename]
		i = structindices[parentname];
		OutputStructFields(data['structs'][i])

	for enumvalue in struct['fields']:
		fieldtype = enumvalue['fieldtype']
		otype = striparraysuffix(fieldtype)
		lastchar = fieldtype[len(fieldtype) - 1]
		if(lastchar == ']'):
			#print('\t//fixed '+otype+' '+enumvalue['fieldname']+ fieldtype[fieldtype.find('['):]+';')
			dims = map(int, fieldtype[fieldtype.find('[')+1:-1].split(']['))
			size = reduce(operator.mul, dims, 1)
			utype = unmanagedtype(otype)
			print('\t'+('char ' if(otype=='char') else ctype(otype)+' ')+enumvalue['fieldname']+ fieldtype[fieldtype.find('['):]+'; //'+otype+fieldtype[fieldtype.find('['):])
		else:
			if(lastchar == '*'):
				print('\t'+ctype(fieldtype)+' '+enumvalue['fieldname']+';'+' // '+fieldtype)
			else:
				print('\t'+ctype(otype)+' '+enumvalue['fieldname']+';')

########
# Output constants into OpenVR class
print ("""
// OpenVR Constants
""")
for const in data['consts']:
	if(len(const) > 0):
		consttype = converttype(const['consttype'])
		constval = const['constval']
		if(consttype == 'string'):
			constval = '\"' + constval + '\"'
			consttype = 'char *'
		print('static const '+ctype(consttype)+' '+const['constname']+' = '+constval+';')

######
# Output enums
print ("""
// OpenVR Enums
""")
for enum in data['enums']:
	if(len(enum) > 0 and len(enum['enumname'])):
		ns = getnamespace(enum['enumname'])
		enumname = getclasswithoutnamespace(enum['enumname'])
		if(ns == namespace or (namespace == '' and ns[:1] == 'I')):
			print('typedef enum '+enumname+'\n{')
			for enumvalue in enum['values']:
				entry = enumvalue['name']
				if(entry.startswith(enumname)): entry = entry[len(enumname):] # strip off enum name
				if(entry.startswith('_')): entry = entry[1:] # strip off leading underscore
				print('\t'+enumname+'_'+entry+' = '+enumvalue['value']+',')
			print('} '+enumname+';\n')

########
# Output typedefs into OpenVR class

print ("""
// OpenVR typedefs

typedef uint32_t TrackedDeviceIndex_t;
typedef uint32_t VRNotificationId;
typedef uint64_t VROverlayHandle_t;
""")

for typedef in data['typedefs']:
	if(len(typedef) > 0):
		thetype = typedef['type']
		if(thetype[0:6] != 'union ' and thetype[0:7] != 'struct '):
			thetypedef = getclasswithoutnamespace(typedef['typedef']) # remove the vr:: bit from thetypedef
			thetype = getclasswithoutnamespace(thetype)
			print('typedef '+thetype+' '+thetypedef+';')

######
# Output structs
print ("""
// OpenVR Structs
""")
for struct in data['structs']:
	if(len(struct) > 0 and len(struct['struct'])):
		structname = struct['struct']
		#print('//'+structname)
		ns = getnamespace(structname)
		basename = getclasswithoutnamespace(structname)
		if(basename != 'VREvent_t' and basename != 'VROverlayIntersectionMaskPrimitive_t' and basename != '(anonymous)' and (ns == namespace or (namespace == '' and ns[:1] == 'I') or (namespace == '' and ns[:1] == 'C'))):
		#if basename != '(anonymous)':
			unsafe = 0
			for enumvalue in struct['fields']:
				fieldtype = enumvalue['fieldtype']
				if(fieldtype[len(fieldtype) - 1] == ']'):
					unsafe = 1

			pack = ((basename == 'RenderModel_t') or 
					(basename == 'VRControllerState_t') or 
					(basename=='RenderModel_TextureMap_t') or
					(basename=='VREvent_t'))

			# resolve typedefs used to rename structs
			for key, value in typedeflist.items():
				if(converttype(value) == basename):
					basename = getclasswithoutnamespace(key)
					break

			if (pack):
				print('#if defined(__linux__) || defined(__APPLE__)\n#pragma pack( push, 4 )\n#endif')
			if(unsafe == 1):
				print('typedef struct '+basename+'\n{')  # this was for potentially putting unsafe fields in
			else:
				print('typedef struct '+basename+'\n{')

			OutputStructFields(struct)

			print('} '+basename+';\n')
			if pack:
				print('#if defined(__linux__) || defined(__APPLE__)\n#pragma pack( pop )\n#endif')

print ("""
typedef union
{
	VREvent_Reserved_t reserved;
	VREvent_Controller_t controller;
	VREvent_Mouse_t mouse;
	VREvent_Scroll_t scroll;
	VREvent_Process_t process;
	VREvent_Notification_t notification;
	VREvent_Overlay_t overlay;
	VREvent_Status_t status;
	VREvent_Keyboard_t keyboard;
	VREvent_Ipd_t ipd;
	VREvent_Chaperone_t chaperone;
	VREvent_PerformanceTest_t performanceTest;
	VREvent_TouchPadMove_t touchPadMove;
	VREvent_SeatedZeroPoseReset_t seatedZeroPoseReset;
	VREvent_Screenshot_t screenshot;
	VREvent_ScreenshotProgress_t screenshotProgress;
	VREvent_ApplicationLaunch_t applicationLaunch;
	VREvent_EditingCameraSurface_t cameraSurface;
	VREvent_MessageOverlay_t messageOverlay;
	VREvent_Property_t property;
	VREvent_HapticVibration_t hapticVibration;
	VREvent_WebConsole_t webConsole;
	VREvent_InputBindingLoad_t inputBinding;
	VREvent_InputActionManifestLoad_t actionManifest;
	VREvent_SpatialAnchor_t spatialAnchor;
} VREvent_Data_t;

#if defined(__linux__) || defined(__APPLE__) 
// This structure was originally defined mis-packed on Linux, preserved for 
// compatibility. 
#pragma pack( push, 4 )
#endif

/** An event posted by the server to all running applications */
struct VREvent_t
{
	uint32_t eventType; // EVREventType enum
	TrackedDeviceIndex_t trackedDeviceIndex;
	float eventAgeSeconds;
	// event data must be the end of the struct as its size is variable
	VREvent_Data_t data;
};

#if defined(__linux__) || defined(__APPLE__) 
#pragma pack( pop )
#endif
""")

print ("""
typedef union
{
	IntersectionMaskRectangle_t m_Rectangle;
	IntersectionMaskCircle_t m_Circle;
} VROverlayIntersectionMaskPrimitive_Data_t;

struct VROverlayIntersectionMaskPrimitive_t
{
	EVROverlayIntersectionMaskPrimitiveType m_nPrimitiveType;
	VROverlayIntersectionMaskPrimitive_Data_t m_Primitive;
};
""")

print ("""
// OpenVR Function Pointer Tables
""")
api_shared.outputfntables(namespace, data)

print ("""
#if 0
// Global entry points
S_API intptr_t VR_InitInternal( EVRInitError *peError, EVRApplicationType eType );
S_API void VR_ShutdownInternal();
S_API bool VR_IsHmdPresent();
S_API intptr_t VR_GetGenericInterface( const char *pchInterfaceVersion, EVRInitError *peError );
S_API bool VR_IsRuntimeInstalled();
S_API const char * VR_GetVRInitErrorAsSymbol( EVRInitError error );
S_API const char * VR_GetVRInitErrorAsEnglishDescription( EVRInitError error );
#endif

#endif // __OPENVR_API_FLAT_H__

""")

