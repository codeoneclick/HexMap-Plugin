// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileMeshesComponent.h"
#include "HexMapGrid.h"
#include "HexMapTileComponent.h"
#include "HexMapTileMeshComponent.h"

// Sets default values for this component's properties
UHexMapTileMeshesComponent::UHexMapTileMeshesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHexMapTileMeshesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHexMapTileMeshesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHexMapTileMeshesComponent::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHexMapTileMeshesComponent, HexMapTileMesh_01))
	{
		OnHexMapTileMeshesChanged();
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHexMapTileMeshesComponent, HexMapTileMesh_02))
	{
		UE_LOG(LogTemp, Warning, TEXT("HexMapTileMesh_02 property changed!"))
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHexMapTileMeshesComponent, HexMapTileMesh_03))
	{
		UE_LOG(LogTemp, Warning, TEXT("HexMapTileMesh_03 property changed!"))
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHexMapTileMeshesComponent, HexMapTileMesh_04))
	{
		UE_LOG(LogTemp, Warning, TEXT("HexMapTileMesh_04 property changed!"))
	}
	Super::PostEditChangeProperty(Event);
}

void UHexMapTileMeshesComponent::OnHexMapTileMeshesChanged()
{
	/*AHexMapGrid* HexMapGrid = static_cast<AHexMapGrid *>(GetOwner());
	TArray<USceneComponent *> TilesComponents;

	TArray<UHexMapTileComponent*> Tiles = HexMapGrid->Tiles;
	for (int TileIndex = 0; TileIndex < Tiles.Num(); ++TileIndex)
	{
		USceneComponent* Tile = Tiles[TileIndex];
		if (Tile)
		{
			Tile->GetChildrenComponents(true, TilesComponents);

			UHexMapTileMeshComponent* HexMapTileMeshComponent = nullptr;
			if (TilesComponents.FindItemByClass(&HexMapTileMeshComponent))
			{
				UE_LOG(LogTemp, Warning, TEXT("HexMapTileMeshComponent found!"))
					HexMapTileMeshComponent->SetStaticMesh(HexMapTileMesh_01);
			}
		}
	}*/
}

