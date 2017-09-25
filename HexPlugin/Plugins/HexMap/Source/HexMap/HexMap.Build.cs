using UnrealBuildTool;
using System.IO;
 
public class HexMap : ModuleRules
{
    public HexMap(TargetInfo Target)
    {
        PrivateIncludePaths.AddRange(new string[] 
        {
            "HexMap/Private"
        });

        PublicIncludePaths.AddRange(new string[] 
        {
            "HexMap/Public"
        });
 
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "EditorStyle",
            "UnrealEd"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "InputCore",
            "SlateCore",
            "RenderCore",
            "PropertyEditor",
            "WorkspaceMenuStructure",
            "LevelEditor",
            "EditorStyle",
            "ContentBrowser"
        });
    }
}