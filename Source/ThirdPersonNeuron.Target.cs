// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ThirdPersonNeuronTarget : TargetRules
{
	public ThirdPersonNeuronTarget(TargetInfo Target): base (Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.Add("ThirdPersonNeuron");
	}
}
