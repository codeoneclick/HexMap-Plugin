// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexMapTileMeshesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHexMapTileMeshesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHexMapTileMeshesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* HexMapTileMesh_01;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* HexMapTileMesh_02;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* HexMapTileMesh_03;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* HexMapTileMesh_04;
};
