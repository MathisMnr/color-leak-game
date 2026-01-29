// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ColorLeak : ModuleRules
{
	public ColorLeak(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG", "MediaAssets", "AIModule", "NavigationSystem", "Niagara", "LevelSequence", "MovieScene", "CinematicCamera", "Slate", "SlateCore", "MovieSceneTracks" });
	}
}
