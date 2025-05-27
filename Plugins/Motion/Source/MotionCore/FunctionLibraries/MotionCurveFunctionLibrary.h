// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Curves/CurveVector.h>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MotionCore/Core/MotionStructs.h"
#include "MotionCurveFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class MOTIONCORE_API UMotionCurveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Evaluates the value of a Runtime Vector Curve using the given time. */
	UFUNCTION(BlueprintPure, Category = "Math")
	static FVector GetRuntimeVectorCurveValue(const FRuntimeVectorCurve& InCurve, float InTime);

	/* Evaluates the value of a Runtime Vector Curve and returns it's length. */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float GetRuntimeVectorTimeRange(const FRuntimeVectorCurve& InCurve);

	/* Evaluates the value of a Runtime Float Curve using the given time. */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float GetRuntimeFloatCurveValue(const FRuntimeFloatCurve& InCurve, float InTime);

	/* Evaluates the value of a Runtime Float Curve returns it's length. */
	UFUNCTION(BlueprintPure, Category = "Math")
	static float GetRuntimeFloatTimeRange(const FRuntimeFloatCurve& InCurve);

	/* Returns the provided ActiveSeconds, influenced by Tick Rate & Delta Seconds that have passed. */
	UFUNCTION(BlueprintPure, Category = "Motion")
	static float UpdateSeconds(float ActiveSeconds, float DeltaTickRate, float DeltaSeconds, bool bReverse);

	/* Returns the provided ActiveSeconds, influenced by the provided curve & Delta Seconds that have passed. */
	UFUNCTION(BlueprintPure, Category = "Motion")
	static float UpdateSecondsWithCurve(const FStructMotionCurve& MotionCurve, float DeltaSeconds, bool bReverse);
};
