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
#include "Engine.h"
#include "PerceptionNeuronBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

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

UCLASS()
class UPerceptionNeuronBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Init Neuron", Keywords = "Init and read BVH reference skeleton"))
		static bool NeuronInit(APerceptionNeuronController *Controller, const FString BVHFileName = FString(TEXT("axis.bvh")));
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Connect Neuron", Keywords = "Connect to Axis Neuron"))
		static bool NeuronConnect(APerceptionNeuronController *Controller, const FString HostName = FString(TEXT("127.0.0.1")), int32 Port = 7001, bool bReference = false, bool bDisplacement = true, ENeuronMotionLineFormatEnum MotionLineFormat = ENeuronMotionLineFormatEnum::VE_Neuron);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Disconnect Neuron", Keywords = "Disconnect from Axis Neuron"))
		static bool NeuronDisconnect(APerceptionNeuronController *Controller);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Play BVH File", Keywords = "Play a BVH file"))
		static bool NeuronPlay(APerceptionNeuronController *Controller, FString BVHFileName = FString(TEXT("test.bvh")), bool bEndless = true, bool bReference = false, bool bDisplacement = true, ENeuronMotionLineFormatEnum MotionLineFormat = ENeuronMotionLineFormatEnum::VE_Standard);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Pause playing BVH File", Keywords = "Play a BVH file"))
		static bool NeuronPause(APerceptionNeuronController *Controller, bool bPause = false);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Read Neuron Motion", Keywords = "Read motion data from Axis Neuron"))
		static bool NeuronReadMotion(APerceptionNeuronController *Controller, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, int32 BoneIndex = 0, ENeuronSkeletonEnum skeletontype = ENeuronSkeletonEnum::VE_Neuron);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Get Local Bone Rotation", Keywords = "Get local bone rotation from mesh"))
		static bool NeuronGetLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex = 0);
	UFUNCTION(BlueprintPure, Category = "Perception Neuron", meta = (DisplayName = "Negate Rotation", CompactNodeTitle = "Neg", Keywords = "Negate Yaw, Pitch and Roll in rotation vector"))
		static FRotator NeuronNegateRotation(FRotator Rotation);
};
