// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "XYSCharacter.generated.h"

class UXYSHealthComponent;
class UXYSHeroComponent;
class UXYSPawnExtensionComponent;
class UXYSAbilitySystemComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UXYSCameraComponent;
class UCameraComponent;
class UXYSCharacterMovementComponent;

USTRUCT(BlueprintType)
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
};

/*
*   The base character pawn class used by this project.
 *	Responsible for sending events to pawn components.
 *	New behavior should be added via pawn components when possible.
 *  新逻辑应该添加到pawn component中，它只负责将消息转发给pawn component
 */
UCLASS(BlueprintType, Blueprintable)
class XYSGAME_API AXYSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXYSCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "XYS|Character")
	UXYSAbilitySystemComponent* GetXYSAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "XYS|Character")
	void UpperBodyEnterCrouchImplement();
	UFUNCTION(BlueprintImplementableEvent, Category = "XYS|Character")
	void UpperBodyLeaveCrouchImplement();
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

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

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);
	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();
	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();

	UPROPERTY(EditDefaultsOnly, Category = "XYSComponents")
	FName CameraAttachSocket = FName(TEXT("S_Camera"));

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "XYS|Character")
	FORCEINLINE USkeletalMeshComponent* GetFPUpperMesh() const { return FPUpperMesh;}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "XYS|Character")
	FORCEINLINE USkeletalMeshComponent* GetTPMesh() const { return GetMesh();}
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYS|Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UXYSCharacterMovementComponent> XYSMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "XYS|Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> FPUpperMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYS|Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UXYSCameraComponent> XYSCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "XYS|Character", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USkeletalMeshComponent> FPLowerMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYS|Character", Meta=(AllowPrivateAccess="true"))
	TObjectPtr<UXYSPawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYS|Character", Meta=(AllowPrivateAccess="true"))
	TObjectPtr<UXYSHeroComponent> HeroComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYS|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UXYSHealthComponent> HealthComponent;
};


















