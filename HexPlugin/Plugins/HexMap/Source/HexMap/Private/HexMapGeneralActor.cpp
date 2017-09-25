// Fill out your copyright notice in the Description page of Project Settings.

#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexMapGeneralActor.h"
#include "HexMapChunkActor.h"
#include "HexMapTileComponent.h"

// Sets default values
AHexMapGeneralActor::AHexMapGeneralActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HexMapGeneralRoot"));;
}

// Called when the game starts or when spawned
void AHexMapGeneralActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHexMapGeneralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMapGeneralActor::OnHexMapChunkActorChangedLocation()
{
	HexMapChunkTilePositions.Empty();
	for (int HexMapChunkActorIndex = 0; HexMapChunkActorIndex < HexMapChunkActors.Num(); ++HexMapChunkActorIndex)
	{
		AHexMapChunkActor* HexMapChunkActor = HexMapChunkActors[HexMapChunkActorIndex];
		TArray<USceneComponent*> ComponentsInRoot;
		HexMapChunkActor->GetRootComponent()->GetChildrenComponents(false, ComponentsInRoot);
		for (int ComponentInRootIndex = 0; ComponentInRootIndex < ComponentsInRoot.Num(); ++ComponentInRootIndex)
		{
			auto ComponentInRoot = ComponentsInRoot[ComponentInRootIndex];
			if (ComponentInRoot->IsA(UHexMapTileComponent::StaticClass()))
			{
				FVector Position = ComponentInRoot->GetComponentLocation();
				HexMapChunkTilePositions.Add(Position);
			}
		}
	}
}

void AHexMapGeneralActor::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	AHexMapGeneralActor::OnHexMapChunkActorChangedLocation();
}

void AHexMapGeneralActor::EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyRotation(DeltaRotation, bAltDown, bShiftDown, bCtrlDown);
	AHexMapGeneralActor::OnHexMapChunkActorChangedLocation();
}
