// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "MotionCore/Character/MotionCharacter.h"
#include "XYSCharacter.generated.h"

class UXYSHeroComponent;
class UXYSPawnExtensionComponent;
class UXYSAbilitySystemComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UXYSCameraComponent;
class UCameraComponent;
class UXYSCharacterMovementComponent;

/*USTRUCT(BlueprintType)
struct FCharacterGroundInfo
{
	GENERATED_BODY()

	FCharacterGroundInfo() :
	LastUpdateFrame(0),
	GroundHitResult(FHitResult()),
	GroundDistance(0)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;	
};*/

/*
*   The base character pawn class used by this project.
 *	Responsible for sending events to pawn components.
 *	New behavior should be added via pawn components when possible.
 *  新逻辑应该添加到pawn component中，它只负责将消息转发给pawn component
 */
UCLASS()
class XYSGAME_API AXYSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXYSCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Character")
	UXYSAbilitySystemComponent* GetXYSAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/*
	 * Basic locomotion logic
	 */
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	// void Input_Crouch(const FInputActionValue& InputActionValue);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float TurnRateCamera = 50.0f;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

//~Animation
	UFUNCTION(BlueprintCallable, Category = "Animation")
	const FCharacterGroundInfo& GetGroundInfo();

protected:
	// Ground Info that is used by the Animation Blueprint that ships with Motion
	FCharacterGroundInfo CachedGroundInfo;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> DefaultAnimLinkedLayerForAllSkeletalMeshes;
//~End of animation

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYSComponents")
	TObjectPtr<UXYSCharacterMovementComponent> XYSMovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYSComponents")
	TObjectPtr<UXYSCameraComponent> XYSCameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "XYSComponents")
	FName CameraAttachSocket = FName(TEXT("S_Camera"));
	
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UXYSPawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UXYSHeroComponent> HeroComponent;
};


















