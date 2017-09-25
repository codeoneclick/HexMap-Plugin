// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "FHexMapEdMode.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "HexMapTileObjectActor.h"

FEditorModeID FHexMapEdMode::EM_HexMap(TEXT("EM_HexMap"));

FHexMapEdMode::FHexMapEdMode()
{
	UE_LOG(LogTemp, Warning, TEXT("HexMapEdMode enabled!"));
}

FHexMapEdMode::~FHexMapEdMode()
{
}

bool FHexMapEdMode::StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	for (TActorIterator<AHexMapTileObjectActor> ActorItr(InViewportClient->GetWorld()); ActorItr; ++ActorItr)
	{
		AHexMapTileObjectActor* HexMapTileObjectActor = *ActorItr;
		HexMapTileObjectActor->OnEditorMousePressed();
	}
	return FEdMode::StartTracking(InViewportClient, InViewport);
}


bool FHexMapEdMode::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	for (TActorIterator<AHexMapTileObjectActor> ActorItr(InViewportClient->GetWorld()); ActorItr; ++ActorItr)
	{
		AHexMapTileObjectActor* HexMapTileObjectActor = *ActorItr;
		HexMapTileObjectActor->OnEditorMouseReleased();
	}
	return FEdMode::EndTracking(InViewportClient, InViewport);
}
