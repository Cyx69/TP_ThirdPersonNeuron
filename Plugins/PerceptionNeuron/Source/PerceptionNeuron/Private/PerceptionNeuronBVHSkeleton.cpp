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


// Sets default values
PerceptionNeuronBVHSkeleton::PerceptionNeuronBVHSkeleton()
{

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
	int32 bone = 0;
	bool  bToggle = false;

	for (int32 i = 0; i < Lines.Num(); i++)
	{
		Lines[i].Trim();
		if ((Lines[i].StartsWith(TEXT("ROOT"), ESearchCase::IgnoreCase) == true) ||
			(Lines[i].StartsWith(TEXT("JOINT"), ESearchCase::IgnoreCase) == true))
		{
			TArray<FString> Words;
			if (Lines[i].ParseIntoArray(Words, TEXT(" "), false) >= 2)
			{
				Bones[bone].Name = Words[1];
				bToggle = true;
			}
		}
		else if (Lines[i].StartsWith(TEXT("OFFSET"), ESearchCase::IgnoreCase) == true)
		{
			if (bToggle == true)	// Ignore additional offsets from "End Site"
			{
				TArray<FString> Words;
				if (Lines[i].ParseIntoArray(Words, TEXT(" "), false) >= 4)
				{
					Bones[bone].Offset[0] = FCString::Atof(*Words[1]);
					Bones[bone].Offset[1] = FCString::Atof(*Words[2]);
					Bones[bone].Offset[2] = FCString::Atof(*Words[3]);
					bToggle = false;
				}
			}
		}
		else if (Lines[i].StartsWith(TEXT("CHANNELS"), ESearchCase::IgnoreCase) == true)
		{
			TArray<FString> Words;
			if (Lines[i].ParseIntoArray(Words, TEXT(" "), false) >= 4)
			{
				Bones[bone].ChannelCount = FCString::Atoi(*Words[1]);
				if (Bones[bone].ChannelCount != 6)
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Channelcount must be 6 on bone:%u"), bone));
					}
					return false;
				}
				Bones[bone].XPos = Bones[bone].YPos = Bones[bone].ZPos = 0;
				Bones[bone].XRot = Bones[bone].YRot = Bones[bone].ZRot = 0;
				for (int32 j = 0; j < Bones[bone].ChannelCount; j++)
				{
					// Detect channel order
					// E.g.: Xposition Yposition Zposition Xrotation Yrotation Zrotation
					if (Words[j + 2].StartsWith(TEXT("Xposition"), ESearchCase::IgnoreCase) == true)
						Bones[bone].XPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Yposition"), ESearchCase::IgnoreCase) == true)
						Bones[bone].YPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Zposition"), ESearchCase::IgnoreCase) == true)
						Bones[bone].ZPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Xrotation"), ESearchCase::IgnoreCase) == true)
						Bones[bone].XRot = j;
					else if (Words[j + 2].StartsWith(TEXT("Yrotation"), ESearchCase::IgnoreCase) == true)
						Bones[bone].YRot = j;
					else if (Words[j + 2].StartsWith(TEXT("Zrotation"), ESearchCase::IgnoreCase) == true)
						Bones[bone].ZRot = j;
				}
				// Calculate rotation order
				if ((Bones[bone].XRot < Bones[bone].YRot) && (Bones[bone].YRot < Bones[bone].ZRot)) // e.g. 123
					Bones[bone].RotOrder = XYZ;
				else if ((Bones[bone].XRot < Bones[bone].ZRot) && (Bones[bone].ZRot < Bones[bone].YRot)) // e.g. 132
					Bones[bone].RotOrder = XZY;
				else if ((Bones[bone].YRot < Bones[bone].XRot) && (Bones[bone].XRot < Bones[bone].ZRot)) // e.g. 213
					Bones[bone].RotOrder = YXZ;
				else if ((Bones[bone].YRot < Bones[bone].ZRot) && (Bones[bone].ZRot < Bones[bone].XRot)) // e.g. 231
					Bones[bone].RotOrder = YZX;
				else if ((Bones[bone].ZRot < Bones[bone].XRot) && (Bones[bone].XRot < Bones[bone].YRot)) // e.g. 312
					Bones[bone].RotOrder = ZXY;
				else 
					Bones[bone].RotOrder = ZYX;

				if (bone >= MAXBONES)
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Too much bones in BVH file:%u"), bone));
					}
					return false;
				}
				bone++;
			}
		}
	} // end of for

	BoneNr = bone;

	return true;
}
