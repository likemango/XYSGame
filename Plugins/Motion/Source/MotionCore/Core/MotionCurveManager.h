// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MotionComponent.h"
#include "MotionCore/Core/MotionStructs.h"
#include "MotionCurveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurveArrayUpdatedSignature, TArray<FStructMotionCurve>, UpdatedCurveArray);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MOTIONCORE_API UMotionCurveManager : public UMotionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMotionCurveManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Motion")
		FCurveArrayUpdatedSignature CurveArrayUpdatedDelegate;

	/* Array containing all registers curves. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
		TArray<FStructMotionCurve> ActiveMotionCurves;

	/* Adds a new curve to the curve array. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		int AddCurve(FStructMotionCurve MotionCurve);

	/* Tries to find a curve by its identifier. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		int GetCurveIndexByIdentifier(FString CurveIdentifier);

	/* Sets the pause state of a curve using its identifier. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		void SetPauseCurve(FString CurveIdentifier, bool bNewIsPaused);

	/* Deletes a curve from the active curve array. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		void DeleteCurve(FString CurveIdentifier);

	/* Resets a curve. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		void ResetCurve(FString CurveIdentifier);

	/* If true, prints information about all curves on screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
		bool bPrintDebug;
};
