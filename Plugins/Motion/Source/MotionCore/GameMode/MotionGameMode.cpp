// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.


#include "MotionGameMode.h"

#include "MotionCore/Player/MotionPlayerController.h"
#include "MotionCore/Character/MotionCharacter.h"

AMotionGameMode::AMotionGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = AMotionPlayerController::StaticClass();
	DefaultPawnClass = AMotionCharacter::StaticClass();
}
