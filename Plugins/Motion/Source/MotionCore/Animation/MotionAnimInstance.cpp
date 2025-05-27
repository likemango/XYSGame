// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.


#include "MotionAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "MotionCore/Character/MotionCharacter.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UMotionAnimInstance::UMotionAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMotionAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult UMotionAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UMotionAnimInstance::NativeInitializeAnimation()
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

void UMotionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMotionCharacter* Character = Cast<AMotionCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}
	
	const FCharacterGroundInfo& GroundInfo = Character->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

