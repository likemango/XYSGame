// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MotionCore/Character/MotionCharacterMovementComponent.h"
#include "MotionComponentFunctionLibrary.generated.h"

UCLASS()
class MOTIONCORE_API UMotionComponentFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Evaluates the value of a Runtime Vector Curve using the given time. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		static bool OverlapBlockingTestByChannel(UMotionCharacterMovementComponent* MovementComponent, const FVector &Pos, const float CollisionShapeHeight);

	/* Returns the class name of the provided object. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Motion")
	static FString GetIdentifier(const UObject* Object);
};
