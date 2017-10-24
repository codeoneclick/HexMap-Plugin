// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapGrid.h"
#include "HexMapChunk.h"
#include "HexMapTile.h"
#include "HexMapTileLocationComponent.h"
#include "HexCoord.h"

AHexMapGrid::AHexMapGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HexMapGridComponent"));
	Layout = FHexLayout::Init(FHexLayout::Flat, FVector2D(TileSize, TileSize), FVector2D(TileSize * .5f, TileSize * .5f));
}

void AHexMapGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AHexMapGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMapGrid::OnChunkAdded(AHexMapChunk* Chunk)
{
	if (!Chunk->IsAttachedTo(this))
	{
		static FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
		Chunk->AttachToActor(this, AttachmentRules);
	}
	Chunks.AddUnique(Chunk);
	OnChunkLocationChanged();
}

void AHexMapGrid::OnChunkRemoved(AHexMapChunk* Chunk)
{
	Chunks.Remove(Chunk);
	Chunks.RemoveAll([](const AHexMapChunk* Chunk) {
		return Chunk == nullptr;
	});
	OnChunkLocationChanged();
}

#if WITH_EDITOR

void AHexMapGrid::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapGrid, TileSize))
	{
		OnTileSizeChanged();
	}
}

void AHexMapGrid::SetTileSize(float TileSize_)
{
	TileSize = TileSize_;
	OnTileSizeChanged();
}

void AHexMapGrid::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	OnChunkLocationChanged();
}

void AHexMapGrid::EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyRotation(DeltaRotation, bAltDown, bShiftDown, bCtrlDown);
	OnChunkLocationChanged();
}

#endif

void AHexMapGrid::OnTileSizeChanged()
{
	Layout = FHexLayout::Init(FHexLayout::Flat, FVector2D(TileSize, TileSize), FVector2D(TileSize * .5f, TileSize * .5f));
	for (AHexMapChunk* Chunk : Chunks)
	{
		Chunk->OnTileSizeChanged(false);
	}
}

void AHexMapGrid::OnChunkLocationChanged()
{
	TilesLocations.Empty();
	ChunksToLocationsLinkages.Empty();
	for (AHexMapChunk* Chunk : Chunks)
	{
		TArray<USceneComponent*> ComponentsInRoot;
		Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
		for (USceneComponent* ComponentInRoot : ComponentsInRoot)
		{
			if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()))
			{
				UHexMapTileLocationComponent* TileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
				FVector Location = ComponentInRoot->GetComponentLocation();
				TilesLocations.Add(Location);
				ChunksToLocationsLinkages.Add(FVector2D(Location.X, Location.Y), Chunk);
				UpdateTileNeighbours(TileLocationComponent);
			}
		}
	}
}

void AHexMapGrid::UpdateTileNeighbours(class UHexMapTileLocationComponent* TileLocationComponent)
{
	FVector CurrentTileLocation = TileLocationComponent->GetComponentLocation();
	TileLocationComponent->Neighbours.Empty();
	for (AHexMapChunk* Chunk : Chunks)
	{
		TArray<USceneComponent*> ComponentsInRoot;
		Chunk->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
		for (USceneComponent* ComponentInRoot : ComponentsInRoot)
		{
			if (ComponentInRoot->IsA(UHexMapTileLocationComponent::StaticClass()) && ComponentInRoot != TileLocationComponent)
			{
				UHexMapTileLocationComponent* NeighbourTileLocationComponent = Cast<UHexMapTileLocationComponent>(ComponentInRoot);
				FVector NeighbourTileLocation = NeighbourTileLocationComponent->GetComponentLocation();
				float Distance = CurrentTileLocation.Distance(CurrentTileLocation, NeighbourTileLocation);
				if (Distance < (static_cast<float>(TileSize * 2)) * NeigboursTileDistanceTrashhold)
				{
					TileLocationComponent->Neighbours.Add(Cast<UHexMapTileLocationComponent>(ComponentInRoot));
				}
			}
		}
	}
}

