// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/XYSController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AXYSController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(EnhancedInputLocalPlayerSubsystem);

	EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultInputMappingContext, 0);

	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this,  &AXYSController::InputAction_Move);
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this,  &AXYSController::InputAction_Look);
}

void AXYSController::InputAction_Move(const FInputActionValue& InputValue)
{
	FVector2D XYValue = InputValue.Get<FVector2D>();
	// X: Left/Right   Y: Foward/Backward
	if (XYValue != FVector2D::ZeroVector)
	{
		// FVector ActorForward = ControlledPawn->GetActorForwardVector();
		// FVector ActorRight = ControlledPawn->GetActorRightVector();
		// ControlledPawn->AddMovementInput(ActorForward, XYValue.Y);
		// ControlledPawn->AddMovementInput(ActorRight, XYValue.X);
		
		// 应该从相机角度出发设置朝向
		APawn* ControlledPawn = GetPawn();
		check(ControlledPawn);
		FRotator YawRotator = FRotator(0, GetControlRotation().Yaw, 0);
		const FVector ControlForwardDir = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector ControlRightDir = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		ControlledPawn->AddMovementInput(ControlForwardDir, XYValue.Y);
		ControlledPawn->AddMovementInput(ControlRightDir, XYValue.X);
	}
}

void AXYSController::InputAction_Look(const FInputActionValue& InputValue)
{
	FVector2D XYValue = InputValue.Get<FVector2D>();
	// X: Look Left/Right   Y: Look Up/Down
	if (XYValue != FVector2D::ZeroVector)
	{
		AddPitchInput(XYValue.Y);
		AddYawInput(XYValue.X);
	}
}
