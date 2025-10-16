// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SplitScreenThree : ModuleRules
{
	public SplitScreenThree(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "SlateCore", "Slate" });
		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
