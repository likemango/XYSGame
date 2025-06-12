// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/XYSGameplayAbility.h"
#include "XYSGameplayAbility_FromEquipment.generated.h"

class UXYSInventoryItemInstance;
class UXYSEquipmentInstance;
/**
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class XYSGAME_API UXYSGameplayAbility_FromEquipment : public UXYSGameplayAbility
{
	GENERATED_BODY()
	
public:

	UXYSGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="XYS|Ability")
	UXYSEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "XYS|Ability")
	UXYSInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
