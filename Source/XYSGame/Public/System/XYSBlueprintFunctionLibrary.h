// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XYSBlueprintFunctionLibrary.generated.h"

class UXYSInventoryComponent;
class UXYSInventoryItemDefinition;
class UXYSInventoryItemFragment;

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

};

UCLASS()
class UXYSInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UXYSInventoryItemFragment* FindFragmentFromItemDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef, TSubclassOf<UXYSInventoryItemFragment> FragmentClass);
};


