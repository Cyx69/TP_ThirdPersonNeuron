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
// This header defines the classes that are provided by the Perception Neuron (TM) Blueprint Function Library.
// 

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "NeuronBPFunctionLibrary.generated.h"


UENUM(BlueprintType)
enum class ENeuronSkeletonEnum : uint8
{
	VE_None 		UMETA(DisplayName = "None"),		// Map to nothing, use translation and rotation as it is
	VE_Neuron 		UMETA(DisplayName = "Neuron"),		// Map to Neuron BVH skeleton
	VE_TPP_Hero		UMETA(DisplayName = "TPP_Hero"),	// Map to Hero_TPP, Old blue Unreal default skeleton with T-Pose
	VE_Mannequin	UMETA(DisplayName = "Mannequin")	// Map to Mannequin, New grey Unreal default skeleton with A-Pose
};

UENUM(BlueprintType)
enum class ENeuronMotionLineFormatEnum : uint8
{
	VE_Standard		UMETA(DisplayName = "Standard"),	// BVH Standard format (only the float numbers and a CR or LF at line ending)
	VE_Neuron 		UMETA(DisplayName = "Neuron")		// Axis Neuron BVH live format (with character name at beginning and || at line ending)
};

/**
 * 
 */
UCLASS()
class THIRDPERSONNEURON_API UNeuronBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Init Neuron", Keywords = "Init and read BVH reference skeleton"))
		static bool NeuronInit(AThirdPersonNeuronController *Controller, const FString BVHFileName = FString(TEXT("axis.bvh")));
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Connect Neuron", Keywords = "Connect to Axis Neuron"))
		static bool NeuronConnect(AThirdPersonNeuronController *Controller, const FString HostName = FString(TEXT("127.0.0.1")), int32 Port = 7001, bool bReference = false, bool bDisplacement = true, ENeuronMotionLineFormatEnum MotionLineFormat = ENeuronMotionLineFormatEnum::VE_Neuron);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Disconnect Neuron", Keywords = "Disconnect from Axis Neuron"))
		static bool NeuronDisconnect(AThirdPersonNeuronController *Controller);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Play BVH File", Keywords = "Play a BVH file"))
		static bool NeuronPlay(AThirdPersonNeuronController *Controller, FString BVHFileName = FString(TEXT("test.bvh")), bool bEndless = true, bool bReference = false, bool bDisplacement = true, ENeuronMotionLineFormatEnum MotionLineFormat = ENeuronMotionLineFormatEnum::VE_Standard);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Pause playing BVH File", Keywords = "Play a BVH file"))
		static bool NeuronPause(AThirdPersonNeuronController *Controller, bool bPause = false);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Read Neuron Motion", Keywords = "Read motion data from Axis Neuron"))
		static bool NeuronReadMotion(AThirdPersonNeuronController *Controller, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, int32 BoneIndex = 0, ENeuronSkeletonEnum skeletontype = ENeuronSkeletonEnum::VE_Neuron);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Get Local Bone Rotation", Keywords = "Get local bone rotation from mesh"))
		static bool NeuronGetLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex = 0);
	UFUNCTION(BlueprintPure, Category = "Perception Neuron", meta = (DisplayName = "Negate Rotation", CompactNodeTitle = "Neg", Keywords = "Negate Yaw, Pitch and Roll in rotation vector"))
		static FRotator NeuronNegateRotation(FRotator Rotation);
	
};



