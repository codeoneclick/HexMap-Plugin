// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"
#include "IDetailsView.h"

class FHexMapEdToolkit : public FModeToolkit
{
private:

	static TSharedRef<SWidget> MAKE_SetTileSize_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_CreateCircleChunk_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_CreateRectangleChunk_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_FillSelectedChunks_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_ClearSelectedChunks_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_AddTile_BTN(const FText& Label);
	static TSharedRef<SWidget> MAKE_RandomizeTiles_BTN(const FText& Label);

	static TSharedRef<SWidget> MAKE_SetTileSize_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_CreateCircleChunk_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_CreateRectangleChunk_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_FillChunks_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_ClearChunks_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_RandomizeTiles_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_AddTile_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_ValidateMap_SLOT(FHexMapEdToolkit* SELF);
	static TSharedRef<SWidget> MAKE_Copyright_SLOT(FHexMapEdToolkit* SELF);

	static FReply ON_SetTileSize_BTN();
	static FReply ON_CreateCircleChunk_BTN();
	static FReply ON_CreateRectangleChunk_BTN();
	static FReply ON_FillSelectedChunks_BTN();
	static FReply ON_ClearSelectedChunks_BTN();
	static FReply ON_AddTile_BTN();
	static FReply ON_RandomizeTiles_BTN();

protected:

	TSharedPtr<IDetailsView> HexMapEdModeSetTileSizePanel;
	TSharedPtr<IDetailsView> HexMapEdModeCreateCircleChunkPanel;
	TSharedPtr<IDetailsView> HexMapEdModeCreateRectangleChunkPanel;
	TSharedPtr<IDetailsView> HexMapEdModeFillSelectedChunksPanel;
	TSharedPtr<IDetailsView> HexMapEdModeAddTilePanel;
	TSharedPtr<IDetailsView> HexMapEdModeRandomizeTilesPanel;
	TSharedPtr<SWidget> ToolkitWidget;

	static void GetSelectedChunks(TArray<class AHexMapChunk*>& Chunks);

public:

	FHexMapEdToolkit();
	~FHexMapEdToolkit();

	virtual void Init(const TSharedPtr<class IToolkitHost>& ToolkitHost) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }
};

#endif