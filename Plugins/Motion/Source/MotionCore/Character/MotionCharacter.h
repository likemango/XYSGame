// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "MotionCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "MotionCore/AbilitySystem/MotionAbilitySystemComponent.h"
#include "MotionCore/Core/MotionCurveManager.h"
#include "MotionCore/Input/MotionInputConfig.h"
#include "MotionCharacter.generated.h"

class UMotionCameraComponent;
struct FInputActionValue;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVelocityBeginSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVelocityStopSignature);

UCLASS()
class MOTIONCORE_API AMotionCharacter : public ACharacter, public IGameplayTagAssetInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit AMotionCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void HandleCameraCollision(float DeltaSeconds) const;
	
	virtual void PostInitializeComponents() override;

// Events
public:
	/* Event that gets fired when the player starts to move. */
	UPROPERTY(BlueprintAssignable, Category = "Motion")
	FOnVelocityBeginSignature OnVelocityBeginDelegate;

	/* Event that gets fired when the player stops to move. */
	UPROPERTY(BlueprintAssignable, Category = "Motion")
	FOnVelocityStopSignature OnVelocityStopDelegate;
private:
	bool bHasVelocityEventFired;
	
// Camera
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	UMotionCameraComponent* MotionCameraComponent;

// Collision Detection
public:
	/* Capsule Collision used to detect geometry in front of the camera and prevent the camera from clipping into it. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	UCapsuleComponent* CollisionFeeler;

	/* Height and radius of the CollisionFeeler capsule. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	float CollisionFeelerSize = 15.0f;

	/* Distance that the collision feeler should stick in front of the camera. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	float CollisionFeelerCameraDistance = -5.0f;
	
	/* Extra size added to the collision feeler dimensions when doing the capsule trace to detect if the collision feeler is encroached.
	 * Note: This size is only added to the Z-axis of the trace object. Meaning it only increases the size it goes downwards.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	float CollisionFeelerTraceExtraSize = 5.0f;

	/* Distance that the player capsule should be pushed back in case the feeler overlaps with geometry. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	float CollisionFeelerPlayerPushbackDistance = 7.5f;

	/* Interp Speed used when interpolating towards the determined target position for the capsule after a collision was detected and requires the capsule to be pushed back from. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	float CollisionFeelerPlayerPushbackInterpSpeed = 20.0f;

	/* Updated location that the player gets moved to once a collision has been detected and accounted for. */
	FVector CollisionPreventionLocation;

// Curves
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	UMotionCurveManager* CameraRotationCurvesManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
	UMotionCurveManager* CameraLocationCurvesManager;

//~AbilitySystem
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Motion")
	TObjectPtr<UMotionAbilitySystemComponent> AbilitySystemComponent;
//~End of AbilitySystem

//~GameplayTags
public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	// Gameplay-related tags associated with this actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Motion")
	FGameplayTagContainer GameplayTags;
//~End of GameplayTags

//~Input
protected:
	UPROPERTY(EditAnywhere, Category= "Motion")
	TArray<UInputMappingContext*> DefaultInputMappings;
	
	UPROPERTY(EditAnywhere, Category = "Motion")
	UMotionInputConfig* InputConfig;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Multiplier with which the input gets multiplied with before applying the input
	// Not the most elegant solution, will be replaced in 1.6.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float TurnRateCamera = 50.0f;

	// Default Move and Look implementations.
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	// Not implemented. Crouch is implemented by BPC_CrouchComponent instead.
	UFUNCTION(BlueprintImplementableEvent, Category = "Motion")
	void Input_Crouch(const FInputActionValue& InputActionValue);

	// Not implemented. Jump is implemented by BPC_JumpComponent instead.
	UFUNCTION(BlueprintImplementableEvent, Category = "Motion")
	void Input_Jump(const FInputActionValue& InputActionValue);

	// Handles Ability Tag Input
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

//~End of Input

//~Animation
protected:
	// Ground Info that is used by the Animation Blueprint that ships with Motion
	FCharacterGroundInfo CachedGroundInfo;

public:
	UFUNCTION(BlueprintCallable, Category = "Motion")
	const FCharacterGroundInfo& GetGroundInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> DefaultAnimLinkedLayerForAllSkeletalMeshes;
//~End of Animation

//~Movement Component
public:
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
	UMotionCharacterMovementComponent* MovementComponent;
};
//~End of Movement Component