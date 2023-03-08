//============ Copyright (c) Valve Corporation, All rights reserved. ============
#include "controller_device_driver.h"

#include "driverlog.h"
#include "vrmath.h"

// Let's create some variables for strings used in getting settings.
// This is the section where all of the settings we want are stored. A section name can be anything,
// but if you want to store driver specific settings, it's best to namespace the section with the driver identifier
// ie "<my_driver>_<section>" to avoid collisions
static const char *my_controller_main_settings_section = "driver_handskeletonsimulation";

// Individual right/left hand settings sections
static const char *my_controller_right_settings_section = "driver_handskeletonsimulation_left_controller";
static const char *my_controller_left_settings_section = "driver_handskeletonsimulation_right_controller";

// These are the keys we want to retrieve the values for in the settings
static const char *my_controller_settings_key_model_number = "model_number";
static const char *my_controller_settings_key_serial_number = "serial_number";


MyControllerDeviceDriver::MyControllerDeviceDriver( vr::ETrackedControllerRole role )
{
	// we're not activated yet
	is_active_ = false;

	// The constructor takes a role argument, that gives us information about if our controller is a left or right hand.
	// Let's store it for later use. We'll need it.
	my_controller_role_ = role;

	// We have our model number and serial number stored in SteamVR settings. We need to get them and do so here.
	// Other IVRSettings methods (to get int32, floats, bools) return the data, instead of modifying, but strings are
	// different.
	char model_number[ 1024 ];
	vr::VRSettings()->GetString( my_controller_main_settings_section, my_controller_settings_key_model_number, model_number, sizeof( model_number ) );
	my_controller_model_number_ = model_number;

	// Get our serial number depending on our "handedness"
	char serial_number[ 1024 ];
	vr::VRSettings()->GetString( my_controller_role_ == vr::TrackedControllerRole_LeftHand ? my_controller_left_settings_section : my_controller_right_settings_section,
		my_controller_settings_key_serial_number, serial_number, sizeof( serial_number ) );
	my_controller_serial_number_ = serial_number;

	// Here's an example of how to use our logging wrapper around IVRDriverLog
	// In SteamVR logs (SteamVR Hamburger Menu > Developer Settings > Web console) drivers have a prefix of
	// "<driver_name>:". You can search this in the top search bar to find the info that you've logged.
	DriverLog( "My Controller Model Number: %s", my_controller_model_number_.c_str() );
	DriverLog( "My Controller Serial Number: %s", my_controller_serial_number_.c_str() );
}

//-----------------------------------------------------------------------------
// Purpose: This is called by vrserver after our
//  IServerTrackedDeviceProvider calls IVRServerDriverHost::TrackedDeviceAdded.
//-----------------------------------------------------------------------------
vr::EVRInitError MyControllerDeviceDriver::Activate( uint32_t unObjectId )
{
	// Let's keep track of our device index. It'll be useful later.
	my_controller_index_ = unObjectId;

	// Properties are stored in containers, usually one container per device index. We need to get this container to set
	// The properties we want, so we call this to retrieve a handle to it.
	vr::PropertyContainerHandle_t container = vr::VRProperties()->TrackedDeviceToPropertyContainer( my_controller_index_ );

	// Let's begin setting up the properties now we've got our container.
	// A list of properties available is contained in vr::ETrackedDeviceProperty.

	// First, let's set the model number.
	vr::VRProperties()->SetStringProperty( container, vr::Prop_ModelNumber_String, my_controller_model_number_.c_str() );

	// Let's tell SteamVR our role which we received from the constructor earlier.
	vr::VRProperties()->SetInt32Property( container, vr::Prop_ControllerRoleHint_Int32, my_controller_role_ );


	// Now let's set up our inputs

	// This tells the UI what to show the user for bindings for this controller,
	// As well as what default bindings should be for legacy apps.
	// Note, we can use the wildcard {<driver_name>} to match the root folder location
	// of our driver.
	vr::VRProperties()->SetStringProperty( container, vr::Prop_InputProfilePath_String, "{indexcontroller}/input/index_controller_profile.json" );
	vr::VRProperties()->SetStringProperty( container, vr::Prop_ControllerType_String, "knuckles" );


	// Let's set up some inputs for our curls. Not strictly needed, but cool to have.
	vr::VRDriverInput()->CreateScalarComponent( container, "/input/finger/index", &input_handles_[ MyComponent_indexFinger ], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided );
	vr::VRDriverInput()->CreateScalarComponent( container, "/input/finger/middle", &input_handles_[ MyComponent_middleFinger ], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided );
	vr::VRDriverInput()->CreateScalarComponent( container, "/input/finger/ring", &input_handles_[ MyComponent_ringFinger ], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided );
	vr::VRDriverInput()->CreateScalarComponent( container, "/input/finger/pinky", &input_handles_[ MyComponent_pinkyFinger ], vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided );

	// Now our skeletons. This is slightly harder.
	vr::EVRInputError err = vr::VRDriverInput()->CreateSkeletonComponent( container,								  // container
		my_controller_role_ == vr::TrackedControllerRole_LeftHand ? "/input/skeleton/left" : "/input/skeleton/right", // The path to the skeleton for input. This is for binding to applications.
		my_controller_role_ == vr::TrackedControllerRole_LeftHand ? "/skeleton/hand/left" : "/skeleton/hand/right",	  // Where on the body this skeleton is.
		"/pose/raw",							// Where the origin for the skeleton is going to be. These pose locations are defined in the render model file.
		vr::VRSkeletalTracking_Full,			// Inform the runtime about the capabilities of the device.
		nullptr,								// Used for calculating curl and splay values. If this is null then defaults are used.
		0,										// How many bones there are in the gripLimitTransforms above.
		&input_handles_[ MyComponent_skeleton ] // Bind the component to a handle.
	);

	// SteamVR wants skeletal data immediately.
	MyInputThread();
	MyInputThread();

	// initialise our hand tracking simulation class
	my_hand_simulation_ = std::make_unique< MyHandSimulation >();

	// create a thread for updating our skeleton
	is_active_ = true;
	my_input_thread_ = std::thread( &MyControllerDeviceDriver::MyInputThread, this );

	// We've activated everything successfully!
	// Let's tell SteamVR that by saying we don't have any errors.
	return vr::VRInitError_None;
}

