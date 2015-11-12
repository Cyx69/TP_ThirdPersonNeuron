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
#include "PerceptionNeuronController.h"


// Sets default values
APerceptionNeuronController::APerceptionNeuronController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APerceptionNeuronController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerceptionNeuronController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Read each tick a new motionline. Additionally motionlines are just discarded.
	if ((bConnected == true) || (bPlay == true))
	{
		uint32 BytesPending = 0;

		if (bConnected == true)
			if (ReceiverSocket->HasPendingData(BytesPending) != true)
				BytesPending = 0;

		if ((BytesPending) || (bPlay == true))
		{
#define SIZEOFDATA 10000
			uint8 Data[SIZEOFDATA];
			int32 BytesRead = 0;
			bool bMotionLineBeginFound = false;
			bool bMotionLineEndFound = false;

			if (bConnected == true) // Network connection?
			{
				if (ReceiverSocket->Recv(Data, SIZEOFDATA, BytesRead) != true)
					BytesRead = 0;
			}
			else if (bPlay == true)	// Player connected ?
			{				
				DeltaTimeAdded += DeltaTime;
				if (FrameTime < DeltaTimeAdded) // Synchronize to FrameTime
				{
					DeltaTimeAdded -= FrameTime;
					while (DeltaTimeAdded > FrameTime) // We are too slow => Skip frames
					{						
						DeltaTimeAdded -= FrameTime;						
						MotionLinePointer++;
						if ((MotionLinePointer >= (MotionLineOffset + Frames)) && (bEndless == true))
							MotionLinePointer = MotionLineOffset;				
					}

					if (MotionLinePointer < (MotionLineOffset + Frames))
					{
						if (SIZEOFDATA >= PlayerMotionLines[MotionLinePointer].Len() + 1) // Sanity check
						{
							// Convert string based motion line back to an array of chars to pump the line through the parser (Little bit ugly...)
							memcpy(Data, TCHAR_TO_ANSI(*PlayerMotionLines[MotionLinePointer]), PlayerMotionLines[MotionLinePointer].Len());
							Data[PlayerMotionLines[MotionLinePointer].Len()] = '\n';
							BytesRead = PlayerMotionLines[MotionLinePointer].Len() + 1;
						}
						MotionLinePointer++; // Increment line for next time;
						if ((MotionLinePointer >= (MotionLineOffset + Frames)) && (bEndless == true))
							MotionLinePointer = MotionLineOffset;						
					}					
				}
			}

			if (BytesRead)
			{
				int32 i, j, k;

				if (MotionLineFormat == Neuron)
				{
					// Scan for the beginning of a motion line and synchronize to Avatarname
					// Neuron BVH output format:
					// 0 Avatarname 0.00 0.00 ... 0.00 ||
					for (i = j = k = 0; i < BytesRead - 2; i++)
					{
						if ((Data[i] == '0') && (Data[i + 1] == ' ') && ((Data[i + 2] < '0') || (Data[i + 2] > '9')) && (Data[i + 2] != '-'))
						{
							bMotionLineBeginFound = true;
							// Skip Avatarname
							for (j = i + 2; j < BytesRead; j++)
							{
								if (Data[j] == ' ')
									break;
							}
							break;
						}
					}
					if (bMotionLineBeginFound == true)
					{
						// Scan for the end of the motion line
						for (k = j; k < BytesRead; k++)
						{
							if ((Data[k] == '|') || (Data[k] == '\n') || (Data[k] == '\r'))
							{
								bMotionLineEndFound = true;
								break;
							}
						}
					}
				}
				else
				{
					// Just synchronize to first float and check if floatcount matches bonecount else try next line
					// Standard BVH format:
					// 0.00 0.00 ... 0.00
					i = j = k = 0;
					for (; i < BytesRead - 1;)
					{
						for (; i < BytesRead; i++)
						{
							if ((Data[i] >= '0') && (Data[i] <= '9') || (Data[i] == '-') || (Data[i] == '.'))
							{
								j = i;
								bMotionLineBeginFound = true;
								break;
							}
						}
						if (bMotionLineBeginFound == true)
						{
							int32 t = 0;
							bMotionLineEndFound = false;
							// Scan for the end of the motion line and count empty spaces between floats
							for (i = j; i < BytesRead; i++)
							{
								if (i < (BytesRead - 1))
									if (((Data[i] == ' ') || (Data[i] == '\t')) && (((Data[i + 1] >= '0') && (Data[i + 1] <= '9')) || (Data[i + 1] == '-') || (Data[i + 1] == '.')))
										t++;
								if ((Data[i] == '\n') || (Data[i] == '\r'))
								{
									bMotionLineEndFound = true;
									break;
								}
							}
							if (bMotionLineEndFound)
							{
								int32 FloatsPerBone = 6;	 // 3 for x,y,z translation and 3 for x,y,z rotation
								int32 FloatsPerReference = 6; // 3 for x,y,z translation and 3 for x,y,z rotation

								if (bDisplacement == false)
									FloatsPerBone = 3;	// If there is no displacement (translation) info we have only 3 floats for rotation left
								if (bReference == false)
									FloatsPerReference = 0;
								// Check if empty spaces matches bone count
								if (t == ((BoneNr * FloatsPerBone) + FloatsPerReference - 1))
								{
									k = i;
									break;
								}
							}
						}
					} 
				}

				if (bMotionLineEndFound == true)
				{
					// We have now a valid motion line captured between j and k
												
					// Forward to first value
					for (i = j; i < k; i++)
						if ((Data[i] >= '0') && (Data[i] <= '9') || (Data[i] == '-') || (Data[i] == '.'))
							break;

					int32 m = 0;
					// Get all float values from motion line
					for (int32 f = 0; f < MAXFLOATS && i < k; f++)
					{													
						// Read ASCII float value into float array
						if (f >= FloatSkip)
						{
							MotionLine[m] = atof((char *)&Data[i]);
							m++;
						}	
						// Forward to end of value we read
						for (; i < k; i++)
							if (((Data[i] < '0') || (Data[i] > '9')) && (Data[i] != '-') && (Data[i] != '.'))
								break;
							
						// Forward to next value
						for (; i < k; i++)
						{
							// Sanitycheck for line ending
							if ((Data[i] == '|') || (Data[i] == '\n') || (Data[i] == '\r'))
							{							
								i = k; // Stop scanning
								break;
							}
							if ((Data[i] >= '0') && (Data[i] <= '9') || (Data[i] == '-') || (Data[i] == '.'))
								break;
						}
					}
					FloatCount = m;
				} // End bMotionLineEndFound

				if (bConnected)
				{
					// If we get more data then we can process per tick, read socket till no more data is left and ignore it
					while (ReceiverSocket->HasPendingData(BytesPending))
					{
						ReceiverSocket->Recv(Data, SIZEOFDATA, BytesRead);
						// if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data skipped: %u"), BytesRead));
					}
				}
			} // End Recv
		} // End HasPendingData
	} // End bConnected


}

