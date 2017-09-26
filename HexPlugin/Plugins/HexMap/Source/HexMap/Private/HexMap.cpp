#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "FHexMapEdMode.h"
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
#include "FHexMapEdStyle.h"
 
void HexMapImpl::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("HexMap Plugin loaded!"));
	FHexMapEdStyle::Initialize();
	FEditorModeRegistry::Get().RegisterMode<FHexMapEdMode>(
			FHexMapEdMode::EM_HexMap,
			NSLOCTEXT("EditorModes", "HexMapEdMode", "HexMap"),
			FSlateIcon(FHexMapEdStyle::Get()->GetStyleSetName(), "LevelEditor.HexMapMode", "LevelEditor.HexMapMode.Small"),
			true, 400);
}
 
void HexMapImpl::ShutdownModule()
{
	FEditorModeRegistry::Get().UnregisterMode(FHexMapEdMode::EM_HexMap);
}
 
IMPLEMENT_MODULE(HexMapImpl, HexMap)
