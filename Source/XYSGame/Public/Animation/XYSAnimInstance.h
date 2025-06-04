// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "XYSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UXYSAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithOnwerAbilitySystem();

protected:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
