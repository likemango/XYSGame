// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XYSGame : ModuleRules
{
	public XYSGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"MetasoundEngine",
			"GameplayTags",
			"GameplayTasks",
			"NetCore",
			"PhysicsCore", 
			"ModularGameplay",
			"SlateCore",
			"Niagara",
			"ModularGameplayActors",
			"CommonUI", 
			"GameSettings",
			
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayMessageRuntime",
			"IrisCore", 
			"AIModule",
			"SKGShooterFramework",
			"SKGProceduralAnim",
			"SKGShooterFrameworkActors",
			"SKGMuzzle", 
			"CommonGame",
			"Kismet",
			"UMG",
			"InputCore",
			"ApplicationCore", 
			"GameSubtitles",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
