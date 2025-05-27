// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.


#include "MotionPlayerController.h"

#include "MotionCore/AbilitySystem/MotionAbilitySystemComponent.h"
#include "MotionCore/Character/MotionCharacter.h"

AMotionPlayerController::AMotionPlayerController()
{
	
}

void AMotionPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (const AMotionCharacter* MotionCharacter = Cast<AMotionCharacter>(GetCharacter()))
	{
		UMotionAbilitySystemComponent* AbilitySystemComponent = Cast<UMotionAbilitySystemComponent>(MotionCharacter->GetAbilitySystemComponent());
		AbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
