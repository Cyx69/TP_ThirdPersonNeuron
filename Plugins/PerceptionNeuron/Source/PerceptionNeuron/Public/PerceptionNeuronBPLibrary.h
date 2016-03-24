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
#include "PerceptionNeuronController.h"
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


// Deprecated
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

// Bones delivered in the Perception Neuron live stream
UENUM(BlueprintType)
enum class EPerceptionNeuronBonesEnum : uint8
{
	VE_Hips				UMETA(DisplayName = "Hips"),
	VE_RightUpLeg		UMETA(DisplayName = "RightUpLeg"),
	VE_RightLeg			UMETA(DisplayName = "RightLeg"),
	VE_RightFoot		UMETA(DisplayName = "RightFoot"),
	VE_LeftUpLeg		UMETA(DisplayName = "LeftUpLeg"),
	VE_LeftLeg			UMETA(DisplayName = "LeftLeg"),
	VE_LeftFoot			UMETA(DisplayName = "LeftFoot"),
	VE_Spine			UMETA(DisplayName = "Spine"),
	VE_Spine1			UMETA(DisplayName = "Spine1"),
	VE_Spine2			UMETA(DisplayName = "Spine2"),
	VE_Spine3			UMETA(DisplayName = "Spine3"),
	VE_Neck				UMETA(DisplayName = "Neck"),
	VE_Head				UMETA(DisplayName = "Head"),
	VE_RightShoulder	UMETA(DisplayName = "RightShoulder"),
	VE_RightArm			UMETA(DisplayName = "RightArm"),
	VE_RightForeArm		UMETA(DisplayName = "RightForeArm"),
	VE_RightHand		UMETA(DisplayName = "RightHand"),
	VE_RightHandThumb1	UMETA(DisplayName = "RightHandThumb1"),
	VE_RightHandThumb2	UMETA(DisplayName = "RightHandThumb2"),
	VE_RightHandThumb3	UMETA(DisplayName = "RightHandThumb3"),
	VE_RightInHandIndex UMETA(DisplayName = "RightInHandIndex"),
	VE_RightHandIndex1	UMETA(DisplayName = "RightHandIndex1"),
	VE_RightHandIndex2	UMETA(DisplayName = "RightHandIndex2"),
	VE_RightHandIndex3	UMETA(DisplayName = "RightHandIndex3"),
	VE_RightInHandMiddle UMETA(DisplayName = "RightInHandMiddle"),
	VE_RightHandMiddle1 UMETA(DisplayName = "RightHandMiddle1"),
	VE_RightHandMiddle2 UMETA(DisplayName = "RightHandMiddle2"),
	VE_RightHandMiddle3 UMETA(DisplayName = "RightHandMiddle3"),
	VE_RightInHandRing	UMETA(DisplayName = "RightInHandRing"),
	VE_RightHandRing1	UMETA(DisplayName = "RightHandRing1"),
	VE_RightHandRing2	UMETA(DisplayName = "RightHandRing2"),
	VE_RightHandRing3	UMETA(DisplayName = "RightHandRing3"),
	VE_RightInHandPinky UMETA(DisplayName = "RightInHandPinky"),
	VE_RightHandPinky1	UMETA(DisplayName = "RightHandPinky1"),
	VE_RightHandPinky2	UMETA(DisplayName = "RightHandPinky2"),
	VE_RightHandPinky3	UMETA(DisplayName = "RightHandPinky3"),
	VE_LeftShoulder		UMETA(DisplayName = "LeftShoulder"),
	VE_LeftArm			UMETA(DisplayName = "LeftArm"),
	VE_LeftForeArm		UMETA(DisplayName = "LeftForeArm"),
	VE_LeftHand			UMETA(DisplayName = "LeftHand"),
	VE_LeftHandThumb1	UMETA(DisplayName = "LeftHandThumb1"),
	VE_LeftHandThumb2	UMETA(DisplayName = "LeftHandThumb2"),
	VE_LeftHandThumb3	UMETA(DisplayName = "LeftHandThumb3"),
	VE_LeftInHandIndex	UMETA(DisplayName = "LeftInHandIndex"),
	VE_LeftHandIndex1	UMETA(DisplayName = "LeftHandIndex1"),
	VE_LeftHandIndex2	UMETA(DisplayName = "LeftHandIndex2"),
	VE_LeftHandIndex3	UMETA(DisplayName = "LeftHandIndex3"),
	VE_LeftInHandMiddle UMETA(DisplayName = "LeftInHandMiddle"),
	VE_LeftHandMiddle1	UMETA(DisplayName = "LeftHandMiddle1"),
	VE_LeftHandMiddle2	UMETA(DisplayName = "LeftHandMiddle2"),
	VE_LeftHandMiddle3	UMETA(DisplayName = "LeftHandMiddle3"),
	VE_LeftInHandRing	UMETA(DisplayName = "LeftInHandRing"),
	VE_LeftHandRing1	UMETA(DisplayName = "LeftHandRing1"),
	VE_LeftHandRing2	UMETA(DisplayName = "LeftHandRing2"),
	VE_LeftHandRing3	UMETA(DisplayName = "LeftHandRing3"),
	VE_LeftInHandPinky	UMETA(DisplayName = "LeftInHandPinky"),
	VE_LeftHandPinky1	UMETA(DisplayName = "LeftHandPinky1"),
	VE_LeftHandPinky2	UMETA(DisplayName = "LeftHandPinky2"),
	VE_LeftHandPinky3	UMETA(DisplayName = "LeftHandPinky3")
};