//-----------------------------------------------------------------------------
// Purpose: If you're an HMD, this is where you would return an implementation
// of vr::IVRDisplayComponent, vr::IVRVirtualDisplay or vr::IVRDirectModeComponent.
//
// But this a simple example to demo for a controller, so we'll just return nullptr here.
//-----------------------------------------------------------------------------
void *MyControllerDeviceDriver::GetComponent( const char *pchComponentNameAndVersion )
{
	return nullptr;
}

//-----------------------------------------------------------------------------
// Purpose: This is called by vrserver when a debug request has been made from an application to the driver.
// What is in the response and request is up to the application and driver to figure out themselves.
//-----------------------------------------------------------------------------
void MyControllerDeviceDriver::DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize )
{
	if ( unResponseBufferSize >= 1 )
		pchResponseBuffer[ 0 ] = 0;
}

//-----------------------------------------------------------------------------
// Purpose: This is called by vrserver when the device should enter standby mode.
// The device should be put into whatever low power mode it has
//-----------------------------------------------------------------------------
void MyControllerDeviceDriver::EnterStandby()
{
}

//-----------------------------------------------------------------------------
// Purpose: This is called by vrserver when the device should deactivate.
// This is typically at the end of a session
// The device should free any resources it has allocated here.
//-----------------------------------------------------------------------------
void MyControllerDeviceDriver::Deactivate()
{
	// unassign our controller index (we don't want to be calling vrserver any more after Deactivate() has been called
	my_controller_index_ = vr::k_unTrackedDeviceIndexInvalid;

	if ( is_active_.exchange( false ) )
	{
		my_input_thread_.join();
	}
}


