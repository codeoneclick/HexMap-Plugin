// Copyright serhii serhiiv 2017. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HMTileNavigationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXMAP_API UHMTileNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHMTileNavigationComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Category = "Hex Map", EditAnywhere)
	bool bIsPassable = true;

	virtual bool IsPassable(class UHMActorNavigationComponent* ActorNavigationComponent) const;
	virtual void RemoveNavigationReferences();
};
