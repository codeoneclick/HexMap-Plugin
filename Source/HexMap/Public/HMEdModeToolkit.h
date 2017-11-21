// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"
#include "IDetailsView.h"

class FHMEdModeToolkit : public FModeToolkit
{
private:

	static TSharedRef<SWidget> MAKE_SetTileSize_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_AddCircle_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_AddRectangle_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_AddTile_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_RandomizeTiles_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_TilesBatchApplier_BTN(const FText& Label);

	static TSharedRef<SWidget> MAKE_SetTileSize_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_AddCircle_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_AddRectangle_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_AddTile_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_RandomizeTiles_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_TilesBatchApplier_SLOT(FHMEdModeToolkit* SELF);
	static TSharedRef<SWidget> MAKE_Copyright_SLOT(FHMEdModeToolkit* SELF);

	static FReply ON_SetTileSize_BTN();
	static FReply ON_AddCircle_BTN();
	static FReply ON_AddRectangle_BTN();
	static FReply ON_AddTile_BTN();
	static FReply ON_RandomizeTiles_BTN();
	static FReply ON_TilesBatchApplier_BTN();

protected:

	static void GetSelectedTiles(TArray<class AHMTile*>& Tiles);

	TSharedPtr<IDetailsView> EdModePanelSetTileSize;
	TSharedPtr<IDetailsView> EdModePanelAddCircle;
	TSharedPtr<IDetailsView> EdModePanelAddRectangle;
	TSharedPtr<IDetailsView> EdModePanelAddTile;
	TSharedPtr<IDetailsView> EdModePanelRandomizeTiles;
	TSharedPtr<IDetailsView> EdModePanelTilesBatchApplier;
	TSharedPtr<SWidget> ToolkitWidget;

public:

	FHMEdModeToolkit() = default;
	~FHMEdModeToolkit() = default;

	virtual void Init(const TSharedPtr<class IToolkitHost>& ToolkitHost) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }
};

#endif