#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "FHexMapEdMode.h"
#include "FHexMapEdStyle.h"

#if WITH_EDITOR

#include "Editor/EditorStyle/Public/EditorStyleSet.h"

#endif
 
void HexMapImpl::StartupModule()
{
#if WITH_EDITOR

	UE_LOG(LogTemp, Warning, TEXT("HexMap Plugin loaded!"));
	FHexMapEdStyle::Initialize();
	FEditorModeRegistry::Get().RegisterMode<FHexMapEdMode>(
			FHexMapEdMode::EM_HexMap,
			NSLOCTEXT("EditorModes", "HexMapEdMode", "HexMap"),
			FSlateIcon(FHexMapEdStyle::Get()->GetStyleSetName(), "LevelEditor.HexMapMode", "LevelEditor.HexMapMode.Small"),
			true, 400);

#endif
}
 
void HexMapImpl::ShutdownModule()
{
#if WITH_EDITOR

	FEditorModeRegistry::Get().UnregisterMode(FHexMapEdMode::EM_HexMap);

#endif
}
 
IMPLEMENT_MODULE(HexMapImpl, HexMap)
