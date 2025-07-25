// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "XYSFrontendStateComponent.generated.h"


UCLASS(Abstract, NotBlueprintable)
class XYSGAME_API UXYSFrontendStateComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UXYSFrontendStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
