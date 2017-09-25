// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMapGrid.h"
#include "HexMapTile.h"
#include "HexMapComponent.h"
#include "HexMapTileMeshesComponent.h"
#include "FHex.h"
#include "HexMapTileComponent.h"
#include "HexMapTileMeshComponent.h"
#include <sstream>

// Sets default values
AHexMapGrid::AHexMapGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("HexMapGrid created!"))

	Root = CreateDefaultSubobject<UHexMapComponent>(TEXT("Root"));
	RootComponent = Root;

	/*TilesContainer = CreateDefaultSubobject<UChildActorComponent>(TEXT("TilesContainer"));
	TilesContainer->SetChildActorClass(AHexMapTile::StaticClass());
	TilesContainer->CreateChildActor();

	RootComponent = TilesContainer;*/

	//HexMapTileMeshesComponent = CreateDefaultSubobject<UHexMapTileMeshesComponent>(TEXT("HexMapTileMeshes"));
	
	/*FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	static FHexMapUtility::Layout HexMapLayout = FHexMapUtility::Layout(FHexMapUtility::LayoutPointy,
																		FHexMapUtility::Point(TileWidth, TileHeight),
																		FHexMapUtility::Point(0, 0));
	static int Index = 0;
	for (int Q = -MapRadius; Q <= MapRadius; ++Q)
	{
		int R1 = std::max(-MapRadius, -Q - MapRadius);
		int R2 = std::min(MapRadius, -Q + MapRadius);

		for (int R = R1; R <= R2; R++)
		{
			FHexMapUtility::Hex HexPosition = FHexMapUtility::Hex(Q, R, -Q - R);
			FHexMapUtility::Point Position = FHexMapUtility::HexToPixel(HexMapLayout, HexPosition);
			
			std::stringstream TileStrStream;
			TileStrStream << "Tile" << Index;
			FString TileStrName = FString(TileStrStream.str().c_str());
			FName TileName = FName(*TileStrName);

			USceneComponent* Tile = CreateDefaultSubobject<USceneComponent>(TileName);
			//Tile->RegisterComponent();
			Tile->AttachToComponent(Root, AttachmentRules);
			Tile->SetRelativeLocation(FVector(Position.x, Position.y, 40.f));

			std::stringstream TileMeshComponentStrStream;
			TileMeshComponentStrStream << "TileMeshComponent" << Index;
			FString TileMeshComponentStrName = FString(TileMeshComponentStrStream.str().c_str());
			FName TileMeshComponentName = FName(*TileMeshComponentStrName);
			UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TileMeshComponentName);
			MeshComponent->RegisterComponent();
			MeshComponent->AttachToComponent(Tile, AttachmentRules);
			Tiles.Add(Tile);
			Index++;
		}
	}

	HexMapTileMeshesComponent->OnHexMapTileMeshesChanged();*/
}

void AHexMapGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(!bIsCreated)
	{
		DestroyTiles();
		CreateTiles();
		OnHexMapTileMeshesChanged();
		bIsCreated = true;
	}
}

// Called when the game starts or when spawned
void AHexMapGrid::BeginPlay()
{
	Super::BeginPlay();
	OnHexMapTileMeshesChanged();
	UE_LOG(LogTemp, Warning, TEXT("HexMapGrid added to scene!"))
}

// Called every frame
void AHexMapGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMapGrid::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapGrid, MapRadius))
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Radius changed!"));
		DestroyTiles();
		CreateTiles();
		OnHexMapTileMeshesChanged();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapGrid, HexMapTileMesh))
	{
		OnHexMapTileMeshesChanged();
	}
}

