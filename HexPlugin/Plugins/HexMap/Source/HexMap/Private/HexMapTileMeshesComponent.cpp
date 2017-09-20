// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapTileMeshesComponent.h"
#include "HexMapGrid.h"

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

#ifdef ENABLED

void UHexMapTileMeshesComponent::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHexMapTileMeshesComponent, HexMapTileMesh_01))
	{
		UE_LOG(LogTemp, Warning, TEXT("HexMapTileMesh_01 property changed!"))
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

#endif

void UHexMapTileMeshesComponent::OnHexMapTileMeshesChanged()
{
	AHexMapGrid* HexMapGrid = static_cast<AHexMapGrid *>(GetOwner());
	TArray<USceneComponent *> TilesComponents;

	TArray<UChildActorComponent*> Tiles = HexMapGrid->Tiles;
	for (int TileIndex = 0; TileIndex < Tiles.Num(); ++TileIndex)
	{
		UChildActorComponent* Tile = Tiles[TileIndex];
		Tile->GetChildrenComponents(true, TilesComponents);

		UStaticMeshComponent* StaticMeshComponent = nullptr;
		if (TilesComponents.FindItemByClass(&StaticMeshComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("HexMapTile StaticMeshComponent found!"))
			StaticMeshComponent->SetStaticMesh(HexMapTileMesh_01);
		}
	}
}

