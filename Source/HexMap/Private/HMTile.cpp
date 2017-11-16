
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMCoord.h"
#include "HMUtilities.h"
#include "HMGrid.h"
#include "HMTilePropertiesComponent.h"
#include "HMTile.h"

#if WITH_EDITOR

#include "Editor.h"
#include "EditorModeManager.h"
#include "FHexMapEdMode.h"

#endif

AHMTile::AHMTile()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/HexMap/M_LocationError.M_LocationError'"));
	DetachedMaterial = Material.Object;
}

void AHMTile::BeginPlay()
{
	Super::BeginPlay();
}

void AHMTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHMTile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PropertiesComponent = FindComponentByClass<UHMTilePropertiesComponent>();
	if (!PropertiesComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Tile properties component!"));
	}
}

#if WITH_EDITOR

void AHMTile::OnEditorTick(float DeltaTime)
{
	if (bShouldBeAttached)
	{
		bShouldBeAttached = false;

		bool bError = false;
		AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld(), bError);
		if (!bError)
		{
			if (!Grid)
			{
				Grid = GetWorld()->SpawnActor<AHMGrid>();
			}
			OnSnapError(CanBeSnapped());
			OnEditorMouseReleased();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Can't attach Tile to unknown Grid!");
		}

		if (!IsAttachedTo(Grid))
		{
			static FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
			AttachToActor(Grid, AttachmentRules);
		}
	}
}

#endif

void AHMTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!bConstructed)
	{
		AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
		if (Grid)
		{
			Grid->OnTileAdded(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find Grid!"));
		}
		bConstructed = true;
		bShouldBeAttached = true;
		bEdit = true;
	}
}

void AHMTile::Destroyed()
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
	if (Grid)
	{
		Grid->OnTileRemoved(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Grid!"));
	}
	Super::Destroyed();
}

void AHMTile::SnapToGrid()
{
	FVector CurrentLocation = GetActorLocation();
	FHMCoord HexCoord = FHMUtilities::ToHex(GetWorld(), CurrentLocation);
	FVector SnapLocation = FHMUtilities::ToSnapLocation(GetWorld(), CurrentLocation);
	SetActorLocation(SnapLocation);
	UUID = FHMTileUUID::Init(HexCoord);
}

#if WITH_EDITOR

void AHMTile::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);
	FName PropertyName = (Event.Property != nullptr) ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AHMTile, bAttached))
	{
		Mark();
	}
}

void AHMTile::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	if (GLevelEditorModeTools().GetActiveMode(FHexMapEdMode::EM_HexMap))
	{
		OnSnapError(CanBeSnapped());
		Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
		bEdit = true;
	}
	else
	{
		GEditor->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "You can move Tile only in HexMap Mode!");
	}
}

void AHMTile::OnEditorMousePressed()
{

}

void AHMTile::OnEditorMouseReleased()
{
	if (bEdit)
	{
		bEdit = false;
		if (bAttached)
		{
			AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
			FHMTileUUID OldUUID = UUID;
			SnapToGrid();
			Grid->OnTileLocationUpdated(this, OldUUID);
		}
		else
		{
			AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());
			FHMTileUUID OldUUID = UUID;
			UUID = FHMTileUUID::Undefined();
			Grid->OnTileLocationUpdated(this, OldUUID, true);
		}
	}
}

#endif

bool AHMTile::CanBeSnapped()
{
	AHMGrid* Grid = FHMUtilities::GetGrid(GetWorld());

	FVector CurrentLocation = GetActorLocation();
	FHMCoord HexCoord = FHMUtilities::ToHex(GetWorld(), CurrentLocation);
	AHMTile** Tile = Grid->TilesToLocationsLinkages.Find(HexCoord.ToVec());
	bool bCanBeSnapped = !(Tile != nullptr && (*Tile) != this);

	if (!bCanBeSnapped)
	{
		USelection* Selection = GEditor->GetSelectedActors();
		for (int32 i = 0; i < Selection->Num(); ++i)
		{
			if (Selection->GetSelectedObject(i) == (*Tile))
			{
				bCanBeSnapped = true;
				break;
			}
		}
	}
	return bCanBeSnapped;
}

void AHMTile::OnSnapError(bool bAttached_)
{
	if (bAttached != bAttached_)
	{
		bAttached = bAttached_;
		Mark();
	}
}

void AHMTile::Mark()
{
	if (!bAttached)
	{
		TArray<UActorComponent*> MeshesComponents = GetComponentsByClass(UMeshComponent::StaticClass());
		for (UActorComponent* ActorComponent : MeshesComponents)
		{
			UMeshComponent* MeshComponent = Cast<UMeshComponent>(ActorComponent);
			AttachedMaterials.Add(MeshComponent->GetName(), MeshComponent->GetMaterial(0));
			MeshComponent->SetMaterial(0, DetachedMaterial);
		}
	}
	else
	{
		TArray<UActorComponent*> MeshesComponents = GetComponentsByClass(UMeshComponent::StaticClass());
		for (UActorComponent* ActorComponent : MeshesComponents)
		{
			UMeshComponent* MeshComponent = Cast<UMeshComponent>(ActorComponent);
			UMaterialInterface** Material = AttachedMaterials.Find(MeshComponent->GetName());
			if (Material)
			{
				MeshComponent->SetMaterial(0, (*Material));
			}
		}
		AttachedMaterials.Empty();
	}
}
