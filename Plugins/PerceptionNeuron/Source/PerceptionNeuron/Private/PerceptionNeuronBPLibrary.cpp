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
#include "PerceptionNeuronController.h"


UPerceptionNeuronBPLibrary::UPerceptionNeuronBPLibrary(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{

}

// Init and read BVH reference skeleton
bool UPerceptionNeuronBPLibrary::NeuronInit(APerceptionNeuronController *Controller, FString BVHFileName)
{
	if (Controller == NULL)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	if (Controller->ParseBVHReferenceFile(BVHFileName) != true)
		return false;

	return true;
}

// Connect to Axis Neuron
bool UPerceptionNeuronBPLibrary::NeuronConnect(APerceptionNeuronController *Controller, FString HostName, int32 Port, bool bReference, bool bDisplacement, ENeuronMotionLineFormatEnum MotionLineFormat)
{
	if (Controller == NULL)
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
	if (Controller == NULL)
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
	if (Controller == NULL)
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
	if (Controller == NULL)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}

	return (Controller->Pause(bPause));
}

// Read motion data from Axis Neuron
bool UPerceptionNeuronBPLibrary::NeuronReadMotion(APerceptionNeuronController *Controller, FVector& Translation, FRotator& Rotation, FVector AddTranslation, FRotator AddRotation, int32 BoneIndex, ENeuronSkeletonEnum SkeletonType)
{
	bool bExit = false;
	int32 FloatsPerBone = 6; // 3 for x,y,z translation and 3 for x,y,z rotation

	if (Controller == NULL)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		bExit = true;
	}
	else if ((Controller->bConnected == false) && (Controller->bPlay == false))
	{
		bExit = true;
	}
	else if (BoneIndex > Controller->BoneNr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boneindex %d exceeds maximum available bones %d."), BoneIndex, Controller->BoneNr));
		}
		bExit = true;
	}

	if (Controller->bDisplacement == false)
	{
		FloatsPerBone = 3;	// If there is no displacement (translation) info we have only 3 floats for rotation left
	}

	if ((BoneIndex * FloatsPerBone) > Controller->FloatCount)
	{
		bExit = true;
	}

	if (bExit == true)
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
		float X = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton[BoneIndex].XPos] - Controller->Skeleton[BoneIndex].Offset[0];
		float Y = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton[BoneIndex].YPos] - Controller->Skeleton[BoneIndex].Offset[1];
		float Z = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton[BoneIndex].ZPos] - Controller->Skeleton[BoneIndex].Offset[2];

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
			}
		}
	}
	else
	{
		Translation.X = Translation.Y = Translation.Z = 0;
	}

	// Add additional translation
	Translation.X += AddTranslation.X;
	Translation.Y += AddTranslation.Y;
	Translation.Z += AddTranslation.Z;
	


	//
	// Rotation 
	//

	// Read rotation values and map to pitch, yaw, roll (y, z, x)
	float XR = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton[BoneIndex].XRot] * PI / 180.f;
	float YR = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton[BoneIndex].YRot] * PI / 180.f;
	float ZR = Controller->MotionLine[(BoneIndex * FloatsPerBone) + Controller->Skeleton[BoneIndex].ZRot] * PI / 180.f;

	float SX = sin(XR);
	float CX = cos(XR);
	float SY = sin(YR);
	float CY = cos(YR);
	float SZ = sin(ZR);
	float CZ = cos(ZR);

	FMatrix RotMatrix;
	switch (Controller->Skeleton[BoneIndex].RotOrder)
	{
		case XYZ:
		{
			RotMatrix.M[0][0] = CY*CZ;
			RotMatrix.M[0][1] = -CY*SZ;
			RotMatrix.M[0][2] = SY;
			RotMatrix.M[0][3] = 0;
			RotMatrix.M[1][0] = CZ*SX*SY + CX*SZ;
			RotMatrix.M[1][1] = CX*CZ - SX*SY*SZ;
			RotMatrix.M[1][2] = -CY*SX;
			RotMatrix.M[1][3] = 0;
			RotMatrix.M[2][0] = SX*SZ - CX*CZ*SY;
			RotMatrix.M[2][1] = CZ*SX + CX*SY*SZ;
			RotMatrix.M[2][2] = CX*CY;
			RotMatrix.M[2][3] = 0;
			break;
		}
		case ZXY:
		{
			RotMatrix.M[0][0] = CY*CZ - SX*SY*SZ;
			RotMatrix.M[0][1] = -CX*SZ;
			RotMatrix.M[0][2] = CZ*SY + CY*SX*SZ;
			RotMatrix.M[0][3] = 0;
			RotMatrix.M[1][0] = CZ*SX*SY + CY*SZ;
			RotMatrix.M[1][1] = CX*CZ;
			RotMatrix.M[1][2] = SY*SZ - CY*CZ*SX;
			RotMatrix.M[1][3] = 0;
			RotMatrix.M[2][0] = -CX*SY;
			RotMatrix.M[2][1] = SX;
			RotMatrix.M[2][2] = CX*CY;
			RotMatrix.M[2][3] = 0;
			break;
		}
		case YXZ:
		default:
		{
			RotMatrix.M[0][0] = CY*CZ + SX*SY*SZ;
			RotMatrix.M[0][1] = CZ*SX*SY - CY*SZ;
			RotMatrix.M[0][2] = CX*SY;
			RotMatrix.M[0][3] = 0;
			RotMatrix.M[1][0] = CX*SZ;
			RotMatrix.M[1][1] = CX*CZ;
			RotMatrix.M[1][2] = -SX;
			RotMatrix.M[1][3] = 0;
			RotMatrix.M[2][0] = CY*SX*SZ - CZ*SY;
			RotMatrix.M[2][1] = CY*CZ*SX + SY*SZ;
			RotMatrix.M[2][2] = CX*CY;
			RotMatrix.M[2][3] = 0;
			break;
		}
	}

	RotMatrix.M[3][0] = 0;
	RotMatrix.M[3][1] = 0;
	RotMatrix.M[3][2] = 0;
	RotMatrix.M[3][3] = 1;

	// Rotation Matrix => Quaternion and map to each bone coordinate systems dependend on skeleton type
	FQuat Quat = RotMatrix.ToQuat();
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
	}		
	
	// Map to Rotator
	Rotation = Quat.Rotator();

	// Add additional rotation
	Rotation.Yaw += AddRotation.Yaw;
	Rotation.Pitch += AddRotation.Pitch;
	Rotation.Roll += AddRotation.Roll;
	Rotation.Normalize();

	return true;
}

// Map a bone coordinate system
bool UPerceptionNeuronBPLibrary::NeuronBoneMap(APerceptionNeuronController *Controller, int32 BoneIndex, ENeuronXYZEnum X, ENeuronXYZEnum Y, ENeuronXYZEnum Z)
{
	if (Controller == NULL)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid.")));
		}
		return false;
	}
	else if (BoneIndex > Controller->BoneNr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boneindex %d exceeds maximum available bones %d."), BoneIndex, Controller->BoneNr));
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

// Get local bone rotation from mesh
bool UPerceptionNeuronBPLibrary::NeuronGetLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex)
{
	if (Mesh == NULL)
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

// Negate Yaw, Pitch and Roll in rotation vector
FRotator UPerceptionNeuronBPLibrary::NeuronNegateRotation(FRotator Rotation)
{
	FRotator NewRotation;
	NewRotation.Yaw = -Rotation.Yaw;
	NewRotation.Pitch = -Rotation.Pitch;
	NewRotation.Roll = -Rotation.Roll;

	return NewRotation;
}
