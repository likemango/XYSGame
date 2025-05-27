// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MotionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOTIONCORE_API AMotionPlayerController : public APlayerController
{
	GENERATED_BODY()

//~APlayerController interface
public:
	explicit AMotionPlayerController();

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
