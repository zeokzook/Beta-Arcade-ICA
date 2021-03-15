// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Evil_Bun_Dungeon : ModuleRules
{
	public Evil_Bun_Dungeon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
