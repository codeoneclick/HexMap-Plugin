// Copyright serhii serhiiv 2017. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class HexMap : ModuleRules
{
    public HexMap(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

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
                "ContentBrowser",
                "CustomMeshComponent"
            });
        }
    }
}