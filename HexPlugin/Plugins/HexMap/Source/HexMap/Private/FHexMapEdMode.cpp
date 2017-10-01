// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"

#if WITH_EDITOR

#include "HexMap.h"
#include "FHexMapEdMode.h"
#include "FHexMapEdToolkit.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Editor/UnrealEd/Public/EditorModeManager.h"
#include "Toolkits/ToolkitManager.h"
#include "HexMapTileObjectActor.h"

FEditorModeID FHexMapEdMode::EM_HexMap(TEXT("EM_HexMap"));

FHexMapEdMode::FHexMapEdMode()
{
	UE_LOG(LogTemp, Warning, TEXT("HexMapEdMode enabled!"));
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

bool FHexMapEdMode::UsesToolkits() const
{
	return true;
}

#endif
