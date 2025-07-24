// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSCameraMode.h"
#include "XYSCameraMode_FirstPerson.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class XYSGAME_API UXYSCameraMode_FirstPerson : public UXYSCameraMode
{
	GENERATED_BODY()

public:
	UXYSCameraMode_FirstPerson();

	static FName NAME_XYSFirstPersonCameraAttachSocket;

protected:
	virtual void OnActivation() override;
	virtual FVector GetPivotLocation() const override;
	virtual void UpdateView(float DeltaTime) override;
	
	virtual void DrawDebug(UCanvas* Canvas) const override;
	
private:
	void UpdateMotionOffset(float DeltaTime);

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocket = NAME_None;
	UPROPERTY(EditAnywhere)
	float CrouchOffsetBlendMultiplier = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	bool bEnableMotionOffset = false;
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "bEnableMotionOffset == true", EditConditionHides))
	float OffsetInterpSpeed = 4.f;

	// FPMesh motion offset
	FVector TargetMotionLocation = FVector::ZeroVector;
	FVector LastMotionLocation = FVector::ZeroVector;
	FVector CurrentMotionOffset = FVector::ZeroVector;
	FVector MotionVector = FVector::ZeroVector;
};
