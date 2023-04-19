// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class C_Inventory : ModuleRules
{
	public C_Inventory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",  });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemSteam" });
    }
}
