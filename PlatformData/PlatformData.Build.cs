using UnrealBuildTool;

public class PlatformData: ModuleRules
{
    public PlatformData(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "SlateCore", "Slate", "ApplicationCore" });

        PublicIncludePathModuleNames.AddRange(new string[] { "RHI" });

        if ((Target.IsInPlatformGroup(UnrealPlatformGroup.Windows)))
        {
            // Uses DXGI to query GPU hardware
            // This is what will allow us to get GPU usage statistics at runtime
            PublicSystemLibraries.Add("DXGI.lib");
        }
    }
}
