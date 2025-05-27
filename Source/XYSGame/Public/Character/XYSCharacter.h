// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionCore/Core/MotionStructs.h"
#include "XYSCharacter.generated.h"

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


UCLASS()
class XYSGAME_API AXYSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AXYSCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	/*
	 * Basic locomotion logic
	 */
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Look;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Jump;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Crouch;
	
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
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYSComponents")
	TObjectPtr<UXYSCharacterMovementComponent> XYSMovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XYSComponents")
	TObjectPtr<UXYSCameraComponent> XYSCameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "XYSComponents")
	FName CameraAttachSocket = FName(TEXT("S_Camera"));
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float TurnRateCamera = 50.0f;
};


















