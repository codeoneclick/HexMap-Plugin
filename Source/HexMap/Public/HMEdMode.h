// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#if WITH_EDITOR

#include "EdMode.h"

class FHMEdMode : public FEdMode
{
protected:

	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;

	virtual bool UsesToolkits() const override;

public:

	FHMEdMode();
	virtual ~FHMEdMode() = default;

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