bool APerceptionNeuronController::ParseBVHReferenceFile(FString BVHFileName)
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
				// Calculate rotation order
				if ((Skeleton[bone].XRot < Skeleton[bone].YRot) && (Skeleton[bone].YRot < Skeleton[bone].ZRot)) // e.g. 123
					Skeleton[bone].RotOrder = XYZ;
				else if ((Skeleton[bone].XRot < Skeleton[bone].ZRot) && (Skeleton[bone].ZRot < Skeleton[bone].YRot)) // e.g. 132
					Skeleton[bone].RotOrder = XZY;
				else if ((Skeleton[bone].YRot < Skeleton[bone].XRot) && (Skeleton[bone].XRot < Skeleton[bone].ZRot)) // e.g. 213
					Skeleton[bone].RotOrder = YXZ;
				else if ((Skeleton[bone].YRot < Skeleton[bone].ZRot) && (Skeleton[bone].ZRot < Skeleton[bone].XRot)) // e.g. 231
					Skeleton[bone].RotOrder = YZX;
				else if ((Skeleton[bone].ZRot < Skeleton[bone].XRot) && (Skeleton[bone].XRot < Skeleton[bone].YRot)) // e.g. 312
					Skeleton[bone].RotOrder = ZXY;
				else 
					Skeleton[bone].RotOrder = ZYX;

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
bool APerceptionNeuronController::Connect(FString HostName, int32 Port)
{
	if (bConnected == true)
		return false;

	// Set up reference bone handling
	// Template does not support transition for the reference bone yet
	// Should be no problem, because normally reference bone does not change values.
	if (bReference == true)
	{
		// Skip reference bone x,y,z translation and x,y,z rotation 
		// regardless if displacement is active or not (Axis Neuron sends always 6 floats)
		FloatSkip = 6; 
	}

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
void APerceptionNeuronController::BeginDestroy()
{
	Super::BeginDestroy();

	Disconnect();
}

// Disconnect from BVH server
void APerceptionNeuronController::Disconnect(void)
{
	if (bConnected == true)
	{
		bConnected = false;
		ReceiverSocket->Close();
	}
}

// BVH Player
bool APerceptionNeuronController::Play(FString BVHFileName, bool bEndless)
{
	// Set to default
	bPlayerInitialized = false;
	bPlay = false;
	PlayerMotionLines.Empty();

	// Load BVH file	
	if (FFileHelper::LoadANSITextFileToStrings(*(FPaths::GameDir() + FString("Content/") + BVHFileName), NULL, PlayerMotionLines) != true)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not load BVH file:%s"), *(FPaths::GameDir() + FString("Content/") + BVHFileName)));
		}
		return false;
	}

	// Parse BVH file, get frame time and beginning of motion line part
	for (int32 i = 0; i < PlayerMotionLines.Num(); i++)
	{
		PlayerMotionLines[i].Trim();

		if (PlayerMotionLines[i].StartsWith(TEXT("Frames:"), ESearchCase::IgnoreCase) == true)
		{
			TArray<FString> Words;
			if (PlayerMotionLines[i].ParseIntoArray(Words, TEXT(" "), false) >= 2)
			{
				Frames = FCString::Atoi(*Words[1]);
			}
		}
		else if (PlayerMotionLines[i].StartsWith(TEXT("Frame Time:"), ESearchCase::IgnoreCase) == true)
		{
			TArray<FString> Words;
			if (PlayerMotionLines[i].ParseIntoArray(Words, TEXT(" "), false) >= 2)
			{
				FrameTime = FCString::Atof(*Words[2]);
				if (FrameTime == 0)
					FrameTime = 0.010f;  // Default to a common value
			}
			if ((i + 1) <= PlayerMotionLines.Num())
			{			
				MotionLineOffset = i + 1;
				MotionLinePointer = MotionLineOffset;
				bPlayerInitialized = true;
				bPlay = true;
				APerceptionNeuronController::bEndless = bEndless;
				if (Frames > PlayerMotionLines.Num() - (i + 1))
					Frames = PlayerMotionLines.Num() - (i + 1);
				DeltaTimeAdded = FrameTime; // Play motion immediately with next frame
			}
		}
	}
	return bPlayerInitialized;
}

bool APerceptionNeuronController::Pause(bool bPause)
{
	if (bPlayerInitialized == true)
		bPlay = bPause;
	return bPlay;
}

