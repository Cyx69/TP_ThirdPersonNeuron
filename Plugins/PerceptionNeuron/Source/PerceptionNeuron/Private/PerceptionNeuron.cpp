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
// This code implements the Perception Neuron (TM) module startup and shutdown functions.
// 

#include "PerceptionNeuronPrivatePCH.h"

#define LOCTEXT_NAMESPACE "FPerceptionNeuronModule"

void FPerceptionNeuronModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	UE_LOG(LogInit, Log, TEXT("Unofficial Perception Neuron (TM) Plugin started."));
}

void FPerceptionNeuronModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	UE_LOG(LogExit, Log, TEXT("Unofficial Perception Neuron (TM) Plugin unloaded."));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPerceptionNeuronModule, PerceptionNeuron)