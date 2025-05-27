// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#include "MotionCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "MotionCore/MotionGameplayTags.h"

void UMotionCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	DefaultMaxWalkSpeedOnInit = MaxWalkSpeed;
}

void UMotionCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If no speeds are registered, we leave it at the default.
	if(WalkSpeeds.IsEmpty())
	{
		MaxWalkSpeed = DefaultMaxWalkSpeedOnInit;
		return;
	}

	// Calculate final player speed
	float NewMaxWalkSpeed = DefaultMaxWalkSpeedOnInit;
	for(const TTuple<FString, float> & WalkSpeed : WalkSpeeds)
	{
		NewMaxWalkSpeed += WalkSpeed.Value;
	}

	// Ensure that speed does not go negative
	NewMaxWalkSpeed = FMath::Max(0.0f, NewMaxWalkSpeed);

	// Update walk speed
	MaxWalkSpeed = NewMaxWalkSpeed;
}


void UMotionCharacterMovementComponent::AddWalkSpeed(FString Identifier, float Value)
{
	WalkSpeeds.Emplace(Identifier, Value);
}

void UMotionCharacterMovementComponent::RemoveWalkSpeed(FString Identifier)
{
	WalkSpeeds.Remove(Identifier);
}

float UMotionCharacterMovementComponent::GetMaxSpeed() const
{
	if (const UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(MotionGameplayTags::GameplayTag_MovementHalted))
		{
			return 0;
		}
	}
	
	return Super::GetMaxSpeed();
}
