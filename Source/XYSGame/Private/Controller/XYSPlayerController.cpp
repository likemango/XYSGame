// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/XYSPlayerController.h"

#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Player/XYSPlayerState.h"

AXYSPlayerController::AXYSPlayerController(const FObjectInitializer& ObjectInitializer)
{
	
}

AXYSPlayerState* AXYSPlayerController::GetXYSPlayerState() const
{
	return CastChecked<AXYSPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UXYSAbilitySystemComponent* AXYSPlayerController::GetXYSAbilitySystemComponent() const
{
	const AXYSPlayerState* XYSPS = GetXYSPlayerState();
	return XYSPS ? XYSPS->GetXYSAbilitySystemComponent() : nullptr;
}


void AXYSPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UXYSAbilitySystemComponent* ASC = GetXYSAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

