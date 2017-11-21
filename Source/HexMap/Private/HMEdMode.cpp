// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"

#if WITH_EDITOR

#include "HexMap.h"
#include "HMEdMode.h"
#include "HMEdModeToolkit.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Toolkits/ToolkitManager.h"
#include "HMTile.h"
#include "HMGrid.h"
#include "HMEdModeProperties.h"

FEditorModeID FHMEdMode::EM_HexMap(TEXT("EM_HexMap"));

FHMEdMode::FHMEdMode()
{
	EdModePropertiesSetTileSize = NewObject<UHMEdModePropertiesSetTileSize>(GetTransientPackage(), TEXT("EdModePropertiesSetTileSize"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesAddCircle = NewObject<UHMEdModePropertiesAddCircle>(GetTransientPackage(), TEXT("EdModePropertiesAddCircle"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesAddRectangle = NewObject<UHMEdModePropertiesAddRectangle>(GetTransientPackage(), TEXT("EdModePropertiesAddRectangle"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesAddTile = NewObject<UHMEdModePropertiesAddTile>(GetTransientPackage(), TEXT("EdModePropertiesAddTile"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesRandomizeTiles = NewObject<UHMEdModePropertiesRandomizeTiles>(GetTransientPackage(), TEXT("EdModePropertiesRandomizeTiles"), RF_Transactional | RF_MarkAsRootSet);
	EdModePropertiesTileBatchApplier = NewObject<UHMEdModePropertiesTileBatchApplier>(GetTransientPackage(), TEXT("EdModePropertiesTileBatchApplier"), RF_Transactional | RF_MarkAsRootSet);
}

void FHMEdMode::Enter()
{
	FEdMode::Enter();
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FHMEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FHMEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	FEdMode::Exit();
}

void FHMEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	FEdMode::Tick(ViewportClient, DeltaTime);
	for (TActorIterator<AHMTile> It(ViewportClient->GetWorld()); It; ++It)
	{
		AHMTile* Tile = *It;
		Tile->OnEditorTick(DeltaTime);
	}
	for (TActorIterator<AHMGrid> It(ViewportClient->GetWorld()); It; ++It)
	{
		AHMGrid* Grid = *It;
		Grid->OnEditorTick(DeltaTime);
	}
}

bool FHMEdMode::StartTracking(FEditorViewportClient* ViewportClient, FViewport* InViewport)
{
	for (TActorIterator<AHMTile> It(ViewportClient->GetWorld()); It; ++It)
	{
		AHMTile* Tile = *It;
		Tile->OnEditorMousePressed();
	}
	return FEdMode::StartTracking(ViewportClient, InViewport);
}

bool FHMEdMode::EndTracking(FEditorViewportClient* ViewportClient, FViewport* InViewport)
{
	for (TActorIterator<AHMTile> It(ViewportClient->GetWorld()); It; ++It)
	{
		AHMTile* Tile = *It;
		Tile->OnEditorMouseReleased();
	}
	return FEdMode::EndTracking(ViewportClient, InViewport);
}

bool FHMEdMode::UsesToolkits() const
{
	return true;
}

#endif
