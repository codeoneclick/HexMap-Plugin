// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMapGrid.h"
#include "HexMapTile.h"


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
}

// Called when the game starts or when spawned
void AHexMapGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHexMapGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

