// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "XYSCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UXYSCameraComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Camera")
	static UXYSCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UXYSCameraComponent>() : nullptr); }


};
