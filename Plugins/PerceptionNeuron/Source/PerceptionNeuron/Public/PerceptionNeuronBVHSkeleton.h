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
		// Name of bone
		FString Name;

		// x,y,z joint offset
		float Offset[3];	
		
		// Motion line channel informations
		uint8 ChannelCount;			// Number of channels per bone. Should be 6 or 3 (with or without displacement)
		uint8 XPos, YPos, ZPos;		// Position of translation channels
		uint8 XRot, YRot, ZRot;		// Position of rotation channels
		ChannelOrderEnum RotOrder;	// Rotation order
	} BONE;

	BONE Bones[MAXBONES];
	int32 BoneNr = 0;		// How many bones has above Bones array
};
