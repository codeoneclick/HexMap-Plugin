// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR

#include "EdMode.h"

class FHexMapEdMode : public FEdMode
{
protected:

	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;

	virtual bool UsesToolkits() const override;

public:

	FHexMapEdMode();
	virtual ~FHexMapEdMode();

	virtual void Enter() override;
	virtual void Exit() override;

	class UHMEdModePropertiesSetTileSize* EdModePropertiesSetTileSize;
	class UHMEdModePropertiesAddCircle* EdModePropertiesAddCircle;
	class UHMEdModePropertiesAddRectangle* EdModePropertiesAddRectangle;
	class UHMEdModePropertiesAddTile* EdModePropertiesAddTile;
	class UHMEdModePropertiesRandomizeTiles* EdModePropertiesRandomizeTiles;
	class UHMEdModePropertiesTileBatchApplier* EdModePropertiesTileBatchApplier;

	static FEditorModeID EM_HexMap;
};

#endif
