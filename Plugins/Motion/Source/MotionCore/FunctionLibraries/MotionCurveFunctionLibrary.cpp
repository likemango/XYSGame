// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#include "MotionCurveFunctionLibrary.h"

FVector UMotionCurveFunctionLibrary::GetRuntimeVectorCurveValue(const FRuntimeVectorCurve& InCurve, const float InTime)
{
	auto CurveX = InCurve.GetRichCurveConst(0)->Eval(InTime);
	auto CurveY = InCurve.GetRichCurveConst(1)->Eval(InTime);
	auto CurveZ = InCurve.GetRichCurveConst(2)->Eval(InTime);

	return FVector{ CurveX, CurveY, CurveZ };
}

float UMotionCurveFunctionLibrary::GetRuntimeVectorTimeRange(const FRuntimeVectorCurve& InCurve)
{
	float XMin, YMin, ZMin;
	float XMax, YMax, ZMax;
	InCurve.GetRichCurveConst(0)->GetTimeRange(XMin, XMax);
	InCurve.GetRichCurveConst(1)->GetTimeRange(YMin, YMax);
	InCurve.GetRichCurveConst(2)->GetTimeRange(ZMin, ZMax);
	return FMath::Max3(XMax, YMax, ZMax);
}

float UMotionCurveFunctionLibrary::GetRuntimeFloatCurveValue(const FRuntimeFloatCurve& InCurve, const float InTime)
{
	return InCurve.GetRichCurveConst()->Eval(InTime);
}

float UMotionCurveFunctionLibrary::GetRuntimeFloatTimeRange(const FRuntimeFloatCurve& InCurve)
{
	float MinTime, MaxTime;
	InCurve.GetRichCurveConst()->GetTimeRange(MinTime, MaxTime);
	return MaxTime;
}

float UMotionCurveFunctionLibrary::UpdateSeconds(const float ActiveSeconds, const float DeltaTickRate, const float DeltaSeconds,
                                                 const bool bReverse)
{
	return bReverse ? ActiveSeconds - (DeltaSeconds * DeltaTickRate) : ActiveSeconds + (DeltaSeconds * DeltaTickRate);
}

float UMotionCurveFunctionLibrary::UpdateSecondsWithCurve(const FStructMotionCurve& MotionCurve, const float DeltaSeconds,
                                                          const bool bReverse)
{
	const float AdjustedDelta = MotionCurve.Tickrate * DeltaSeconds;

	// Handle reversing curves
	if(bReverse)
	{
		if(const bool bIsAtMin = FMath::IsNearlyEqual( MotionCurve.ActiveSeconds, 0.0f, 0.001f); !bIsAtMin)
		{
			if(const float AdjustedSeconds = MotionCurve.ActiveSeconds - AdjustedDelta; AdjustedSeconds >= 0.0f)
			{
				return AdjustedSeconds;
			}
			return 0.0f;
		}
		
		return FMath::FInterpTo(MotionCurve.ActiveSeconds, 0.0f, DeltaSeconds, MotionCurve.FalloutSmoothness);
	}

	const float CurveRange = UMotionCurveFunctionLibrary::GetRuntimeVectorTimeRange(MotionCurve.CurveVector);
	if(FMath::IsNearlyEqual(CurveRange - MotionCurve.ActiveSeconds, 0.0f, 0.001f))
	{
		return CurveRange;
	}

	// Progressing while paused means we are supposed to smooth this value to its end
	if(MotionCurve.bIsPaused &&
		MotionCurve.FalloutSmoothness != 0.0f)
	{
		return FMath::FInterpTo(MotionCurve.ActiveSeconds, 0.0f, DeltaSeconds, MotionCurve.FalloutSmoothness);
	}

	if(const float AdjustedSeconds = MotionCurve.ActiveSeconds + AdjustedDelta; AdjustedSeconds <= CurveRange)
	{
		return AdjustedSeconds;
	}
	
	return CurveRange;
}