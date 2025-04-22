// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;


public class Calypso_Dev : ModuleRules
{
	public Calypso_Dev(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PlatformData", "PhysicsCore", "GameplayTags", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Дэбаг отключен.
        PublicDefinitions.Add("ENABLE_DRAW_DEBUG=0");

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
