// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hack_N_Slash : ModuleRules
{
	public Hack_N_Slash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AnimGraphRuntime" });
	}
}
