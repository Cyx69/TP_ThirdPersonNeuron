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
#include "PerceptionNeuronBVHSkeleton.h"
#include "PerceptionNeuronController.generated.h"


// Supported motion line formats
enum MotionLineFormatEnum { Standard, Neuron };

UCLASS()
class APerceptionNeuronController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerceptionNeuronController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when the game stops
	virtual void BeginDestroy() override;


	//
	// TCP connection
	//
private:
	FSocket* ReceiverSocket;
public:
	// Connect to BVH server (for e.g. Axis Neuron SW) with
	bool Connect(FString HostName, int32 Port);

	// Disconnect from BVH server
	void Disconnect(void);			


	bool bConnected = false;		// Connection to BVH server established?
	bool bReference = false;		// BVH server sends a reference bone
	bool bDisplacement = true;		// BVH server sends displacement (translation) infos
	MotionLineFormatEnum MotionLineFormat = Neuron;	// Which motion line format is used by the BVH server?

#define MAXFLOATS	MAXBONES * 6	// 3 for x,y,z translation and 3 for x,y,z rotation	
	float MotionLine[MAXFLOATS];	// Array of floats with last motion line read
	int32 FloatCount = 0;			// How many floats did we read into MotionLine array
private:
	int32 FloatSkip = 0;			// How many leading floats should we skip?	


	//
	// BVH Reference Skeleton
	//
public:
	PerceptionNeuronBVHSkeleton Skeleton;

	// Map bone coordinate system (Used for VE_Map)
	typedef struct BoneMap
	{
		int32 XYZ[3];		// Map XYZ => X'Y'Z'
		int32 Sign[3];		// Map direction 1 or -1
	} BONEMAP;

	BONEMAP Bonemap[MAXBONES];


	//
	// BVH Player
	//
public:
	bool Play(FString BVHFileName, bool bEndless);
	bool Pause(bool bPause);

private:
	float FrameTime = 0;
	float DeltaTimeAdded = 0;
	int32 MotionLineOffset = 0;
	int32 Frames = 0;
	int32 MotionLinePointer = 0;
	bool bPlayerInitialized = false;
	bool bEndless = true;
	TArray<FString> PlayerMotionLines;

public:
	bool bPlay = false;

};
