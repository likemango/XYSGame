// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "MotionCore/Core/MotionComponent.h"
#include "MotionCore/Core/MotionStructs.h"
#include "MotionCameraComponent.generated.h"

class UMotionCurveManager;

/**
 * 
 */
UCLASS()
class MOTIONCORE_API UMotionCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	explicit UMotionCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	
	/* Owning character of this camera component. */
	UPROPERTY(BlueprintReadOnly, Category = "Component Data")
		ACharacter* OwningCharacter;

	/* Owning main capsule component of OwningCharacter. */
	UPROPERTY(BlueprintReadOnly, Category = "Component Data")
	    UCapsuleComponent* OwningCapsuleComponent;

	/* Owning main capsule component of OwningCharacter. */
	UPROPERTY(BlueprintReadOnly, Category = "Component Data")
		APlayerCameraManager* OwningPlayerCameraManager;

	/* MotionCurveManager component of OwningCharacter that is responsible for rotations. */
	UPROPERTY(BlueprintReadOnly, Category = "Component Data")
		UMotionCurveManager* CameraRotationCurvesManager;

	/* MotionCurveManager component of OwningCharacter that is responsible for locations. */
	UPROPERTY(BlueprintReadOnly, Category = "Component Data")
		UMotionCurveManager* CameraLocationCurvesManager;

	UFUNCTION()
		void OnRotationCurveArrayUpdated(TArray<FStructMotionCurve> UpdatedCurveArray);

	UFUNCTION()
		void OnLocationCurveArrayUpdated(TArray<FStructMotionCurve> UpdatedCurveArray);
	
	/* Defines the positional offset (Time) the camera should have at a given camera angle (Value). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		FRuntimeVectorCurve CameraTargetOffset;

	/* The Minimum / Maximum Pitch (moving camera up and down) the camera can be moved at. Useful to avoid the player to look down / up too much */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float ViewPitchMin = -89.0f;

	/* The Minimum / Maximum Pitch (moving camera up and down) the camera can be moved at. Useful to avoid the player to look down / up too much */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float ViewPitchMax = 89.0f;

	/* The Minimum / Maximum Roll (rolling camera left and right) the camera can be moved at. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float ViewRollMin = -359.99f;

	/* The Minimum / Maximum Roll (rolling camera left and right) the camera can be moved at. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float ViewRollMax = 359.99f;

	/* The Minimum / Maximum Roll (rolling camera left and right) the camera can be moved at. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float ViewYawMin = -359.99f;

	/* The Minimum / Maximum Roll (rolling camera left and right) the camera can be moved at. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
		float ViewYawMax = 359.99f;

	/* Array of FStructMotionCurve containing information about all curves that modify the relative location of the camera. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Data")
		TArray<FStructMotionCurve> CameraLocationCurves;

	/* Array of FStructMotionCurve containing information about all curves that modify the relative rotation of the camera. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Data")
		TArray<FStructMotionCurve> CameraRotationCurves;

	/* Map of hard location values that are added to the final camera location. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Data")
		TMap<FString, FVector> CameraStaticLocationOffsets;

	/* Map of hard rotation values that are added to the final camera location. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Data")
		TMap<FString, FRotator> CameraStaticRotationOffsets;

	/* Camera location from all active location curves combined. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Data")
		FVector CameraLocationFromCurves;

	/* Camera rotation from all active location curves combined. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Data")
		FRotator CameraRotatorFromCurves;

	/* Adds a static rotation offset to the camera. It is not influenced by curves and will be added after curve calculations. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		void AddStaticRotationOffset(const FString& Identifier, const FRotator& Offset);

	/* Adds a static location offset to the camera. It is not influenced by curves and will be added after curve calculations. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		void AddStaticLocationOffset(const FString& Identifier, const FVector& Offset);

protected:
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	
#if WITH_EDITOR
	virtual bool GetEditorPreviewInfo(float DeltaTime, FMinimalViewInfo& ViewOut) override;
#endif

	float DefaultCapsuleHalfHeight;
	FVector InitialCameraTransform;
};

