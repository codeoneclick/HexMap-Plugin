using UnrealBuildTool;
using System.IO;

public class HexMap : ModuleRules
{
    public HexMap(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "RenderCore",
        });

        if (UEBuildConfiguration.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "InputCore",
                "SlateCore",
                "Slate",
                "RenderCore",
                "ShaderCore",
                "RHI",
                "AIModule",
                "PropertyEditor",
                "WorkspaceMenuStructure",
                "UnrealEd",
                "LevelEditor",
                "EditorStyle",
                "ContentBrowser"
            });
        }
    }
}