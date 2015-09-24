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

#include "ThirdPersonNeuron.h"
#include "ThirdPersonNeuronController.h"


// Sets default values
AThirdPersonNeuronController::AThirdPersonNeuronController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AThirdPersonNeuronController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThirdPersonNeuronController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Read each tick a new motionline. Additionally motionlines are just discarded.
	if (bConnected)
	{
		uint32 BytesPending = 0;

		if (ReceiverSocket->HasPendingData(BytesPending))
		{
#define SIZEOFDATA 10000
			uint8 Data[SIZEOFDATA];
			int32 BytesRead = 0;
			bool bMotionlineFound = false;
			bool bMotionlineEndFound = false;

			if (ReceiverSocket->Recv(Data, SIZEOFDATA, BytesRead))
			{
				int32 i, j, k;
				for (i = j = k = 0; i < BytesRead - 2; i++)
				{
					if ((Data[i] == '0') && (Data[i + 1] == ' ') && ((Data[i + 2] < '0') || (Data[i + 2] > '9') || (Data[i + 2] == '-')))
					{
						bMotionlineFound = true;
						// Skip Avatarname
						for (j = i + 2; j < BytesRead; j++)
						{
							if (Data[j] == ' ')
								break;
						}
						break;
					}
				}

				if (bMotionlineFound == true)
				{
					for (k = j; k < BytesRead; k++)
					{
						if (Data[k] == '|')
						{
							bMotionlineEndFound = true;
							break;
						}
					}

					if (bMotionlineEndFound == true)
					{
						int32 f = 0;
						// We have now a motion line captured between j and k
						for (i = j; f < MAXFLOATS && i < k; f++)
						{
							if ((Data[i] == '|') || (Data[i + 1] == '|'))
								break;

							// Read ASCII float value into float array and forward to next value
							MotionLine[f] = atof((char *)&Data[i]);
							for (i++; i < k; i++)
								if (Data[i] == ' ')
									break;
						}
						FloatCount = f;
					}
				} // End motionlinefound

				// If we get more data then we can process per tick, read socket till no more data is left and ignore it
				while (ReceiverSocket->HasPendingData(BytesPending))
				{
					ReceiverSocket->Recv(Data, SIZEOFDATA, BytesRead);
					// if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data skipped: %u"), BytesRead));
				}
			} // End Recv
		} // End HasPendingData
	} // End connected
}

bool AThirdPersonNeuronController::ParseBVHFile(FString BVHFileName)
{
	// Load BVH file
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
				Skeleton[bone].Name = Words[1];
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
					Skeleton[bone].Offset[0] = FCString::Atof(*Words[1]);
					Skeleton[bone].Offset[1] = FCString::Atof(*Words[2]);
					Skeleton[bone].Offset[2] = FCString::Atof(*Words[3]);
					bToggle = false;
				}
			}
		}
		else if (Lines[i].StartsWith(TEXT("CHANNELS"), ESearchCase::IgnoreCase) == true)
		{
			TArray<FString> Words;
			if (Lines[i].ParseIntoArray(Words, TEXT(" "), false) >= 4)
			{
				Skeleton[bone].ChannelCount = FCString::Atoi(*Words[1]);
				if (Skeleton[bone].ChannelCount != 6)
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Channelcount must be 6 on bone:%u"), bone));
					}
					return false;
				}
				Skeleton[bone].XPos = Skeleton[bone].YPos = Skeleton[bone].ZPos = 0;
				Skeleton[bone].XRot = Skeleton[bone].YRot = Skeleton[bone].ZRot = 0;
				for (int32 j = 0; j < Skeleton[bone].ChannelCount; j++)
				{
					// Detect channel order
					// E.g.: Xposition Yposition Zposition Xrotation Yrotation Zrotation
					if (Words[j + 2].StartsWith(TEXT("Xposition"), ESearchCase::IgnoreCase) == true)
						Skeleton[bone].XPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Yposition"), ESearchCase::IgnoreCase) == true)
						Skeleton[bone].YPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Zposition"), ESearchCase::IgnoreCase) == true)
						Skeleton[bone].ZPos = j;
					else if (Words[j + 2].StartsWith(TEXT("Xrotation"), ESearchCase::IgnoreCase) == true)
						Skeleton[bone].XRot = j;
					else if (Words[j + 2].StartsWith(TEXT("Yrotation"), ESearchCase::IgnoreCase) == true)
						Skeleton[bone].YRot = j;
					else if (Words[j + 2].StartsWith(TEXT("Zrotation"), ESearchCase::IgnoreCase) == true)
						Skeleton[bone].ZRot = j;
				}
				// Check if we support rotation order
				if ((Skeleton[bone].XRot < Skeleton[bone].YRot) && (Skeleton[bone].YRot < Skeleton[bone].ZRot)) // e.g. 123
					Skeleton[bone].RotOrder = XYZ;
				else if ((Skeleton[bone].ZRot < Skeleton[bone].XRot) && (Skeleton[bone].XRot < Skeleton[bone].YRot)) // e.g. 312
					Skeleton[bone].RotOrder = ZXY;
				else if ((Skeleton[bone].YRot < Skeleton[bone].XRot) && (Skeleton[bone].XRot < Skeleton[bone].ZRot)) // e.g. 213
					Skeleton[bone].RotOrder = YXZ;
				else
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Rotation order not supported on bone:%u"), bone));
					}
					return false;
				}

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

// Connect to BVH server (for e.g. Axis Neuron SW)
bool AThirdPersonNeuronController::Connect(FString HostName, int32 Port)
{
	if (bConnected == true)
		return false;

	// Create TCP Socket and connect
	ISocketSubsystem* const SocketSubSystem = ISocketSubsystem::Get(); 
	if (SocketSubSystem)
	{
		ReceiverSocket = SocketSubSystem->CreateSocket(NAME_Stream, TEXT("PerceptionNeuronSocket"), false);
		if (ReceiverSocket == NULL)
			return false;
	
		auto ResolveInfo = SocketSubSystem->GetHostByName(TCHAR_TO_ANSI(*HostName));
		while (!ResolveInfo->IsComplete());

		if (ResolveInfo->GetErrorCode() == 0)
		{
			const FInternetAddr* Addr = &ResolveInfo->GetResolvedAddress();
			uint32 IP;
			Addr->GetIp(IP);
			
			TSharedRef<FInternetAddr> InetAddr = SocketSubSystem->CreateInternetAddr();
			InetAddr->SetIp(IP);
			InetAddr->SetPort(Port);

			bConnected = ReceiverSocket->Connect(*InetAddr);
		}
	}

	return bConnected;
}

// Called when the game stops
void AThirdPersonNeuronController::BeginDestroy()
{
	Super::BeginDestroy();

	Disconnect();
}

// Disconnect from BVH server
void AThirdPersonNeuronController::Disconnect(void)
{
	if (bConnected == true)
	{
		bConnected = false;
		ReceiverSocket->Close();
	}
}

