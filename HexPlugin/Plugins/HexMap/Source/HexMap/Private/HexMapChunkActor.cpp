// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "FHex.h"
#include "HexMapChunkActor.h"
#include "HexMapComponent.h"
#include "HexMapTileComponent.h"
#include "HexMapTileMeshComponent.h"
#include "HexMapTile.h"
#include "HexMapGeneralActor.h"

AHexMapChunkActor::AHexMapChunkActor()
{
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("HexMapChunk created!"))

	HexMapComponent = CreateDefaultSubobject<UHexMapComponent>(TEXT("HexMapChunkRoot"));
	RootComponent = HexMapComponent;
}

void AHexMapChunkActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHexMapChunkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMapChunkActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!bIsCreated)
	{
		AHexMapChunkActor::DestroyTiles();
		AHexMapChunkActor::CreateTiles();
		AHexMapChunkActor::OnHexMapTileMeshChanged();
		bIsCreated = true;
	}
}

void AHexMapChunkActor::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != NULL) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapChunkActor, MapRadius))
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Radius changed!"));
		AHexMapChunkActor::DestroyTiles();
		AHexMapChunkActor::CreateTiles();
		AHexMapChunkActor::OnHexMapTileMeshChanged();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapChunkActor, HexMapTileMesh))
	{
		AHexMapChunkActor::OnHexMapTileMeshChanged();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapChunkActor, TileWidth) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(AHexMapChunkActor, TileHeight))
	{
		AHexMapChunkActor::OnHexMapTileSizeChanged();
	}
}

void AHexMapChunkActor::CreateTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Tiles executed!"));
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);

	FHex::Layout HexMapLayout = FHex::Layout(FHex::LayoutPointy,
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

			HexMapComponent->HexMapTilesComponents.Add(HexMapTileComponent);
		}
	}
	AHexMapChunkActor::OnHexMapChunkActorChangedLocation();
}

void AHexMapChunkActor::DestroyTiles()
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
		}
	}
	HexMapComponent->HexMapTilesComponents.Empty();
	GetWorld()->ForceGarbageCollection(true);
}

void AHexMapChunkActor::OnHexMapTileMeshChanged()
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

void AHexMapChunkActor::OnHexMapTileSizeChanged()
{
	FHex::Layout HexMapLayout = FHex::Layout(FHex::LayoutPointy,
		FHex::Point(TileWidth, TileHeight),
		FHex::Point(0, 0));
	int HexMapTileIndex = 0;
	for (int Q = -MapRadius; Q <= MapRadius; ++Q)
	{
		int R1 = std::max(-MapRadius, -Q - MapRadius);
		int R2 = std::min(MapRadius, -Q + MapRadius);

		for (int R = R1; R <= R2; R++)
		{
			FHex::Hex HexPosition = FHex::Hex(Q, R, -Q - R);
			FHex::Point Position = FHex::HexToPixel(HexMapLayout, HexPosition);
			UHexMapTileComponent* HexMapTileComponent = HexMapComponent->HexMapTilesComponents[HexMapTileIndex];
			HexMapTileComponent->SetRelativeLocation(FVector(Position.x, Position.y, 0.f));
			HexMapTileIndex++;
		}
	}
	AHexMapChunkActor::OnHexMapChunkActorChangedLocation();
}

void AHexMapChunkActor::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	AHexMapChunkActor::OnHexMapChunkActorChangedLocation();
}

void AHexMapChunkActor::EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyRotation(DeltaRotation, bAltDown, bShiftDown, bCtrlDown);
	AHexMapChunkActor::OnHexMapChunkActorChangedLocation();
}

void AHexMapChunkActor::OnHexMapChunkActorChangedLocation()
{
	for (TActorIterator<AHexMapGeneralActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AHexMapGeneralActor actor found!"));
		AHexMapGeneralActor* HexMapGeneralActor = *ActorItr;
		HexMapGeneralActor->OnHexMapChunkActorChangedLocation();
	}
}



