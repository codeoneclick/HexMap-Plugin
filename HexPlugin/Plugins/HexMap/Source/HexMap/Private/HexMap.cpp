#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "FHexMapEdMode.h"
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
 
void HexMapImpl::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("HexMap Plugin loaded!"));
	FEditorModeRegistry::Get().RegisterMode<FHexMapEdMode>(
			FHexMapEdMode::EM_HexMap,
			NSLOCTEXT("EditorModes", "HexMapEdMode", "HexMap"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.FoliageMode", "LevelEditor.FoliageMode.Small"),
			true, 400);
}
 
void HexMapImpl::ShutdownModule()
{
	FEditorModeRegistry::Get().UnregisterMode(FHexMapEdMode::EM_HexMap);
}
 
IMPLEMENT_MODULE(HexMapImpl, HexMap)
