// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapGrid.generated.h"

UCLASS()
class HEXMAP_API AHexMapGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexMapGrid();

protected:

	bool bIsCreated = false;

	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

	void CreateTiles();
	void DestroyTiles();

	void OnHexMapTileMeshesChanged();
	int32 GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects = nullptr);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleDefaultsOnly)
	//class UChildActorComponent* TilesContainer;

	//UPROPERTY(EditAnywhere)
	//class UHexMapTileMeshesComponent* HexMapTileMeshesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHexMapComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 32, NoSpinbox))
	int MapRadius = 3;

	UPROPERTY(EditAnywhere)
	int TileWidth = 32;

	UPROPERTY(EditAnywhere)
	int TileHeight = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* HexMapTileMesh;
};
