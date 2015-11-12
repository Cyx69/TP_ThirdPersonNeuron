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
	// Constructor
	PerceptionNeuronBVHSkeleton();

	// Parse BVH reference file
	bool ParseBVHReferenceFile(FString BVHFileName);

	typedef struct Bone
	{
		FString Name;		// Name of bone
		float Offset[3];	// x,y,z translation offset
		int32 ChannelCount; // Should be 6
		// Channel positions in motion line
		int32 XPos, YPos, ZPos;
		int32 XRot, YRot, ZRot;
		ChannelOrderEnum RotOrder;
	} BONE;

	BONE Bones[MAXBONES];
	int32 BoneNr = 0;		// How many bones has above Skeleton array
};
