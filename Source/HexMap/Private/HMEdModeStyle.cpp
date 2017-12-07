// Copyright serhii serhiiv 2017. All rights reserved.

#include "HMEdModeStyle.h"

#if WITH_EDITOR

#include "SlateStyle.h"
#include "IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define ICON_TAB_MODE( RelativePath, ... ) FSlateImageBrush( FHMEdModeStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )

FString FHMEdModeStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("HexMap"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FHMEdModeStyle::StyleSet = NULL;
TSharedPtr< class ISlateStyle > FHMEdModeStyle::Get() { return StyleSet; }

void FHMEdModeStyle::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("Initializing HexMapEdMode Style"));

	const FVector2D Icon40x40(40.0f, 40.0f);

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable<FSlateStyleSet>(new FSlateStyleSet("HexMapEdStyle"));

	{
		StyleSet->Set("LevelEditor.HexMapMode", new ICON_TAB_MODE("icon_Hex_Map_Mode_Selected_40px", Icon40x40));
		StyleSet->Set("LevelEditor.HexMapMode.Small", new ICON_TAB_MODE("icon_Hex_Map_Mode_Selected_40px", Icon40x40));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	UE_LOG(LogTemp, Log, TEXT("Finished Initializing HexMapEd Style"));
};

#undef ICON_TAB_MODE

void FHMEdModeStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

#endif
