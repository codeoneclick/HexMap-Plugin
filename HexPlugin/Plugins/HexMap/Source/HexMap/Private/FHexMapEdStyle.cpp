// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"

#if WITH_EDITOR

#include "HexMap.h"
#include "FHexMapEdStyle.h"
#include "SlateStyle.h"

#define ICON_TAB_MODE( RelativePath, ... ) FSlateImageBrush( FHexMapEdStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )

FHexMapEdStyle::FHexMapEdStyle()
{
}

FHexMapEdStyle::~FHexMapEdStyle()
{
}

FString FHexMapEdStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = FPaths::GamePluginsDir() / TEXT("HexMap/Content");
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FHexMapEdStyle::StyleSet = NULL;
TSharedPtr< class ISlateStyle > FHexMapEdStyle::Get() { return StyleSet; }

void FHexMapEdStyle::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("Initializing HexMapEd Style"));
	// Const icon sizes
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable<FSlateStyleSet>(new FSlateStyleSet("HexMapEdStyle"));

	// Style
	{
		StyleSet->Set("LevelEditor.HexMapMode", new ICON_TAB_MODE("icon_Hex_Map_Mode_Selected_40px", Icon40x40));
		StyleSet->Set("LevelEditor.HexMapMode.Small", new ICON_TAB_MODE("icon_Hex_Map_Mode_Selected_40px", Icon40x40));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	UE_LOG(LogTemp, Log, TEXT("Finished Initializing HexMapEd Style"));
};

#undef ICON_TAB_MODE

void FHexMapEdStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

#endif
