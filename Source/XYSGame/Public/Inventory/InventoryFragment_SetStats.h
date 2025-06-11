// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Inventory/XYSInventoryItemDefinition.h"

#include "InventoryFragment_SetStats.generated.h"

class UXYSInventoryItemInstance;
class UObject;
struct FGameplayTag;

UCLASS()
class XYSGAME_API UInventoryFragment_SetStats : public UXYSInventoryItemFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(UXYSInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
