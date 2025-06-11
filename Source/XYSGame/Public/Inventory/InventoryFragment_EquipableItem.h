// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/XYSInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

#include "InventoryFragment_EquipableItem.generated.h"

class UXYSEquipmentDefinition;
class UObject;

UCLASS()
class XYSGAME_API UInventoryFragment_EquipableItem : public UXYSInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UXYSEquipmentDefinition> EquipmentDefinition;
};