USTRUCT(BlueprintType)
struct FPerceptionNeuronBoneMapStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception Neuron")
	EPerceptionNeuronBonesEnum BVHBone = EPerceptionNeuronBonesEnum::VE_Hips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception Neuron")
	FName CustomBoneName = FName(TEXT("None"));
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

	// Read motion functions
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Read Neuron Motion", ToolTip = "Read incoming motion data for all bones."))
		static bool NeuronRead(APerceptionNeuronController *Controller, USkeletalMeshComponent *Mesh, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, EPerceptionNeuronBonesEnum BVHBone = EPerceptionNeuronBonesEnum::VE_Hips, FName CustomBoneName = FName(TEXT("None")));
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Read Neuron Motion Array", ToolTip = "Read incoming motion data for all bones."))
		static bool NeuronReadArray(APerceptionNeuronController *Controller, USkeletalMeshComponent *Mesh, TArray<FVector> &Translation, TArray<FRotator> &Rotation, TArray<FVector> AdditionalTranslation, TArray<FRotator> AdditionalRotation, TArray<FPerceptionNeuronBoneMapStruct> BoneMap);

	// Additional tools
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Get Local Bone Rotation", ToolTip = "Get local bone rotation from mesh."))
		static bool NeuronGetLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex = 0);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Get Local Bone Location", Keywords = "Get local bone location from mesh."))
		static bool NeuronGetLocalBoneLocation(USkeletalMeshComponent *Mesh, FVector& Location, int32 BoneIndex = 0);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Get Refpose Local Bone Rotation", Keywords = "Get local bone rotation from reference pose."))
		static bool NeuronGetReferencePoseLocalBoneRotation(USkeletalMeshComponent *Mesh, FRotator& Rotation, int32 BoneIndex = 0);
	UFUNCTION(BlueprintPure, Category = "Perception Neuron", meta = (DisplayName = "Negate Rotation", CompactNodeTitle = "Neg", ToolTip = "Negate Yaw, Pitch and Roll in rotation vector."))
		static FRotator NeuronNegateRotation(FRotator Rotation);
	UFUNCTION(BlueprintPure, Category = "Perception Neuron", meta = (DisplayName = "Get BVH Bone Index", CompactNodeTitle = "Index", ToolTip = "Return BVH Bone Index from Enum."))
		static int32 NeuronGetBVHBoneIndex(EPerceptionNeuronBonesEnum BVHBone = EPerceptionNeuronBonesEnum::VE_Hips);


	// Deprecated (Do not use in new projects)
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Read Neuron Motion (Deprecated)", ToolTip = "Read incoming motion data for one bone (Deprecated)."))
		static bool NeuronReadMotion(APerceptionNeuronController *Controller, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, int32 BoneIndex = 0, ENeuronSkeletonEnum SkeletonType = ENeuronSkeletonEnum::VE_Neuron);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Read Neuron Motion Array (Deprecated)", ToolTip = "Read incoming motion data for all bones."))
		static bool NeuronReadMotionArray(APerceptionNeuronController *Controller, TArray<FVector>& Translation, TArray<FRotator>& Rotation, TArray<FVector> AdditionalTranslation, TArray<FRotator> AdditionalRotation, int32 MaxBones = 0, ENeuronSkeletonEnum SkeletonType = ENeuronSkeletonEnum::VE_Neuron);
	UFUNCTION(BlueprintCallable, Category = "Perception Neuron", meta = (DisplayName = "Map Bone Coordinates", ToolTip = "Map bone coordinate system manually (Deprecated)."))
		static bool NeuronBoneMap(APerceptionNeuronController *Controller, int32 BoneIndex, ENeuronXYZEnum X = ENeuronXYZEnum::VE_X, ENeuronXYZEnum Y = ENeuronXYZEnum::VE_Y, ENeuronXYZEnum Z = ENeuronXYZEnum::VE_Z);
};
