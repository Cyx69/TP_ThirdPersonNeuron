// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// 
// Copyright 2015 Heiko Fink, All Rights Reserved.
//
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
// 
// Perception Neuron (TM) is a trademark of Beijing Noitom Technology Ltd.
// 
// Description:
// This header defines the BVH skeleton class which is used as a reference for the Perception Neuron (TM) plugin.
// 

#pragma once

#include "Engine.h"
#include "PerceptionNeuronMath.h"


#define MAXBONES	100

class PerceptionNeuronBVHSkeleton
{
public:	
	// Constructor, Set default values
	PerceptionNeuronBVHSkeleton();

	// Parse BVH reference file
	bool ParseBVHReferenceFile(FString BVHFileName);

	// Set rotation order on all bones
	void BonesSetRotOrder(ChannelOrderEnum RotOrder);
	
	// Set motion line channel position on all bones
	bool BonesSetChannels(uint8 XPos, uint8 YPos, uint8 ZPos, uint8 XRot, uint8 YRot, uint8 ZRot);
	
	// Set translation offset on all bones
	void BonesSetOffset(int32 X, int32 Y, int32 Z);

	// Set translation offset on bone
	bool BoneSetOffset(float X, float Y, float Z, int32 BoneIndex);

	typedef struct Bone
	{
		FString Name;		// Name of bone
		float Offset[3];	// x,y,z translation offset
		uint8 ChannelCount; // Should be 6
		// Channel positions in motion line
		uint8 XPos, YPos, ZPos;
		uint8 XRot, YRot, ZRot;
		ChannelOrderEnum RotOrder;
	} BONE;

	BONE Bones[MAXBONES];
	int32 BoneNr = 0;		// How many bones has above Bones array
};