void AHexMapGrid::CreateTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Tiles executed!"));
	//Tiles.Empty();
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);

	static FHex::Layout HexMapLayout = FHex::Layout(FHex::LayoutPointy,
		FHex::Point(TileWidth, TileHeight),
		FHex::Point(0, 0));
	for (int Q = -MapRadius; Q <= MapRadius; ++Q)
	{
		int R1 = std::max(-MapRadius, -Q - MapRadius);
		int R2 = std::min(MapRadius, -Q + MapRadius);

		for (int R = R1; R <= R2; R++)
		{
			FHex::Hex HexPosition = FHex::Hex(Q, R, -Q - R);
			FHex::Point Position = FHex::HexToPixel(HexMapLayout, HexPosition);

			UHexMapTileComponent* HexMapTileComponent = NewObject<UHexMapTileComponent>(this);
			HexMapTileComponent->RegisterComponent();
			HexMapTileComponent->AttachToComponent(GetRootComponent(), AttachmentRules);
			HexMapTileComponent->SetRelativeLocation(FVector(Position.x, Position.y, 0.f));

			UHexMapTileMeshComponent* HexMapTileMeshComponent = NewObject<UHexMapTileMeshComponent>(this);
			HexMapTileMeshComponent->RegisterComponent();
			HexMapTileMeshComponent->AttachToComponent(HexMapTileComponent, AttachmentRules);
			
			HexMapTileComponent->HexMapTileMeshComponent = HexMapTileMeshComponent;
			
			Root->HexMapTilesComponents.Add(HexMapTileComponent);
			//Tiles.Add(Tile);
		}
	}
}

void AHexMapGrid::DestroyTiles()
{
	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepRelative, false);
	TArray<USceneComponent*> ComponentsInRoot;
	GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
	for (int ComponentInRootIndex = 0; ComponentInRootIndex < ComponentsInRoot.Num(); ++ComponentInRootIndex)
	{
		auto ComponentInRoot = ComponentsInRoot[ComponentInRootIndex];
		if (ComponentInRoot->IsA(UHexMapTileComponent::StaticClass()))
		{
			TArray<USceneComponent *> ComponentsInTile;
			ComponentInRoot->GetChildrenComponents(false, ComponentsInTile);
			for (int ComponentInTileIndex = 0; ComponentInTileIndex < ComponentsInTile.Num(); ++ComponentInTileIndex)
			{
				auto ComponentInTile = ComponentsInTile[ComponentInTileIndex];
				ComponentInTile->DetachFromComponent(DetachmentTransformRules);
				ComponentInTile->UnregisterComponent();
				ComponentInTile->DestroyComponent();
			}

			ComponentInRoot->DetachFromComponent(DetachmentTransformRules);
			ComponentInRoot->UnregisterComponent();
			ComponentInRoot->DestroyComponent();
			//ComponentInRoot->DestroyComponent();
			//ComponentInRoot->DestroyComponent();
		}
	}
	Root->HexMapTilesComponents.Empty();
	GetWorld()->ForceGarbageCollection(true);
}

void AHexMapGrid::OnHexMapTileMeshesChanged()
{
	TArray<USceneComponent*> ComponentsInRoot;
	GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
	for (int ComponentInRootIndex = 0; ComponentInRootIndex < ComponentsInRoot.Num(); ++ComponentInRootIndex)
	{
		auto ComponentInRoot = ComponentsInRoot[ComponentInRootIndex];
		if (ComponentInRoot->IsA(UHexMapTileComponent::StaticClass()))
		{
			TArray<USceneComponent *> ComponentsInTile;
			ComponentInRoot->GetChildrenComponents(false, ComponentsInTile);
			UHexMapTileMeshComponent* HexMapTileMeshComponent = nullptr;
			if (ComponentsInTile.FindItemByClass(&HexMapTileMeshComponent))
			{
				UE_LOG(LogTemp, Warning, TEXT("HexMapTileMeshComponent found!"))
				HexMapTileMeshComponent->SetStaticMesh(HexMapTileMesh);
			}
		}
	}
}

int32 AHexMapGrid::GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects)
{
	if (!Obj || !Obj->IsValidLowLevelFast())
	{
		return -1;
	}

	TArray<UObject*> ReferredToObjects;				//req outer, ignore archetype, recursive, ignore transient
	FReferenceFinder ObjectReferenceCollector(ReferredToObjects, Obj, false, true, true, false);
	ObjectReferenceCollector.FindReferences(Obj);

	if (OutReferredToObjects)
	{
		OutReferredToObjects->Append(ReferredToObjects);
		return OutReferredToObjects->Num();
	}
	return -1;
}

