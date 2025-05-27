// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYSActorComponent.generated.h"


class UCapsuleComponent;
class UXYSCharacterMovementComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XYSGAME_API UXYSActorComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UXYSActorComponent();

	/* Owning character of this component. */
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
	ACharacter* OwningCharacter;

	/* Owning Motion character movement component of this component's owner. */
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
	UXYSCharacterMovementComponent* OwningCharacterMovementComponent;

	/* Owning capsule component of this component's owner. */
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
	UCapsuleComponent* OwningCapsuleComponent;

		
};
