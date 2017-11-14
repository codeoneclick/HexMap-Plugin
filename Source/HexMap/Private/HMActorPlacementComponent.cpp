
#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HMActorPlacementComponent.h"
#include "HMMeshComponent.h"
#include "HMGrid.h"

UHMActorPlacementComponent::UHMActorPlacementComponent()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Material_ASSET(TEXT("Material'/HexMap/M_HMNavigationDebug.M_HMNavigationDebug'"));
	Material = Material_ASSET.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_ASSET(TEXT("StaticMesh'/HexMap/SM_HexTile.SM_HexTile'"));
	Mesh = StaticMesh_ASSET.Object;

	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMRootComponent"));
	RootComponent->SetupAttachment(this);

	RootComponent->SetStaticMesh(Mesh);
	RootComponent->SetMaterial(0, Material);
	RootComponent->SetVisibility(false);
	RootComponent->SetHiddenInGame(true);

	PrimaryComponentTick.bCanEverTick = true;
}

void UHMActorPlacementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHMActorPlacementComponent::OnChildAttached(USceneComponent* ChildComponent)
{
	Super::OnChildAttached(ChildComponent);
	UHMActorPlacementComponent* PlacementComponent = Cast<UHMActorPlacementComponent>(ChildComponent);
	if (PlacementComponent)
	{
		AttachedPlacements.Add(PlacementComponent);
		UMaterialInstanceDynamic* MutableMaterial = UMaterialInstanceDynamic::Create(PlacementComponent->Material, this);
		MutableMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Green);
		PlacementComponent->RootComponent->SetMaterial(0, MutableMaterial);
		PlacementComponent->RootComponent->SetVisibility(true);
	}

	Layout = FHMLayout::Init(FHMLayout::Flat, FVector2D(TileSize, TileSize), FVector2D(TileSize * .5f, TileSize * .5f));
	int32 PlacementIdx = 0;

	if (PlacementMode == EPlacementModeEnum::PM_CIRCLE)
	{
		for (int32 Q = -Radius; Q <= Radius; ++Q)
		{
			int32 R1 = std::max(-Radius, -Q - Radius);
			int32 R2 = std::min(Radius, -Q + Radius);
			PlacementComponent = nullptr;
			
			for (int32 R = R1; R <= R2; R++)
			{
				FHMCoord HexCoord = FHMCoord::Init(Q, R, -Q - R);
				FVector2D Location2D = FHMCoord::ToLocation(Layout, HexCoord);
				if (PlacementIdx < AttachedPlacements.Num())
				{
					PlacementComponent = AttachedPlacements[PlacementIdx++];
				}
				if (PlacementComponent)
				{
					FVector CurrentPlacementLocation = PlacementComponent->RelativeLocation;
					CurrentPlacementLocation.X = Location2D.X;
					CurrentPlacementLocation.Y = Location2D.Y;
					PlacementComponent->SetRelativeLocation(CurrentPlacementLocation);
				}
			}
		}
	}
	else if (PlacementMode == EPlacementModeEnum::PM_RECTANGLE)
	{
		for (int32 i = 0; i < SizeX; ++i)
		{
			for (int32 j = 0; j < SizeY; ++j)
			{
				FHMCoord HexCoord = FHMCoord::QOffsetToCube(FHMCoord::EHMDirection::ODD, FVector2D(i, j));
				FVector2D Location2D = FHMCoord::ToLocation(Layout, HexCoord);
				PlacementComponent = nullptr;
				if (PlacementIdx < AttachedPlacements.Num())
				{
					PlacementComponent = AttachedPlacements[PlacementIdx++];
				}
				if (PlacementComponent)
				{
					FVector CurrentPlacementLocation = PlacementComponent->RelativeLocation;
					CurrentPlacementLocation.X = Location2D.X;
					CurrentPlacementLocation.Y = Location2D.Y;
					PlacementComponent->SetRelativeLocation(CurrentPlacementLocation);
				}
			}
		}
	}
}

void UHMActorPlacementComponent::OnChildDetached(USceneComponent* ChildComponent)
{
	Super::OnChildDetached(ChildComponent);
	UHMActorPlacementComponent* PlacementComponent = Cast<UHMActorPlacementComponent>(ChildComponent);
	if (PlacementComponent)
	{
		AttachedPlacements.Remove(PlacementComponent);
	}
	CalculatePlacementsLocations();
}

void UHMActorPlacementComponent::CalculatePlacementsLocations()
{

}

void UHMActorPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

