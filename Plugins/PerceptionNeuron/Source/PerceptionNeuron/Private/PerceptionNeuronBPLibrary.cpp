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
// This code implements the blueprint functions that are provided by the Perception Neuron (TM) Blueprint Function Library.
// 
#include "PerceptionNeuronPrivatePCH.h"
#include "PerceptionNeuronBPLibrary.h"
#include "PerceptionNeuronMath.h"


UPerceptionNeuronBPLibrary::UPerceptionNeuronBPLibrary(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	// Nothing todo
}

// Init with file and read BVH reference skeleton
bool UPerceptionNeuronBPLibrary::NeuronInitFile(APerceptionNeuronController *Controller, FString BVHFileName)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	if (Controller->Skeleton.ParseBVHReferenceFile(BVHFileName) != true)
		return false;

	return true;
}

// Init and configure BVH reference skeleton
bool UPerceptionNeuronBPLibrary::NeuronInit(APerceptionNeuronController *Controller, int32 BoneNr, ENeuronRotOrderEnum RotationOrder, ENeuronChannelNumberEnum XPos, ENeuronChannelNumberEnum YPos, ENeuronChannelNumberEnum ZPos, ENeuronChannelNumberEnum XRot, ENeuronChannelNumberEnum YRot, ENeuronChannelNumberEnum ZRot)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	Controller->Skeleton.BoneNr = BoneNr;
	Controller->Skeleton.BonesSetRotOrder((ChannelOrderEnum)RotationOrder);
	return Controller->Skeleton.BonesSetChannels((uint8)XPos, (uint8)YPos, (uint8)ZPos, (uint8)XRot, (uint8)YRot, (uint8)ZRot);
}

// Init and configure BVH reference skeleton
bool UPerceptionNeuronBPLibrary::NeuronBoneSetOffset(APerceptionNeuronController *Controller, int32 BoneIndex, float X, float Y, float Z)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	return Controller->Skeleton.BoneSetOffset(BoneIndex, X, Y, Z);
}

// Connect to Axis Neuron
bool UPerceptionNeuronBPLibrary::NeuronConnect(APerceptionNeuronController *Controller, FString HostName, int32 Port, bool bReference, bool bDisplacement, ENeuronMotionLineFormatEnum MotionLineFormat)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	Controller->bReference = bReference;
	Controller->bDisplacement = bDisplacement;
	if (MotionLineFormat == ENeuronMotionLineFormatEnum::VE_Neuron)
		Controller->MotionLineFormat = Neuron;
	else
		Controller->MotionLineFormat = Standard;

	if (Controller->Connect(HostName, Port) != true)
		return false;

	return true;
}

// Disconnect from Axis Neuron
bool UPerceptionNeuronBPLibrary::NeuronDisconnect(APerceptionNeuronController *Controller)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	Controller->Disconnect();

	return true;
}

// Play BVH file
bool UPerceptionNeuronBPLibrary::NeuronPlay(APerceptionNeuronController *Controller, FString BVHFileName, bool bEndless, bool bReference, bool bDisplacement, ENeuronMotionLineFormatEnum MotionLineFormat)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	Controller->bReference = bReference;
	Controller->bDisplacement = bDisplacement;
	if (MotionLineFormat == ENeuronMotionLineFormatEnum::VE_Neuron)
		Controller->MotionLineFormat = Neuron;
	else
		Controller->MotionLineFormat = Standard;

	if (Controller->Play(BVHFileName, bEndless) != true)
		return false;

	return true;
}

// Pause playing BVH file
bool UPerceptionNeuronBPLibrary::NeuronPause(APerceptionNeuronController *Controller, bool bPause)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	return (Controller->Pause(bPause));
}

// Rewind BVH file player
bool UPerceptionNeuronBPLibrary::NeuronRewind(APerceptionNeuronController *Controller)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	return (Controller->Rewind());
}

