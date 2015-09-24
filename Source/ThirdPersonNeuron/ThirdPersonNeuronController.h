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
// This header defines the Controller class that is used to connect to the Perception Neuron (TM) Axis Player.
// 

#pragma once

#include "Engine.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "GameFramework/Actor.h"
#include "ThirdPersonNeuronController.generated.h"


// Supported rotation matrices
enum ChannelOrderEnum { XYZ, ZXY, YXZ }; // 123, 312, 213

UCLASS()
class THIRDPERSONNEURON_API AThirdPersonNeuronController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThirdPersonNeuronController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when the game stops
	virtual void BeginDestroy() override;


	// TCP connection
private:
	FSocket* ReceiverSocket;
public:
	// Connect to BVH server (for e.g. Axis Neuron SW) with
	bool Connect(FString HostName, int32 Port);

	// Disconnect from BVH server
	void Disconnect(void);

	bool bConnected = false;

#define MAXBONES	100
#define MAXFLOATS	MAXBONES * 6  // 3 for x,y,z translation and 3 for x,y,z rotation
	float MotionLine[MAXFLOATS];
	int32 FloatCount = 0;


	// BVH File
	bool ParseBVHFile(FString BVHFileName);

	typedef struct Bone
	{
		FString Name;
		float Offset[3];	// x,y,z translation offset
		int32 ChannelCount; // Should be 6
		// Channel positions in motion line
		int32 XPos, YPos, ZPos;
		int32 XRot, YRot, ZRot;
		ChannelOrderEnum RotOrder;
	} BONE;

	BONE Skeleton[MAXBONES];
	int32 BoneNr = 0;	
};
