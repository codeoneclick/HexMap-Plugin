// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexCoord.h"
#include "HexMapChunk.generated.h"

UCLASS()
class HEXMAP_API AHexMapChunk : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexMapChunk();

private:

	static int32 GUID;
	bool bIsConstructed = false;
	void GenerateTiles(bool bIsReconstruct);
	
protected:

	static const int32 NumSubdivisions;

	void CreateTileLocationComponent(const FHexCoord& HexLocation, const FVector2D& Location2D);
	void UpdateTileLocationComponent(const FHexCoord& HexLocation, const FVector2D& Location2D);
	
	void AddHexTileGeometry(const FVector2D& Location2D, float Size, TArray<struct FFreeformMeshTriangle>& Triangles);

	virtual void GenerateTilesMetadata(bool bIsReconstruct);
	virtual void GenerateTilesGeometry(bool bIsReconstruct);

#if WITH_EDITOR

	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	virtual void EditorApplyRotation(const FRotator & DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

#endif

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void OnEditorMousePressed();
	void OnEditorMouseReleased();
	void Snap();

	void OnLocationChanged();
	void OnSizeChanged();
	void OnTileSizeChanged(bool bIsReconstruct);

	UPROPERTY(VisibleAnywhere)
	TMap<FVector2D, class UHexMapTileLocationComponent*> TileLocationComponentsToLocationsLinkages;
	TMap<FIntVector, class UHexMapTileLocationComponent*> TileLocationComponentsToHexCoordsLinkages;

	UPROPERTY(VisibleAnywhere)
	int32 UUID = std::numeric_limits<int32>::max();

	UPROPERTY()
	class UFreeformMeshComponent* GeometryComponent;
};
