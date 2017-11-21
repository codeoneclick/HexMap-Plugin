
#include "HexMap.h"
#include "HMEdMode.h"
#include "HMEdModeStyle.h"

#if WITH_EDITOR

#include "Editor/EditorStyle/Public/EditorStyleSet.h"

#endif
 
void HexMapImpl::StartupModule()
{
#if WITH_EDITOR

	UE_LOG(LogTemp, Warning, TEXT("HexMap Plugin loaded!"));
	FHMEdModeStyle::Initialize();
	FEditorModeRegistry::Get().RegisterMode<FHMEdMode>(FHMEdMode::EM_HexMap, NSLOCTEXT("EditorModes", "HexMapEdMode", "HexMap"), FSlateIcon(FHMEdModeStyle::Get()->GetStyleSetName(), "LevelEditor.HexMapMode", "LevelEditor.HexMapMode.Small"), true, 400);

#endif
}
 
void HexMapImpl::ShutdownModule()
{
#if WITH_EDITOR


#endif
}
 
IMPLEMENT_MODULE(HexMapImpl, HexMap)
