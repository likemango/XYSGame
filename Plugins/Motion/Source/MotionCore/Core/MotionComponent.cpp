// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#include "MotionCore/Core/MotionComponent.h"

void UMotionComponent::BeginPlay()
{
	OwningCharacter = Cast<ACharacter>(GetOwner());
	ensureAlways(OwningCharacter);

	OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovement<UMotionCharacterMovementComponent>();
	ensureAlways(OwningCharacterMovementComponent);

	OwningCapsuleComponent = OwningCharacter->GetComponentByClass<UCapsuleComponent>();
	ensureAlways(OwningCapsuleComponent);

	Super::BeginPlay();
}

// Sets default values for this component's properties
UMotionComponent::UMotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}