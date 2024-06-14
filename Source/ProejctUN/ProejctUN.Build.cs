// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProejctUN : ModuleRules
{
	public ProejctUN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "ProejctUN" });

        PublicDependencyModuleNames.AddRange(new string[] {
			"ProejctUN", 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"NavigationSystem", 
			"AIModule", 
			"Niagara", 
			"EnhancedInput", 
			"GameplayAbilities", 
			"GameplayTasks", 
			"GameplayTags", 
			"GameFeatures", 
			"SlateCore",
			"Slate",
			"UMG",
			"MultiplayerSessions",
			"OnlineSubsystem",
			"OnlineSubsystemSteam"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "ProejctUN" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
