// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexMapTileObjectActor.generated.h"

UCLASS()
class HEXMAP_API AHexMapTileObjectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexMapTileObjectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EditorApplyTranslation
	(
		const FVector & DeltaTranslation,
		bool bAltDown,
		bool bShiftDown,
		bool bCtrlDown
	) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnEditorMousePressed();
	void OnEditorMouseReleased();
	
};
