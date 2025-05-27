// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#include "MotionCurveManager.h"
#include "MotionCore/FunctionLibraries/MotionCurveFunctionLibrary.h"

// Sets default values for this component's properties
UMotionCurveManager::UMotionCurveManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMotionCurveManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UMotionCurveManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FStructMotionCurve& MotionCurve : ActiveMotionCurves)
	{
		const float CurveRange = UMotionCurveFunctionLibrary::GetRuntimeVectorTimeRange(MotionCurve.CurveVector);

		const bool bIsAtMin = FMath::IsNearlyEqual( MotionCurve.ActiveSeconds, 0.0f, 0.001f);
		const bool bIsAtMax = FMath::IsNearlyEqual(MotionCurve.ActiveSeconds, CurveRange, 0.001f);

		// If curve is paused, do not process
		// unless it is paused, but not at either it's min or max value.
		if(MotionCurve.bIsPaused
		&& (bIsAtMin || bIsAtMax))
			continue;

		if(bIsAtMax)
		{
			if((MotionCurve.bShouldLoop && MotionCurve.bShouldReverse)
				|| (!MotionCurve.bShouldLoop && MotionCurve.bShouldReverse))
			{
				MotionCurve.ActiveSeconds = UMotionCurveFunctionLibrary::UpdateSecondsWithCurve(MotionCurve, DeltaTime, true);
				MotionCurve.bIsReversing = true;
				continue;
			}

			if(MotionCurve.bShouldLoop
				&& !MotionCurve.bShouldReverse)
			{
				MotionCurve.ActiveSeconds = 0.0f;
				MotionCurve.bIsReversing = true;
				continue;
			}
		}

		if(bIsAtMin || !MotionCurve.bIsReversing)
		{
			MotionCurve.ActiveSeconds = UMotionCurveFunctionLibrary::UpdateSecondsWithCurve(MotionCurve, DeltaTime, false);
			MotionCurve.bIsReversing = false;
			continue;
		}

		if(MotionCurve.bIsReversing)
		{
			MotionCurve.ActiveSeconds = UMotionCurveFunctionLibrary::UpdateSecondsWithCurve(MotionCurve, DeltaTime, true);
			MotionCurve.bIsReversing = true;
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("MotionCurveManager found curve that reached unaccounted conditions!"));
	}

	// Broadcast changes to array to subscribers
	CurveArrayUpdatedDelegate.Broadcast(ActiveMotionCurves);

	// Print Debug if enabled
	if(!bPrintDebug)
		return;

	FString DebugString;
	for (const FStructMotionCurve& MotionCurve : ActiveMotionCurves)
	{
		DebugString += FString::Printf(TEXT("Identifier: %s\nActiveSeconds: %f\n"), *MotionCurve.Identifier, MotionCurve.ActiveSeconds);
	}

	// GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Red, DebugString, true);
}

int UMotionCurveManager::AddCurve(const FStructMotionCurve MotionCurve)
{
	const int CurveIndex = GetCurveIndexByIdentifier(MotionCurve.Identifier);
	if(CurveIndex != -1)
	{
		ActiveMotionCurves[CurveIndex] = MotionCurve;
		return CurveIndex;
	}

	return ActiveMotionCurves.Add(MotionCurve);
}

int UMotionCurveManager::GetCurveIndexByIdentifier(const FString CurveIdentifier)
{
	for (int32 i = 0; i < ActiveMotionCurves.Num(); ++i)
	{
		if (ActiveMotionCurves[i].Identifier.Equals(CurveIdentifier))
		{
			return i;
		}
	}
	return -1;
}

void UMotionCurveManager::SetPauseCurve(const FString CurveIdentifier, const bool bNewIsPaused)
{
	const int CurveIndex = GetCurveIndexByIdentifier(CurveIdentifier);
	if(CurveIndex != -1)
	{
		ActiveMotionCurves[CurveIndex].bIsPaused = bNewIsPaused;
	}
}

void UMotionCurveManager::DeleteCurve(const FString CurveIdentifier)
{
	const int CurveIndex = GetCurveIndexByIdentifier(CurveIdentifier);
	if(CurveIndex != -1)
	{
		ActiveMotionCurves.RemoveAt(CurveIndex);
	}
}

void UMotionCurveManager::ResetCurve(FString CurveIdentifier)
{
	const int CurveIndex = GetCurveIndexByIdentifier(CurveIdentifier);
	if(CurveIndex != -1)
	{
		ActiveMotionCurves[CurveIndex].ActiveSeconds = 0.0f;
	}
}

