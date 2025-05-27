// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MotionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MOTIONCORE_API AMotionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMotionGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
