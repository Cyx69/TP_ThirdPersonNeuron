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
	VE_None 		UMETA(DisplayName = "None", ToolTip = "Get BVH coordinates. Use translation and rotation as it is."),
	VE_Neuron 		UMETA(DisplayName = "Neuron", ToolTip = "Get Neuron BVH skeleton coordinates."),
	VE_TPP_Hero		UMETA(DisplayName = "TPP_Hero", ToolTip = "Get Hero_TPP skeleton coordinates. Old blue Unreal default skeleton with T-Pose."),
	VE_Mannequin	UMETA(DisplayName = "Mannequin", ToolTip = "Get Mannequin skeleton coordinates. New grey Unreal default skeleton with A-Pose."),
	VE_Map			UMETA(DisplayName = "Map", ToolTip = "Get custom skeleton coordinates with configured bone map."),
	VE_UE4			UMETA(DisplayName = "World", ToolTip = "Get UE4 world skeleton coordinates.")
};

UENUM(BlueprintType)
enum class ENeuronMotionLineFormatEnum : uint8
{
	VE_Standard		UMETA(DisplayName = "Standard", ToolTip = "BVH Standard format (only the float numbers and a CR or LF at line ending)."),
	VE_Neuron 		UMETA(DisplayName = "Neuron", ToolTip = "Axis Neuron BVH live format (with character name at beginning and || at line ending).")
};

UENUM(BlueprintType)
enum class ENeuronXYZEnum : uint8
{
	VE_X			UMETA(DisplayName = "X"),
	VE_Y			UMETA(DisplayName = "Y"),
	VE_Z			UMETA(DisplayName = "Z"),
	VE_NX			UMETA(DisplayName = "-X"),
	VE_NY			UMETA(DisplayName = "-Y"),
	VE_NZ	 		UMETA(DisplayName = "-Z")	
};

UENUM(BlueprintType)
enum class ENeuronRotOrderEnum : uint8
{
	VE_XYZ			UMETA(DisplayName = "XYZ"),
	VE_XZY			UMETA(DisplayName = "XZY"),
	VE_YXZ			UMETA(DisplayName = "YXZ"),
	VE_YZX			UMETA(DisplayName = "YZX"),
	VE_ZXY			UMETA(DisplayName = "ZXY"),
	VE_ZYX	 		UMETA(DisplayName = "ZYX")
};

UENUM(BlueprintType)
enum class ENeuronChannelNumberEnum : uint8
{
	VE_0			UMETA(DisplayName = "0"),
	VE_1			UMETA(DisplayName = "1"),
	VE_2			UMETA(DisplayName = "2"),
	VE_3			UMETA(DisplayName = "3"),
	VE_4			UMETA(DisplayName = "4"),
	VE_5	 		UMETA(DisplayName = "5")
};


UCLASS()
class UPerceptionNeuronBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// Init functions
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Init Neuron File", ToolTip = "Init BVH reference skeleton with a BVH file."))
		static bool NeuronInitFile(APerceptionNeuronController *Controller, const FString BVHFileName = FString(TEXT("axis.bvh")));
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Init Neuron", ToolTip = "Init BVH reference skeleton manually."))
		static bool NeuronInit(APerceptionNeuronController *Controller, int32 BoneNumber = 59, ENeuronRotOrderEnum RotationOrder = ENeuronRotOrderEnum::VE_YXZ, ENeuronChannelNumberEnum XPositionChannel = ENeuronChannelNumberEnum::VE_0, ENeuronChannelNumberEnum YPositionChannel = ENeuronChannelNumberEnum::VE_1, ENeuronChannelNumberEnum ZPositionChannel = ENeuronChannelNumberEnum::VE_2, ENeuronChannelNumberEnum XRotationChannel = ENeuronChannelNumberEnum::VE_4, ENeuronChannelNumberEnum YRotationChannel = ENeuronChannelNumberEnum::VE_3, ENeuronChannelNumberEnum ZRotationChannel = ENeuronChannelNumberEnum::VE_5);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Bone Set Offset", ToolTip = "Configure BVH bone offset. This offset is removed from the incoming translation before coordinate mapping."))
		static bool NeuronBoneSetOffset(APerceptionNeuronController *Controller, int32 BoneIndex = 0, float X = 0.0f, float Y = 0.0f, float Z = 0.0f);

	// Network functions
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Connect Neuron", ToolTip = "Connect to Axis Neuron Playser."))
		static bool NeuronConnect(APerceptionNeuronController *Controller, const FString HostName = FString(TEXT("127.0.0.1")), int32 Port = 7001, bool bReference = false, bool bDisplacement = true, ENeuronMotionLineFormatEnum MotionLineFormat = ENeuronMotionLineFormatEnum::VE_Neuron);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Disconnect Neuron", ToolTip = "Disconnect from Axis Neuron Player."))
		static bool NeuronDisconnect(APerceptionNeuronController *Controller);

	// Player functions
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Play BVH File", ToolTip = "Play a local stored BVH file."))
		static bool NeuronPlay(APerceptionNeuronController *Controller, FString BVHFileName = FString(TEXT("test.bvh")), bool bEndless = true, bool bReference = false, bool bDisplacement = true, ENeuronMotionLineFormatEnum MotionLineFormat = ENeuronMotionLineFormatEnum::VE_Standard);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Pause playing BVH File", ToolTip = "Pause playing a local stored BVH file"))
		static bool NeuronPause(APerceptionNeuronController *Controller, bool bPause = false);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Rewind BVH File Player", ToolTip = "Rewind BVH file player."))
		static bool NeuronRewind(APerceptionNeuronController *Controller);

	// Read motion function
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Read Neuron Motion", ToolTip = "Read incoming motion data for one bone."))
		static bool NeuronReadMotion(APerceptionNeuronController *Controller, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, int32 BoneIndex = 0, ENeuronSkeletonEnum skeletontype = ENeuronSkeletonEnum::VE_Neuron);

	// Map bone coordinates function
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Map Bone Coordinates", ToolTip = "Map bone coordinate system manually."))
		static bool NeuronBoneMap(APerceptionNeuronController *Controller, int32 BoneIndex, ENeuronXYZEnum X = ENeuronXYZEnum::VE_X, ENeuronXYZEnum Y = ENeuronXYZEnum::VE_Y, ENeuronXYZEnum Z = ENeuronXYZEnum::VE_Z);

	// Additional tools
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Get Local Bone Rotation", ToolTip = "Get local bone rotation from mesh."))
		static bool NeuronGetLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex = 0);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Get Local Bone Location", Keywords = "Get local bone location from mesh"))
		static bool NeuronGetLocalBoneLocation(USkeletalMeshComponent *Mesh, FVector& Location, int32 BoneIndex = 0);
	UFUNCTION(BlueprintPure, Category = "Perception Neuron", meta = (DisplayName = "Negate Rotation", CompactNodeTitle = "Neg", ToolTip = "Negate Yaw, Pitch and Roll in rotation vector."))
		static FRotator NeuronNegateRotation(FRotator Rotation);
};
