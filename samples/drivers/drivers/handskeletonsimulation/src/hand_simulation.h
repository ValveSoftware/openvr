//============ Copyright (c) Valve Corporation, All rights reserved. ============
#pragma once

#include "openvr_driver.h"

// 0-1 values (1 fully curled)
struct MyFingerCurls
{
	float thumb;
	float index;
	float middle;
	float ring;
	float pinky;
};

//-1-1 values (1 fully to the left)
struct MyFingerSplays
{
	float thumb;
	float index;
	float middle;
	float ring;
	float pinky;
};

enum HandSkeletonBone : vr::BoneIndex_t
{
	eBone_Root = 0,
	eBone_Wrist,
	eBone_Thumb0,
	eBone_Thumb1,
	eBone_Thumb2,
	eBone_Thumb3,
	eBone_IndexFinger0,
	eBone_IndexFinger1,
	eBone_IndexFinger2,
	eBone_IndexFinger3,
	eBone_IndexFinger4,
	eBone_MiddleFinger0,
	eBone_MiddleFinger1,
	eBone_MiddleFinger2,
	eBone_MiddleFinger3,
	eBone_MiddleFinger4,
	eBone_RingFinger0,
	eBone_RingFinger1,
	eBone_RingFinger2,
	eBone_RingFinger3,
	eBone_RingFinger4,
	eBone_PinkyFinger0,
	eBone_PinkyFinger1,
	eBone_PinkyFinger2,
	eBone_PinkyFinger3,
	eBone_PinkyFinger4,
	eBone_Aux_Thumb,
	eBone_Aux_IndexFinger,
	eBone_Aux_MiddleFinger,
	eBone_Aux_RingFinger,
	eBone_Aux_PinkyFinger,
	eBone_Count
};

class MyHandSimulation
{
public:
	void ComputeSkeletonTransforms( vr::ETrackedControllerRole role, const MyFingerCurls &curls, const MyFingerSplays &splays, vr::VRBoneTransform_t *out_transforms );
};