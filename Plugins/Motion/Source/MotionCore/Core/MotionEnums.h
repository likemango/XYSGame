// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MotionEnums.generated.h"

UENUM(BlueprintType)
enum class ECurvePositions : uint8 {
	Curve_Pos_Minimum       UMETA(DisplayName="CurvePosMin"),
	Curve_Pos_Maximum       UMETA(DisplayName="CurvePosMax"),
	Curve_Pos_Progressing       UMETA(DisplayName="CurvePosProgressing"),
};

UENUM(BlueprintType)
enum class EMovementDirections : uint8 {
	Forward				UMETA(DisplayName="Forward"),
	ForwardSideway      UMETA(DisplayName="ForwardSideway"),
	Backward			UMETA(DisplayName="Backward"),
	BackwardSideway		UMETA(DisplayName="BackwardSideway"),
};

UENUM(BlueprintType)
enum class EMovementEvents : uint8 {
	Jumped				UMETA(DisplayName="Jumped"),
	Landed				UMETA(DisplayName="Landed"),
};

UENUM(BlueprintType)
enum class EMovementSoundTypes : uint8 {
	Walk_Left			UMETA(DisplayName="Walk_Left"),
	Walk_Right			UMETA(DisplayName="Walk_Right"),
	Sprint_Left			UMETA(DisplayName="Sprint_Left"),
	Sprint_Right		UMETA(DisplayName="Sprint_Right"),
	Crouch_Left			UMETA(DisplayName="Crouch_Left"),
	Crouch_Right		UMETA(DisplayName="Crouch_Right"),
	Jump				UMETA(DisplayName="Jump"),
	Land				UMETA(DisplayName="Land"),
};

/**
 * 
 */
UCLASS()
class MOTIONCORE_API UMotionEnums : public UObject
{
	GENERATED_BODY()
};
