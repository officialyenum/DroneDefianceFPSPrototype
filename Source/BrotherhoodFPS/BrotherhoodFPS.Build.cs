// Copyright Chukwuyenum Opone @officialyenum 2024

using UnrealBuildTool;

public class BrotherhoodFPS : ModuleRules
{
	public BrotherhoodFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "EnhancedInput", "UMG" });

		// Add private dependencies
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "NavigationSystem",
            "Niagara" // Niagara is fine for runtime
        });

        // Only include Cascade (editor-only) if we are building for the Editor
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("Cascade");
        }

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
