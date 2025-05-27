// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#include "MotionCameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "MotionCore/Core/MotionCurveManager.h"
#include "MotionCore/FunctionLibraries/MotionCurveFunctionLibrary.h"
#include "Rendering/MotionVectorSimulation.h"

UMotionCameraComponent::UMotionCameraComponent(const FObjectInitializer& ObjectInitializer)
{
	CameraRotationCurvesManager = nullptr;
	CameraLocationCurvesManager = nullptr;
}

void UMotionCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningCharacter = GetOwner<ACharacter>();
	check(OwningCharacter);

	UWorld* World = GetWorld();
	check(World);
	
	OwningPlayerCameraManager = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
	check(OwningPlayerCameraManager);

	OwningPlayerCameraManager->ViewRollMin = ViewRollMin;
	OwningPlayerCameraManager->ViewRollMax = ViewRollMax;
	OwningPlayerCameraManager->ViewYawMin = ViewYawMin;
	OwningPlayerCameraManager->ViewYawMax = ViewYawMax;
	OwningPlayerCameraManager->ViewPitchMin = ViewPitchMin;
	OwningPlayerCameraManager->ViewPitchMax = ViewPitchMax;
	
	OwningCapsuleComponent = OwningCharacter->GetCapsuleComponent();
	check(OwningCapsuleComponent);

	DefaultCapsuleHalfHeight = OwningCapsuleComponent->GetScaledCapsuleHalfHeight();
	
	TArray<UMotionCurveManager*> CurveManagerComponents;
	OwningCharacter->GetComponents<UMotionCurveManager>(CurveManagerComponents);
	
	for (UMotionCurveManager* CurveManagerComponent : CurveManagerComponents)
	{
		if (CurveManagerComponent->GetName() == TEXT("CameraRotationCurvesManager"))
		{
			CameraRotationCurvesManager = CurveManagerComponent;
		}
		else if (CurveManagerComponent->GetName() == TEXT("CameraLocationCurvesManager"))
		{
			CameraLocationCurvesManager = CurveManagerComponent;
		}
	}

	ensure(CameraRotationCurvesManager);
	ensure(CameraLocationCurvesManager);

	CameraRotationCurvesManager->CurveArrayUpdatedDelegate.AddDynamic(this, &UMotionCameraComponent::OnRotationCurveArrayUpdated);
	CameraLocationCurvesManager->CurveArrayUpdatedDelegate.AddDynamic(this, &UMotionCameraComponent::OnLocationCurveArrayUpdated);
}

void UMotionCameraComponent::OnRotationCurveArrayUpdated(TArray<FStructMotionCurve> UpdatedCurveArray)
{
	CameraRotatorFromCurves = FRotator{0.0, 0.0, 0.0};
	for(FStructMotionCurve MotionCurve : UpdatedCurveArray)
	{
		const FVector CurveValue = UMotionCurveFunctionLibrary::GetRuntimeVectorCurveValue(MotionCurve.CurveVector, MotionCurve.ActiveSeconds);
		
		FQuat FQuatFinalValue = FQuat(CameraRotatorFromCurves);

		// We will follow blueprint guidelines for treating Roll, Pitch & Yaw to X, Y, Z here.
		// Unreal C++ calls for Pitch, Yaw, Roll, but I think its stupid.
		FQuat FQuatCurve = FQuat(FRotator{CurveValue.Y * MotionCurve.Multiplier, CurveValue.Z * MotionCurve.Multiplier, CurveValue.X * MotionCurve.Multiplier});
		CameraRotatorFromCurves = FRotator(FQuatCurve * FQuatFinalValue);
	}
}

void UMotionCameraComponent::OnLocationCurveArrayUpdated(TArray<FStructMotionCurve> UpdatedCurveArray)
{
	CameraLocationFromCurves = FVector{0.0, 0.0, 0.0};
	for(FStructMotionCurve MotionCurve : UpdatedCurveArray)
	{
		const FVector CurveValue = UMotionCurveFunctionLibrary::GetRuntimeVectorCurveValue(MotionCurve.CurveVector, MotionCurve.ActiveSeconds);
		CameraLocationFromCurves = CameraLocationFromCurves + (CurveValue * MotionCurve.Multiplier);
	}
}

void UMotionCameraComponent::AddStaticRotationOffset(const FString& Identifier, const FRotator& Offset)
{
	CameraStaticRotationOffsets.Add(Identifier, Offset);
}

void UMotionCameraComponent::AddStaticLocationOffset(const FString& Identifier, const FVector& Offset)
{
	CameraStaticLocationOffsets.Add(Identifier, Offset);
}

void UMotionCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	// If our character is not ready yet, return the default implementation as fallback, since some editor operations can cause this code to be called.
	if(OwningCharacter == nullptr
		|| OwningCharacter->GetController() == nullptr)
	{
		Super::GetCameraView(DeltaTime, DesiredView);
		return;
	} 
	
	const FRotator ActorRotation = OwningCharacter->GetController()->K2_GetActorRotation();
	const FRotator BaseCameraRotation = FRotator(ActorRotation.Pitch, ActorRotation.Yaw, ActorRotation.Roll);
	FRotator UpdatedCameraRotation = BaseCameraRotation;

	const FVector BaseCameraLocation = OwningCharacter->GetPawnViewLocation();

	const FVector CameraOffsetFromTargetCurve = UMotionCurveFunctionLibrary::GetRuntimeVectorCurveValue(CameraTargetOffset, BaseCameraRotation.Pitch);
	FVector TargetCameraLocation = BaseCameraLocation + BaseCameraRotation.RotateVector(CameraOffsetFromTargetCurve);
	
	for (const TTuple<FString, UE::Math::TVector<double>> & StaticLocationOffset: CameraStaticLocationOffsets)
	{
		TargetCameraLocation += BaseCameraRotation.RotateVector(StaticLocationOffset.Value);
	}
	TargetCameraLocation += CameraLocationFromCurves;
	
	DesiredView.Location = TargetCameraLocation;
	SetWorldLocation(TargetCameraLocation);
	
/**/
	
	for (const TTuple<FString, UE::Math::TRotator<double>> & StaticRotationOffset: CameraStaticRotationOffsets)
	{
		UpdatedCameraRotation = FRotator(FQuat(UpdatedCameraRotation) * FQuat(StaticRotationOffset.Value));
	}

	UpdatedCameraRotation = FRotator(FQuat(UpdatedCameraRotation) * FQuat(CameraRotatorFromCurves));
	DesiredView.Rotation = UpdatedCameraRotation;
	SetWorldRotation(UpdatedCameraRotation);

/**/
	
	// Implement default implementation of GetCameraView here.
	// Can't call Super here as it would override the result.
	if (IsXRHeadTrackedCamera())
	{
		HandleXRCamera(DeltaTime);
	}

	DesiredView.FOV = bUseAdditiveOffset ? (FieldOfView + AdditiveFOVOffset) : FieldOfView;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;

	if (bOverrideAspectRatioAxisConstraint)
	{
		DesiredView.AspectRatioAxisConstraint = AspectRatioAxisConstraint;
	}

	// See if the CameraActor wants to override the PostProcess settings used.
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}

	// If this camera component has a motion vector simulation transform, use that for the current view's previous transform
	DesiredView.PreviousViewTransform = FMotionVectorSimulation::Get().GetPreviousTransform(this);
}

#if WITH_EDITOR
bool UMotionCameraComponent::GetEditorPreviewInfo(float DeltaTime, FMinimalViewInfo& ViewOut)
{
	if (IsActive())
	{
		Super::GetCameraView(DeltaTime, ViewOut);
	}
	return IsActive();
}
#endif
