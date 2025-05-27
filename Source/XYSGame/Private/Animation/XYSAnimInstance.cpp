// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/XYSAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "Character/XYSCharacter.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UXYSAnimInstance::UXYSAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UXYSAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}


#if WITH_EDITOR
EDataValidationResult UXYSAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UXYSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (const AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(Asc);
		}
	}
}

void UXYSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AXYSCharacter* Character = Cast<AXYSCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}
	
	const FCharacterGroundInfo& GroundInfo = Character->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

