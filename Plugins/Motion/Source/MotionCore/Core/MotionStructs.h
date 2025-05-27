// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MotionEnums.h"
#include "Curves/CurveVector.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "MetasoundSource.h"
#include "Chaos/ChaosEngineInterface.h" 
#include "MotionStructs.generated.h"

/**
 * 
 */

/**
 * FStructMotionCurve
 *
 * Structure describing the content of a Motion Curve.
 */
USTRUCT(BlueprintType)
struct FStructMotionCurve
{
public:
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion | Curve")
	FString Identifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion | Curve")
	FRuntimeVectorCurve CurveVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	bool bShouldLoop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	bool bShouldReverse = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	float Tickrate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	float Multiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	float FalloutSmoothness = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	float ActiveSeconds = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	bool bIsReversing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Curve")
	bool bIsPaused = false;
	
	bool operator!=(std::nullptr_t Null) const;
};

/*
 * Structure describing the content of a Movement Sound.
 * */
USTRUCT(BlueprintType)
struct FStructMovementSound : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Movement Sound")
	EMovementSoundTypes Type = EMovementSoundTypes::Walk_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Movement Sound")
	UMetaSoundSource* MetaSoundSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Movement Sound")
	TEnumAsByte<EPhysicalSurface> Surface = SurfaceType1;
};

UCLASS()
class MOTIONCORE_API UMotionStructs : public UObject
{
	GENERATED_BODY()
};
