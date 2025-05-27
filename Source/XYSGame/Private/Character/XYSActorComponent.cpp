// XiaoYao copyright.

#include "Character/XYSActorComponent.h"
#include "Character/XYSCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UXYSActorComponent::UXYSActorComponent(): OwningCharacter(nullptr), OwningCharacterMovementComponent(nullptr),
                                          OwningCapsuleComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UXYSActorComponent::BeginPlay()
{
	OwningCharacter = Cast<ACharacter>(GetOwner());
	ensureAlways(OwningCharacter);

	OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovement<UXYSCharacterMovementComponent>();
	ensureAlways(OwningCharacterMovementComponent);

	OwningCapsuleComponent = OwningCharacter->GetComponentByClass<UCapsuleComponent>();
	ensureAlways(OwningCapsuleComponent);

	Super::BeginPlay();
}

