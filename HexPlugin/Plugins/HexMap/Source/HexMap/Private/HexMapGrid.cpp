// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMapGrid.h"
#include "HexMapTile.h"
#include "HexMapTileMeshesComponent.h"
#include "FHexMapUtility.h"
#include <sstream>

// Sets default values
AHexMapGrid::AHexMapGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("HexMapGrid created!"))

	TilesContainer = CreateDefaultSubobject<UChildActorComponent>(TEXT("TilesContainer"));
	TilesContainer->SetChildActorClass(AHexMapTile::StaticClass());
	TilesContainer->CreateChildActor();

	RootComponent = TilesContainer;

	Tile_01 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Tile_01"));
	Tile_01->SetChildActorClass(AHexMapTile::StaticClass());
	Tile_01->CreateChildActor();

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);

	UStaticMeshComponent* MeshComponent_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_01"));
	MeshComponent_01->AttachToComponent(Tile_01, AttachmentRules);

	Tile_02 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Tile_02"));
	Tile_02->SetChildActorClass(AHexMapTile::StaticClass());
	Tile_02->CreateChildActor();

	UStaticMeshComponent* MeshComponent_02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_02"));
	MeshComponent_02->AttachToComponent(Tile_02, AttachmentRules);

	HexMapTileMeshesComponent = CreateDefaultSubobject<UHexMapTileMeshesComponent>(TEXT("HexMapTileMeshes"));

	int TileWidth = 32;
	int TileHeight = 32;

	FHexMapUtility::Layout HexMapLayout = FHexMapUtility::Layout(FHexMapUtility::LayoutPointy,
		FHexMapUtility::Point(TileWidth, TileHeight),
		FHexMapUtility::Point(0, 0));

	int Index = 0;
	int HexMapRadius = 10;
	for (int Q = -HexMapRadius; Q <= HexMapRadius; ++Q)
	{
		int R1 = std::max(-HexMapRadius, -Q - HexMapRadius);
		int R2 = std::min(HexMapRadius, -Q + HexMapRadius);

		for (int R = R1; R <= R2; R++)
		{
			FHexMapUtility::Hex HexPosition = FHexMapUtility::Hex(Q, R, -Q - R);
			FHexMapUtility::Point Position = FHexMapUtility::HexToPixel(HexMapLayout, HexPosition);

			std::stringstream TileStrStream;
			TileStrStream << "Tile" << Index;
			FString TileStrName = FString(TileStrStream.str().c_str());
			FName TileName = FName(*TileStrName);
			UChildActorComponent* Tile = CreateDefaultSubobject<UChildActorComponent>(TileName);
			Tile->SetChildActorClass(AHexMapTile::StaticClass());
			Tile->CreateChildActor();
			Tile->SetRelativeLocation(FVector(Position.x, Position.y, 40.f));

			std::stringstream TileMeshComponentStrStream;
			TileMeshComponentStrStream << "TileMeshComponent" << Index;
			FString TileMeshComponentStrName = FString(TileMeshComponentStrStream.str().c_str());
			FName TileMeshComponentName = FName(*TileMeshComponentStrName);
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TileMeshComponentName);
			MeshComponent->AttachToComponent(Tile, AttachmentRules);
			Tiles.Add(Tile);
			Index++;
		}
	}
}

// Called when the game starts or when spawned
void AHexMapGrid::BeginPlay()
{
	Super::BeginPlay();
	HexMapTileMeshesComponent->OnHexMapTileMeshesChanged();
	UE_LOG(LogTemp, Warning, TEXT("HexMapGrid added to scene!"))
}

// Called every frame
void AHexMapGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