// Read motion data from Axis Neuron
bool UPerceptionNeuronBPLibrary::NeuronRead(APerceptionNeuronController *Controller, USkeletalMeshComponent *Mesh, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, EPerceptionNeuronBonesEnum BVHBone, FName CustomBoneName, bool InverseForward)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}
	else if ((Controller->bConnected == false) && (Controller->bPlay == false))
	{
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}

	int32 BVHBoneIndex = (int32)BVHBone;
	if (BVHBoneIndex >= Controller->Skeleton.BoneNr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boneindex %d exceeds maximum available bones %d."), BVHBoneIndex, Controller->Skeleton.BoneNr));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}

	int32 FloatsPerBone = 6; // 3 for x,y,z translation and 3 for x,y,z rotation
	if (Controller->bDisplacement == false)
	{
		FloatsPerBone = 3;	// If there is no displacement (translation) info we have only 3 floats for rotation left
	}
	if ((BVHBoneIndex * FloatsPerBone) > Controller->FloatCount)
	{
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}

	// Get the rotation of the reference pose bone
	FQuat RefQuat(FQuat::Identity);
	if (Mesh != nullptr && Mesh->SkeletalMesh != nullptr)
	{
		const FReferenceSkeleton& RefSkeleton(Mesh->SkeletalMesh->RefSkeleton);
		int32 RefBoneIndex = Mesh->GetBoneIndex(CustomBoneName);
		while (RefBoneIndex != INDEX_NONE)
		{
			RefQuat = RefSkeleton.GetRefBonePose()[RefBoneIndex].GetRotation() * RefQuat;
			RefBoneIndex = RefSkeleton.GetParentIndex(RefBoneIndex);
		}
	}

	//
	// Translation
	//

	if (Controller->bDisplacement == true)
	{
		// Read translation values and remove BVH reference position
		float X = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].XPos] - Controller->Skeleton.Bones[BVHBoneIndex].Offset[0];
		float Y = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].YPos] - Controller->Skeleton.Bones[BVHBoneIndex].Offset[1];
		float Z = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].ZPos] - Controller->Skeleton.Bones[BVHBoneIndex].Offset[2];

		// Map BVH translation to UE4 world coordinate system (Inverse if forward direction is -Y)
		if (InverseForward)
		{
			Translation = FVector(-X, -Z, Y);
		}			
		else
		{
			Translation = FVector(X, Z, Y);
		}			

		// Map UE4 world translation to bone space	
		Translation = RefQuat.Inverse().RotateVector(Translation);
	}
	else
	{
		Translation.X = Translation.Y = Translation.Z = 0;
	}

	// Add additional translation
	Translation.X += AdditionalTranslation.X;
	Translation.Y += AdditionalTranslation.Y;
	Translation.Z += AdditionalTranslation.Z;


	//
	// Rotation 
	//

	// Read rotation values and map to pitch, yaw, roll (y, z, x)
	float XR = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].XRot] * PI / 180.f;
	float YR = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].YRot] * PI / 180.f;
	float ZR = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].ZRot] * PI / 180.f;

	// Calculate Rotation Matrix and map to Quaternion
	FQuat BVHQuat = CalculateQuat(XR, YR, ZR, Controller->Skeleton.Bones[BVHBoneIndex].RotOrder);

	// Map BVH rotation to UE4 world coordinate system (Inverse if forward direction is -Y)
	float Y = BVHQuat.Y;
	float Z = BVHQuat.Z;
	if (InverseForward)
	{
		BVHQuat.X *= -1.0;
		BVHQuat.Y = -Z;
		BVHQuat.Z = Y;
	}
	else
	{
		BVHQuat.Y = Z;
		BVHQuat.Z = Y;
	}

	// Map UE4 world rotation to bone space
	FQuat Quat(RefQuat.Inverse() * BVHQuat * RefQuat);
	
	// Add additional rotation
	FQuat AddRot(AdditionalRotation);
	Quat *= AddRot;

	// Convert to Rotator
	Rotation = Quat.Rotator();
	Rotation.Normalize();

	return true;
}

// Read motion as array
bool UPerceptionNeuronBPLibrary::NeuronReadArray(APerceptionNeuronController *Controller, USkeletalMeshComponent *Mesh, TArray<FVector> &Translation, TArray<FRotator> &Rotation, TArray<FVector> AdditionalTranslation, TArray<FRotator> AdditionalRotation, TArray<FPerceptionNeuronBoneMapStruct> BoneMap, bool InverseForward)
{
	for (int32 BoneIndex = 0; BoneIndex < BoneMap.Num(); BoneIndex++)
	{
		EPerceptionNeuronBonesEnum BVHBoneEnum = BoneMap[BoneIndex].BVHBone;
		FName CustomBoneName = BoneMap[BoneIndex].CustomBoneName;
		FVector TempTranslation;
		FRotator TempRotation;
		if (false == NeuronRead(Controller, Mesh, TempTranslation, TempRotation, AdditionalTranslation[BoneIndex], AdditionalRotation[BoneIndex], BVHBoneEnum, CustomBoneName, InverseForward))
			return false;
		Translation.Add(TempTranslation);
		Rotation.Add(TempRotation);
	}

	return true;
}

