// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EdMode.h"

class FHexMapEdMode : public FEdMode
{
protected:

	/** FEdMode: Called when a mouse button is pressed */
	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;

	/** FEdMode: Called when a mouse button is released */
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;

public:

	/** Constructor */
	FHexMapEdMode();

	/** Destructor */
	virtual ~FHexMapEdMode();

	static FEditorModeID EM_HexMap;
};
