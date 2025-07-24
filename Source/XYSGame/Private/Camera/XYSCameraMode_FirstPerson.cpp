// XiaoYao copyright.


#include "Camera/XYSCameraMode_FirstPerson.h"

#include "Character/XYSCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

FName UXYSCameraMode_FirstPerson::NAME_XYSFirstPersonCameraAttachSocket(TEXT("S_Camera"));

UXYSCameraMode_FirstPerson::UXYSCameraMode_FirstPerson()
{
	if (AttachSocket == NAME_None)
	{
		AttachSocket = NAME_XYSFirstPersonCameraAttachSocket;
	}
}

void UXYSCameraMode_FirstPerson::OnActivation()
{
	if (AXYSCharacter* Character = Cast<AXYSCharacter>(GetTargetActor()))
	{
		Character->SetCharacterMeshViewMode(ECharacterViewType::FirstPerson);
	}
}

FVector UXYSCameraMode_FirstPerson::GetPivotLocation() const
{
	if (const AXYSCharacter* TargetCharacter = Cast<AXYSCharacter>(GetTargetActor()))
	{
		return TargetCharacter->GetFPUpperMesh()->GetSocketLocation(AttachSocket);
	}
	return FVector::ZeroVector;
}

void UXYSCameraMode_FirstPerson::UpdateView(float DeltaTime)
{

	FVector MotionOffset = FVector::ZeroVector;
	if (bEnableMotionOffset)
	{
		UpdateMotionOffset(DeltaTime);
		MotionOffset = CurrentMotionOffset;
	}
	
	FVector PivotLocation = GetPivotLocation() + MotionOffset;
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void UXYSCameraMode_FirstPerson::DrawDebug(UCanvas* Canvas) const
{
	Super::DrawDebug(Canvas);

#if ENABLE_DRAW_DEBUG
	if (!MotionVector.IsNearlyZero())
	{
		if (const AXYSCharacter* TargetCharacter = Cast<AXYSCharacter>(GetTargetActor()))
		{
			const FVector TargetSocketLocation= TargetCharacter->GetTPMesh()->GetSocketLocation(AttachSocket);

			UKismetSystemLibrary::DrawDebugArrow(this, TargetSocketLocation, TargetSocketLocation + MotionVector * 10, 3.f, FColor::Red);
		}
	}
#endif
}

void UXYSCameraMode_FirstPerson::UpdateMotionOffset(float DeltaTime)
{
	if (const AXYSCharacter* TargetCharacter = Cast<AXYSCharacter>(GetTargetActor()))
	{
		LastMotionLocation = TargetMotionLocation;
		
		const FVector TargetSocketLocation= TargetCharacter->GetTPMesh()->GetSocketLocation(AttachSocket);
		TargetMotionLocation = FVector(TargetSocketLocation.X, TargetSocketLocation.Y, 0); 
	}
	MotionVector = TargetMotionLocation - LastMotionLocation;
	CurrentMotionOffset = FMath::VInterpTo(MotionVector, FVector::ZeroVector, DeltaTime, OffsetInterpSpeed);
}



