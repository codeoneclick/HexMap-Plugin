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
            "RenderCore"
        });

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "InputCore",
                "SlateCore",
                "Slate",
                "Projects",
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