// Get local bone rotation from mesh
bool UPerceptionNeuronBPLibrary::NeuronGetLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex)
{
	if (Mesh == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Mesh is invalid.")));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		return false;
	}

	if (BoneIndex > Mesh->LocalAtoms.Num())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BoneIndex %d exceeds maximum available bones %d."), BoneIndex, Mesh->LocalAtoms.Num()));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		return false;
	}

	Rotation = Mesh->LocalAtoms[BoneIndex].Rotator();

	return true;
}

// Get local bone location from mesh
bool UPerceptionNeuronBPLibrary::NeuronGetLocalBoneLocation(USkeletalMeshComponent *Mesh, FVector& Location, int32 BoneIndex)
{
	if (Mesh == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Mesh is invalid.")));
		}
		Location.X = Location.Y = Location.Z = 0;
		return false;
	}

	if (BoneIndex > Mesh->LocalAtoms.Num())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BoneIndex %d exceeds maximum available bones %d."), BoneIndex, Mesh->LocalAtoms.Num()));
		}
		Location.X = Location.Y = Location.Z = 0;
		return false;
	}

	Location = Mesh->LocalAtoms[BoneIndex].GetLocation();

	return true;
}

bool UPerceptionNeuronBPLibrary::NeuronGetReferencePoseLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex)
{
	if (Mesh == nullptr && Mesh->SkeletalMesh == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Mesh is invalid.")));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		return false;
	}

	if (BoneIndex > Mesh->LocalAtoms.Num())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BoneIndex %d exceeds maximum available bones %d."), BoneIndex, Mesh->LocalAtoms.Num()));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		return false;
	}

	const FReferenceSkeleton& refskel(Mesh->SkeletalMesh->RefSkeleton);
	FQuat Quat = refskel.GetRefBonePose()[BoneIndex].GetRotation();
	Rotation = Quat.Rotator();

	return true;
}

// Negate Yaw, Pitch and Roll in rotation vector
FRotator UPerceptionNeuronBPLibrary::NeuronNegateRotation(FRotator Rotation)
{
	FRotator NewRotation;
	NewRotation.Yaw = -Rotation.Yaw;
	NewRotation.Pitch = -Rotation.Pitch;
	NewRotation.Roll = -Rotation.Roll;

	return NewRotation;
}

// Return BVH Bone Index
int32 UPerceptionNeuronBPLibrary::NeuronGetBVHBoneIndex(EPerceptionNeuronBonesEnum BVHBone)
{
	return (int32)BVHBone;
}




//
// Deprecated functions (Do not use in new projects)
//

