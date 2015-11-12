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
// This code implements the Controller functions that are used to connect to the Perception Neuron (TM) Axis Player.
// 

#include "PerceptionNeuronPrivatePCH.h"
#include "PerceptionNeuronBVHSkeleton.h"


// Constructor, Set default values
PerceptionNeuronBVHSkeleton::PerceptionNeuronBVHSkeleton()
{
	for (BONE &bone : Bones)
	{
		bone.Offset[0] = bone.Offset[1] = bone.Offset[2] = 0.0f;
		bone.ChannelCount = 6;
		bone.XPos = 0;
		bone.YPos = 1;
		bone.ZPos = 2;
		bone.XRot = 4;
		bone.YRot = 3;
		bone.ZRot = 5;
		bone.RotOrder = YXZ;
	}
}

// Parse BVH reference file
bool PerceptionNeuronBVHSkeleton::ParseBVHReferenceFile(FString BVHFileName)
{
	// Load BVH Reference file
	TArray<FString> Lines;
	if (FFileHelper::LoadANSITextFileToStrings(*(FPaths::GameDir() + FString("Content/") + BVHFileName), NULL, Lines) != true)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not load BVH file:%s"), *(FPaths::GameDir() + FString("Content/") + BVHFileName)));
		}
		return false;
	}

	// Parse BVH file and get bone names, offsets and channels
	int32 Bone = 0;
	bool  bToggle = false;

	for (FString &Line : Lines)
	{
		Line.Trim();
		if ((Line.StartsWith(TEXT("ROOT"), ESearchCase::IgnoreCase) == true) ||
			(Line.StartsWith(TEXT("JOINT"), ESearchCase::IgnoreCase) == true))
		{
			TArray<FString> Words;
			if (Line.ParseIntoArray(Words, TEXT(" "), false) >= 2)
			{
				Bones[Bone].Name = Words[1];
				bToggle = true;
			}
		}
		else if (Line.StartsWith(TEXT("OFFSET"), ESearchCase::IgnoreCase) == true)
		{
			if (bToggle == true)	// Ignore additional offsets from "End Site"
			{
				TArray<FString> Words;
				if (Line.ParseIntoArray(Words, TEXT(" "), false) >= 4)
				{
					Bones[Bone].Offset[0] = FCString::Atof(*Words[1]);
					Bones[Bone].Offset[1] = FCString::Atof(*Words[2]);
					Bones[Bone].Offset[2] = FCString::Atof(*Words[3]);
					bToggle = false;
				}
			}
		}
		else if (Line.StartsWith(TEXT("CHANNELS"), ESearchCase::IgnoreCase) == true)
		{
			TArray<FString> Words;
			if (Line.ParseIntoArray(Words, TEXT(" "), false) >= 4)
			{
				Bones[Bone].ChannelCount = FCString::Atoi(*Words[1]);
				if (Bones[Bone].ChannelCount != 6)
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Channelcount must be 6 on bone:%u"), Bone));
					}
					return false;
				}
				Bones[Bone].XPos = Bones[Bone].YPos = Bones[Bone].ZPos = 0;
				Bones[Bone].XRot = Bones[Bone].YRot = Bones[Bone].ZRot = 0;
				for (int32 j = 0; j < Bones[Bone].ChannelCount; j++)
				{
					// Detect channel order
					// E.g.: Xposition Yposition Zposition Xrotation Yrotation Zrotation
					if (Words[j + 2].StartsWith(TEXT("Xposition"), ESearchCase::IgnoreCase) == true)
						Bones[Bone].XPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Yposition"), ESearchCase::IgnoreCase) == true)
						Bones[Bone].YPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Zposition"), ESearchCase::IgnoreCase) == true)
						Bones[Bone].ZPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Xrotation"), ESearchCase::IgnoreCase) == true)
						Bones[Bone].XRot = j;
					else if (Words[j + 2].StartsWith(TEXT("Yrotation"), ESearchCase::IgnoreCase) == true)
						Bones[Bone].YRot = j;
					else if (Words[j + 2].StartsWith(TEXT("Zrotation"), ESearchCase::IgnoreCase) == true)
						Bones[Bone].ZRot = j;
				}
				// Calculate rotation order
				if ((Bones[Bone].XRot < Bones[Bone].YRot) && (Bones[Bone].YRot < Bones[Bone].ZRot)) // e.g. 123
					Bones[Bone].RotOrder = XYZ;
				else if ((Bones[Bone].XRot < Bones[Bone].ZRot) && (Bones[Bone].ZRot < Bones[Bone].YRot)) // e.g. 132
					Bones[Bone].RotOrder = XZY;
				else if ((Bones[Bone].YRot < Bones[Bone].XRot) && (Bones[Bone].XRot < Bones[Bone].ZRot)) // e.g. 213
					Bones[Bone].RotOrder = YXZ;
				else if ((Bones[Bone].YRot < Bones[Bone].ZRot) && (Bones[Bone].ZRot < Bones[Bone].XRot)) // e.g. 231
					Bones[Bone].RotOrder = YZX;
				else if ((Bones[Bone].ZRot < Bones[Bone].XRot) && (Bones[Bone].XRot < Bones[Bone].YRot)) // e.g. 312
					Bones[Bone].RotOrder = ZXY;
				else 
					Bones[Bone].RotOrder = ZYX;

				if (Bone >= MAXBONES)
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Too much bones in BVH file:%u"), Bone));
					}
					return false;
				}
				Bone++;
			}
		}
	} // end of for

	BoneNr = Bone;

	return true;
}

// Set rotation order on all bones
void PerceptionNeuronBVHSkeleton::BonesSetRotOrder(ChannelOrderEnum RotOrder)
{
	for (BONE &Bone : Bones)
		Bone.RotOrder = RotOrder;
}

// Set motion line channel position on all bones
bool PerceptionNeuronBVHSkeleton::BonesSetChannels(uint8 XPos, uint8 YPos, uint8 ZPos, uint8 XRot, uint8 YRot, uint8 ZRot)
{	
	for (BONE &Bone : Bones)
	{
		if ((XPos >= Bone.ChannelCount) || (YPos >= Bone.ChannelCount) || (ZPos >= Bone.ChannelCount) || (XRot >= Bone.ChannelCount) || (YRot >= Bone.ChannelCount) || (ZRot >= Bone.ChannelCount))
			return false;

		Bone.XPos = XPos;
		Bone.YPos = YPos;
		Bone.ZPos = ZPos;
		Bone.XRot = XRot;
		Bone.YRot = YRot;
		Bone.ZRot = ZRot;
	}
	return true;
}

// Set translation offset on all bones
void PerceptionNeuronBVHSkeleton::BonesSetOffset(int32 X, int32 Y, int32 Z)
{
	for (BONE &Bone : Bones)
	{
		Bone.Offset[0] = X;
		Bone.Offset[1] = Y;
		Bone.Offset[2] = Z;
	}
}

// Set translation offset on bone
bool PerceptionNeuronBVHSkeleton::BoneSetOffset(float X, float Y, float Z, int32 BoneIndex)
{
	if (BoneIndex > MAXBONES)
		return false;

	Bones[BoneIndex].Offset[0] = X;
	Bones[BoneIndex].Offset[1] = Y;
	Bones[BoneIndex].Offset[2] = Z;

	return true;
}
