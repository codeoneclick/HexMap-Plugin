// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMapTile.h"
#include "HexMapGrid.h"
#include "HexMapChunk.h"
#include "HexMapTileLocationComponent.h"
#include "FHexMapEdMode.h"

AHexMapTile::AHexMapTile()
{
	PrimaryActorTick.bCanEverTick = true;
	GeometryComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeometryComponent"));
	RootComponent = GeometryComponent;
}

void AHexMapTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!bIsConstructed)
	{
		bIsConstructed = true;
		PreviousLocation = GetActorLocation();
		Snap();
	}
}

void AHexMapTile::Destroyed()
{
	Super::Destroyed();
}


void AHexMapTile::BeginPlay()
{
	Super::BeginPlay();
}

void AHexMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR

void AHexMapTile::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	if (GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap))
	{
		Super::PostEditChangeProperty(Event);
		FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
		if (PropertyName == FName("RelativeLocation"))
		{
			Snap(true);
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You can edit Tile properties only in HexMap Mode!");
	}
}

void AHexMapTile::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	if (GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap))
	{
		AHexMapGrid* Grid = FHex::GetGrid(GetWorld());

		FVector MoveDirection = DeltaTranslation;
		MoveDirection.Normalize();
		MoveDirection.X *= Grid->TileSize * 2.f;
		MoveDirection.Y *= Grid->TileSize * 2.f;
		FVector CurrentLocation = GetActorLocation();
		FVector PredictedLocation = CurrentLocation + MoveDirection;
		float NearestDistance = std::numeric_limits<float>::max();
		FVector NearestLocation = FHex::GetTileLocation(Grid->TilesLocations, PredictedLocation, NearestDistance);
		if (NearestDistance < Grid->TileSize * 2.f)
		{
			Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
		}
		else
		{
			SetActorLocation(NearestLocation);
		}
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You can move Tile only in HexMap Mode!");
	}
}

#endif

void AHexMapTile::Snap(bool bValidationDialog)
{
	bool bError = false;
	AHexMapGrid* Grid = FHex::GetGrid(GetWorld(), bError);
	if (!Grid)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find HexMapGrid!"));
		return;
	}
	FVector CurrentTileLocation = FHex::GetTileLocation(Grid->TilesLocations, GetActorLocation());
	AHexMapChunk** CurrentChunkIt = Grid->ChunksToLocationsLinkages.Find(FVector2D(CurrentTileLocation.X, CurrentTileLocation.Y));
	if (CurrentChunkIt)
	{
		FVector CachedTileLocation = FHex::GetTileLocation(Grid->TilesLocations, PreviousLocation);
		AHexMapChunk** CachedChunkIt = Grid->ChunksToLocationsLinkages.Find(FVector2D(CachedTileLocation.X, CachedTileLocation.Y));
		if (CachedChunkIt)
		{
			AHexMapChunk* CachedChunk = *CachedChunkIt;
			UHexMapTileLocationComponent** TileLocationComponentIt = CachedChunk->TileLocationComponentsToLocationsLinkages.Find(FVector2D(CachedTileLocation.X, CachedTileLocation.Y));
			UHexMapTileLocationComponent* TileLocationComponent = *TileLocationComponentIt;
			TileLocationComponent->LinkedTile = nullptr;
			UUID = FTileUUID::Undefined();
			static FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
			DetachFromActor(DetachmentTransformRules);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find Chunk for cached location!"));
		}

		AHexMapChunk* CurrentChunk = *CurrentChunkIt;
		UHexMapTileLocationComponent** TileLocationComponentIt = CurrentChunk->TileLocationComponentsToLocationsLinkages.Find(FVector2D(CurrentTileLocation.X, CurrentTileLocation.Y));
		if (TileLocationComponentIt)
		{
			UHexMapTileLocationComponent* TileLocationComponent = *TileLocationComponentIt;
			if (!TileLocationComponent->LinkedTile)
			{
				static FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
				AttachToActor(CurrentChunk, AttachmentTransformRules);
				TileLocationComponent->LinkedTile = this;
				UUID = TileLocationComponent->UUID;
				if (!bIsSnapYToGrid)
				{
					FVector NewLocation = FVector(CurrentTileLocation.X, CurrentTileLocation.Y, GetActorLocation().Z);
					SetActorLocation(NewLocation);
				}
				else
				{
					SetActorLocation(CurrentTileLocation);
				}
				TileLocationComponent->CachedLocationZ = GetActorLocation().Z;
				PreviousLocation = CurrentTileLocation;
			}
			else if (TileLocationComponent->LinkedTile == this)
			{
				if (!bIsSnapYToGrid)
				{
					FVector NewLocation = FVector(CurrentTileLocation.X, CurrentTileLocation.Y, GetActorLocation().Z);
					SetActorLocation(NewLocation);
				}
				else
				{
					SetActorLocation(CurrentTileLocation);
				}
				TileLocationComponent->CachedLocationZ = GetActorLocation().Z;
				PreviousLocation = CurrentTileLocation;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Can't attach more than one Tile per LocationComponent!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find TileLocationComponent for current location!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Chunk for current location!"));
	}
}

#if WITH_EDITOR

void AHexMapTile::OnEditorMousePressed()
{

}

void AHexMapTile::OnEditorMouseReleased()
{
	if (IsSelectedInEditor())
	{
		Snap(true);
	}
}

#endif