//-----------------------------------------------------------------------------
// Purpose: This is never called by vrserver, but is useful for giving data to
// vr::VRServerDriverHost::TrackedDevicePoseUpdated.
//-----------------------------------------------------------------------------
vr::DriverPose_t MyControllerDeviceDriver::GetPose()
{
	// Let's retrieve the Hmd pose to base our controller pose off.

	// First, initialize the struct that we'll be submitting to the runtime to tell it we've updated our pose.
	vr::DriverPose_t pose = { 0 };

	// These need to be set to be valid quaternions. The device won't appear otherwise.
	pose.qWorldFromDriverRotation.w = 1.f;
	pose.qDriverFromHeadRotation.w = 1.f;

	vr::TrackedDevicePose_t hmd_pose{};

	// GetRawTrackedDevicePoses expects an array.
	// We only want the hmd pose, which is at index 0 of the array so we can just pass the struct in directly, instead of in an array
	vr::VRServerDriverHost()->GetRawTrackedDevicePoses( 0.f, &hmd_pose, 1 );

	// Get the position of the hmd from the 3x4 matrix GetRawTrackedDevicePoses returns
	const vr::HmdVector3_t hmd_position = HmdVector3_From34Matrix( hmd_pose.mDeviceToAbsoluteTracking );
	// Get the orientation of the hmd from the 3x4 matrix GetRawTrackedDevicePoses returns
	const vr::HmdQuaternion_t hmd_orientation = HmdQuaternion_FromMatrix( hmd_pose.mDeviceToAbsoluteTracking );

	const vr::HmdQuaternion_t offset_orientation = HmdQuaternion_FromEulerAngles( DEG_TO_RAD( 90.f ), DEG_TO_RAD( 90.f ), 0.f );

	// Set the pose orientation to the hmd orientation with the offset applied.
	pose.qRotation = hmd_orientation * offset_orientation;

	const vr::HmdVector3_t offset_position = {
		my_controller_role_ == vr::TrackedControllerRole_LeftHand ? -0.15f : 0.15f, // translate the controller left/right 0.15m depending on its role
		0.1f,																		// shift it up a little to make it more in view
		-0.5f,																		// put each controller 0.5m forward in front of the hmd so we can see it.
	};

	// Rotate our offset by the hmd quaternion (so the controllers are always facing towards us), and add then add the position of the hmd to put it into position.
	const vr::HmdVector3_t position = hmd_position + ( offset_position * hmd_orientation );

	// copy our position to our pose
	pose.vecPosition[ 0 ] = position.v[ 0 ];
	pose.vecPosition[ 1 ] = position.v[ 1 ];
	pose.vecPosition[ 2 ] = position.v[ 2 ];

	// The pose we provided is valid.
	// This should be set is
	pose.poseIsValid = true;

	// Our device is always connected.
	// In reality with physical devices, when they get disconnected,
	// set this to false and icons in SteamVR will be updated to show the device is disconnected
	pose.deviceIsConnected = true;

	// The state of our tracking. For our virtual device, it's always going to be ok,
	// but this can get set differently to inform the runtime about the state of the device's tracking
	// and update the icons to inform the user accordingly.
	pose.result = vr::TrackingResult_Running_OK;

	return pose;
}

void MyControllerDeviceDriver::MyInputThread()
{
	while ( is_active_ )
	{
		if ( frame_ >= 4000 )
		{
			frame_ = 0;
		}

		const int op = frame_ % 4000;
		if ( op < 1000 ) // curl 0 -> 1
		{
			last_curl_ = last_curl_ + 0.001f;
		}
		else if ( op < 2000 ) // curl 1 -> 0
		{
			last_curl_ = last_curl_ - 0.001f;
		}
		else if ( op < 2500 ) // splay 0 -> 1
		{
			last_splay_ = last_splay_ + 0.002f;
		}
		else if ( op < 3500 ) // splay 1 -> -1
		{
			last_splay_ = last_splay_ - 0.002f;
		}
		else // splay -1 -> 0
		{
			last_splay_ = last_splay_ + 0.002f;
		}


		vr::VRBoneTransform_t transforms[ 31 ];
		// Pass our calculated curl and splay values to our skeleton simulation model to compute the bone transforms for.
		my_hand_simulation_->ComputeSkeletonTransforms(
			my_controller_role_, { last_curl_, last_curl_, last_curl_, last_curl_, last_curl_ }, { last_splay_, last_splay_, last_splay_, last_splay_, last_splay_ }, transforms );
		// Update the skeleton components.
		// Applications can choose between using a skeleton as if it's holding a controller, or an interpretation with having it without one.
		// As ours is just a simulation, let's just set them to the same transforms.
		vr::VRDriverInput()->UpdateSkeletonComponent( input_handles_[ MyComponent_skeleton ], vr::VRSkeletalMotionRange_WithController, transforms, 31 );
		vr::VRDriverInput()->UpdateSkeletonComponent( input_handles_[ MyComponent_skeleton ], vr::VRSkeletalMotionRange_WithoutController, transforms, 31 );


		// We'll also update our pose here as well
		vr::VRServerDriverHost()->TrackedDevicePoseUpdated( my_controller_index_, GetPose(), sizeof( vr::DriverPose_t ) );

		frame_++;
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}
}


//-----------------------------------------------------------------------------
// Purpose: Our IServerTrackedDeviceProvider needs our serial number to add us to vrserver.
// It's not part of the ITrackedDeviceServerDriver interface, we created it ourselves.
//-----------------------------------------------------------------------------
const std::string &MyControllerDeviceDriver::MyGetSerialNumber()
{
	return my_controller_serial_number_;
}