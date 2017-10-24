// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR

#include "EdMode.h"

class FHexMapEdMode : public FEdMode
{
protected:

	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;

	virtual bool UsesToolkits() const override;

public:

	FHexMapEdMode();
	virtual ~FHexMapEdMode();

	virtual void Enter() override;
	virtual void Exit() override;

	class UHexMapEdModeSetTileSizeProperties* EdModeSetTileSizeProperties;
	class UHexMapEdModeCreateCircleChunkProperties* EdModeCreateCircleChunkProperties;
	class UHexMapEdModeCreateRectangleChunkProperties* EdModeCreateRectangleChunkProperties;
	class UHexMapEdModeFillSelectedChunksProperties* EdModeFillSelectedChunksProperties;
	class UHexMapEdModeAddTileProperties* EdModeAddTileProperties;
	class UHexMapEdModeRandomizeTilesProperties* EdModeRandomizeTilesProperties;

	static FEditorModeID EM_HexMap;
};

#endif
