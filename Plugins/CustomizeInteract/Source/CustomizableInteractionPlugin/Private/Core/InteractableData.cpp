// Copyright Vinipi Studios 2024. All rights reserved.

#include "Core/InteractableData.h"
#include "Core/InteractionKey.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

FInteractableData::FInteractableData()
{
	static ConstructorHelpers::FObjectFinder<UInteractionKey> DefaultKeyFinder(CIP_DEFAULT_KEY_PATH);
	if (DefaultKeyFinder.Succeeded())
	{
		InteractionKey = DefaultKeyFinder.Object;
	}
}
