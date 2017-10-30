// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"

#if WITH_EDITOR

#include "HexMap.h"
#include "FHexMapEdMode.h"
#include "FHexMapEdToolkit.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Toolkits/ToolkitManager.h"
#include "HexMapTile.h"
#include "HexMapChunk.h"
#include "HexMapEdModeProperties.h"

FEditorModeID FHexMapEdMode::EM_HexMap(TEXT("EM_HexMap"));

FHexMapEdMode::FHexMapEdMode()
{
	EdModeSetTileSizeProperties = NewObject<UHexMapEdModeSetTileSizeProperties>(GetTransientPackage(), TEXT("HexMapEdModeSetTileSizeProperties"), RF_Transactional | RF_MarkAsRootSet);
	EdModeCreateCircleChunkProperties = NewObject<UHexMapEdModeCreateCircleChunkProperties>(GetTransientPackage(), TEXT("HexMapEdModeCreateCircleChunkProperties"), RF_Transactional | RF_MarkAsRootSet);
	EdModeCreateRectangleChunkProperties = NewObject<UHexMapEdModeCreateRectangleChunkProperties>(GetTransientPackage(), TEXT("HexMapEdModeCreateRectangleChunkProperties"), RF_Transactional | RF_MarkAsRootSet);
	EdModeFillSelectedChunksProperties = NewObject<UHexMapEdModeFillSelectedChunksProperties>(GetTransientPackage(), TEXT("HexMapEdModeFillSelectedChunksProperties"), RF_Transactional | RF_MarkAsRootSet);
	EdModeAddTileProperties = NewObject<UHexMapEdModeAddTileProperties>(GetTransientPackage(), TEXT("HexMapEdModeAddTilePropertiess"), RF_Transactional | RF_MarkAsRootSet);
	EdModeRandomizeTilesProperties = NewObject<UHexMapEdModeRandomizeTilesProperties>(GetTransientPackage(), TEXT("HexMapEdModeRandomizeTilesProperties"), RF_Transactional | RF_MarkAsRootSet);
	EdModeTileBatchApplierProperties = NewObject<UHexMapEdModeTileBatchApplierProperties>(GetTransientPackage(), TEXT("HexMapEdModeTileBatchApplierProperties"), RF_Transactional | RF_MarkAsRootSet);
}

FHexMapEdMode::~FHexMapEdMode()
{
}

void FHexMapEdMode::Enter()
{
	FEdMode::Enter();
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FHexMapEdToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FHexMapEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	FEdMode::Exit();
}

bool FHexMapEdMode::StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	for (TActorIterator<AHexMapChunk> It(InViewportClient->GetWorld()); It; ++It)
	{
		AHexMapChunk* Chunk = *It;
		Chunk->OnEditorMousePressed();
	}
	for (TActorIterator<AHexMapTile> It(InViewportClient->GetWorld()); It; ++It)
	{
		AHexMapTile* Tile = *It;
		Tile->OnEditorMousePressed();
	}
	return FEdMode::StartTracking(InViewportClient, InViewport);
}

bool FHexMapEdMode::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	for (TActorIterator<AHexMapChunk> It(InViewportClient->GetWorld()); It; ++It)
	{
		AHexMapChunk* Chunk = *It;
		Chunk->OnEditorMouseReleased();
	}
	for (TActorIterator<AHexMapTile> It(InViewportClient->GetWorld()); It; ++It)
	{
		AHexMapTile* Tile = *It;
		Tile->OnEditorMouseReleased();
	}
	return FEdMode::EndTracking(InViewportClient, InViewport);
}

bool FHexMapEdMode::UsesToolkits() const
{
	return true;
}

#endif