// Read motion data from Axis Neuron
// Deprecated
bool UPerceptionNeuronBPLibrary::NeuronReadMotion(APerceptionNeuronController *Controller, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, int32 BoneIndex, ENeuronSkeletonEnum SkeletonType)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}
	else if ((Controller->bConnected == false) && (Controller->bPlay == false))
	{
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}
	else if (BoneIndex >= Controller->Skeleton.BoneNr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boneindex %d exceeds maximum available bones %d."), BoneIndex, Controller->Skeleton.BoneNr));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}

	int32 FloatsPerBone = 6; // 3 for x,y,z translation and 3 for x,y,z rotation
	if (Controller->bDisplacement == false)
	{
		FloatsPerBone = 3;	// If there is no displacement (translation) info we have only 3 floats for rotation left
	}

	if ((BoneIndex * FloatsPerBone) > Controller->FloatCount)
	{
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}


	//
	// Translation
	//

	if (Controller->bDisplacement == true)
	{
		// Read translation values and remove BVH reference position
		float X = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BoneIndex].XPos] - Controller->Skeleton.Bones[BoneIndex].Offset[0];
		float Y = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BoneIndex].YPos] - Controller->Skeleton.Bones[BoneIndex].Offset[1];
		float Z = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BoneIndex].ZPos] - Controller->Skeleton.Bones[BoneIndex].Offset[2];

		// Map BVH right hand system to local bone coordinate system
		switch (SkeletonType)
		{
		case ENeuronSkeletonEnum::VE_Neuron:  // Neuron BVH skeleton
		{
			if (BoneIndex == 0)
			{	// Hips
				Translation = FVector(X, -Y, Z);
			}
			else if ((BoneIndex >= 1) && (BoneIndex <= 6))
			{	// Legs
				Translation = FVector(X, Y, -Z);
			}
			else if ((BoneIndex >= 7) && (BoneIndex <= 12))
			{	// Spine,...
				Translation = FVector(X, -Y, -Z);
			}
			else if ((BoneIndex >= 13) && (BoneIndex <= 35))
			{	// Right arm
				Translation = FVector(-Z, X, Y);
			}
			else if ((BoneIndex >= 36) && (BoneIndex <= 58))
			{	// Left arm
				Translation = FVector(Z, -X, Y);
			}
			break;
		}
		case ENeuronSkeletonEnum::VE_TPP_Hero:	// Hero_TPP, Old blue Unreal default skeleton with T-Pose
		case ENeuronSkeletonEnum::VE_Mannequin: // Mannequin, New Unreal default skeleton with A-Pose
		{
			if (BoneIndex == 0)
			{	// Hips
				Translation = FVector(Y, Z, -X);
			}
			// Ignore other bones
			break;
		}
		case ENeuronSkeletonEnum::VE_Map: // Map to configured bone map
		{
			// Map translation with configured Bonemap
			float Map[3] = { X, Y, Z };

			Translation = FVector(Map[Controller->Bonemap[BoneIndex].XYZ[0]] * Controller->Bonemap[BoneIndex].Sign[0],
				Map[Controller->Bonemap[BoneIndex].XYZ[1]] * Controller->Bonemap[BoneIndex].Sign[1],
				Map[Controller->Bonemap[BoneIndex].XYZ[2]] * Controller->Bonemap[BoneIndex].Sign[2]);
			break;
		}
		case ENeuronSkeletonEnum::VE_UE4: // Map to UE4 world coordinate system
		{
			Translation = FVector(X, Z, Y);
			break;
		}
		case ENeuronSkeletonEnum::VE_None: // Map to nothing, use BVH translation as it is
		default:
		{
			Translation = FVector(X, Y, Z);
			break;
		}
		}
	}
	else
	{
		Translation.X = Translation.Y = Translation.Z = 0;
	}

	// Add additional translation
	Translation.X += AdditionalTranslation.X;
	Translation.Y += AdditionalTranslation.Y;
	Translation.Z += AdditionalTranslation.Z;



	//
	// Rotation 
	//

	// Read rotation values and map to pitch, yaw, roll (y, z, x)
	float XR = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BoneIndex].XRot] * PI / 180.f;
	float YR = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BoneIndex].YRot] * PI / 180.f;
	float ZR = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BoneIndex].ZRot] * PI / 180.f;

	// Calculate Rotation Matrix and map to Quaternion
	FQuat Quat = CalculateQuat(XR, YR, ZR, Controller->Skeleton.Bones[BoneIndex].RotOrder);

	// Map BVH coordinate system to each bone coordinate system dependend on skeleton type
	switch (SkeletonType)
	{
	case ENeuronSkeletonEnum::VE_Neuron:  // Neuron BVH skeleton
	{
		if ((BoneIndex >= 1) && (BoneIndex <= 6))
		{	// Legs
			Quat.Z *= -1.f;
		}
		else if ((BoneIndex >= 13) && (BoneIndex <= 35))
		{	// Right Arm
			float X = Quat.X;
			float Y = Quat.Y;
			float Z = Quat.Z;
			Quat.X = -Z;
			Quat.Y = X;
			Quat.Z = Y;
		}
		else if ((BoneIndex >= 36) && (BoneIndex <= 58))
		{	// Left Arm
			float X = Quat.X;
			float Y = Quat.Y;
			float Z = Quat.Z;
			Quat.X = Z;
			Quat.Y = -X;
			Quat.Z = Y;
		}
		else
		{
			Quat.Y *= -1.f;
		}
		break;
	}
	case ENeuronSkeletonEnum::VE_TPP_Hero:	// Hero_TPP, Old blue Unreal default skeleton with T-Pose
	case ENeuronSkeletonEnum::VE_Mannequin: // Mannequin, New Unreal default skeleton with A-Pose
	{
		if ((BoneIndex >= 1) && (BoneIndex <= 3))
		{	// Right Leg
			float X = Quat.X;
			float Y = Quat.Y;
			float Z = Quat.Z;
			Quat.X = -Y;
			Quat.Y = -Z;
			Quat.Z = -X;
		}
		else if (BoneIndex == 16)
		{	// Right Hand
			Quat.Y *= -1.f;
		}
		else if ((BoneIndex >= 13) && (BoneIndex <= 19))
		{	// Right Arm and Thumb
			float Y = Quat.Y;
			float Z = Quat.Z;
			Quat.Y = -Z;
			Quat.Z = -Y;
		}
		else if ((BoneIndex >= 20) && (BoneIndex <= 35))
		{	// Right Finger
			Quat.Y *= -1.f;
		}
		else if (BoneIndex == 39)
		{	// Left Hand
			Quat.Z *= -1.f;
		}
		else if ((BoneIndex >= 36) && (BoneIndex <= 42))
		{	// Left Arm and Thumb
			float Y = Quat.Y;
			float Z = Quat.Z;
			Quat.Y = Z;
			Quat.Z = Y;
		}
		else if ((BoneIndex >= 43) && (BoneIndex <= 58))
		{	// Left Finger
			Quat.Z *= -1.f;
		}
		else
		{	// Left Leg, Hips, Spine, Neck, Head
			float X = Quat.X;
			float Y = Quat.Y;
			float Z = Quat.Z;
			Quat.X = Y;
			Quat.Y = Z;
			Quat.Z = -X;
		}
		break;
	}
	case ENeuronSkeletonEnum::VE_Map: // Map to configured bone map
	{
		// Map Quat.X/Y/Z with configured Bonemap				
		float Map[3] = { Quat.X, Quat.Y, Quat.Z };

		Quat.X = Map[Controller->Bonemap[BoneIndex].XYZ[0]] * Controller->Bonemap[BoneIndex].Sign[0];
		Quat.Y = Map[Controller->Bonemap[BoneIndex].XYZ[1]] * Controller->Bonemap[BoneIndex].Sign[1];
		Quat.Z = Map[Controller->Bonemap[BoneIndex].XYZ[2]] * Controller->Bonemap[BoneIndex].Sign[2];
		break;
	}
	case ENeuronSkeletonEnum::VE_UE4: // Map to UE4 world coordinate system
	{
		float Y = Quat.Y;
		float Z = Quat.Z;
		Quat.Y = Z;
		Quat.Z = Y;
		break;
	}
	case ENeuronSkeletonEnum::VE_None: // Map to nothing, use BVH rotation as it is
	default:
	{
		// Nothing to do, Quaternion is already BVH
		break;
	}
	}

	// Convert to Rotator
	Rotation = Quat.Rotator();

	// Add additional rotation
	Rotation.Yaw += AdditionalRotation.Yaw;
	Rotation.Pitch += AdditionalRotation.Pitch;
	Rotation.Roll += AdditionalRotation.Roll;
	Rotation.Normalize();

	return true;
}

