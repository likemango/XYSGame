// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "MotionCore/Character/MotionCharacterMovementComponent.h"
#include "MotionComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOTIONCORE_API UMotionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UMotionComponent();

	/* Owning character of this component. */
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
	ACharacter* OwningCharacter;

	/* Owning Motion character movement component of this component's owner. */
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
	UMotionCharacterMovementComponent* OwningCharacterMovementComponent;

	/* Owning capsule component of this component's owner. */
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
	UCapsuleComponent* OwningCapsuleComponent;
};