// Read motion as array
// Deprecated
bool UPerceptionNeuronBPLibrary::NeuronReadMotionArray(APerceptionNeuronController *Controller, TArray<FVector> &Translation, TArray<FRotator> &Rotation, TArray<FVector> AdditionalTranslation, TArray<FRotator> AdditionalRotation, int32 MaxBones, ENeuronSkeletonEnum SkeletonType)
{
	for (int32 BoneIndex = 0; BoneIndex < MaxBones; BoneIndex++)
	{
		FVector TempTranslation;
		FRotator TempRotation;
		if (false == NeuronReadMotion(Controller, TempTranslation, TempRotation, AdditionalTranslation[BoneIndex], AdditionalRotation[BoneIndex], BoneIndex, SkeletonType))
			return false;
		Translation.Add(TempTranslation);
		Rotation.Add(TempRotation);
	}

	return true;
}

// Map a bone coordinate system
// Deprecated
bool UPerceptionNeuronBPLibrary::NeuronBoneMap(APerceptionNeuronController *Controller, int32 BoneIndex, ENeuronXYZEnum X, ENeuronXYZEnum Y, ENeuronXYZEnum Z)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}
	else if (BoneIndex >= Controller->Skeleton.BoneNr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boneindex %d exceeds maximum available bones %d."), BoneIndex, Controller->Skeleton.BoneNr));
		}
		return false;
	}

	int32 XX = (int32)X;
	int32 YY = (int32)Y;
	int32 ZZ = (int32)Z;

	Controller->Bonemap[BoneIndex].XYZ[0] = XX > 2 ? XX - 3 : XX;
	Controller->Bonemap[BoneIndex].Sign[0] = XX > 2 ? -1 : 1;

	Controller->Bonemap[BoneIndex].XYZ[1] = YY > 2 ? YY - 3 : YY;
	Controller->Bonemap[BoneIndex].Sign[1] = YY > 2 ? -1 : 1;

	Controller->Bonemap[BoneIndex].XYZ[2] = ZZ > 2 ? ZZ - 3 : ZZ;
	Controller->Bonemap[BoneIndex].Sign[2] = ZZ > 2 ? -1 : 1;

	return true;
}