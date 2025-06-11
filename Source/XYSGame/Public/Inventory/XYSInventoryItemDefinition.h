// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XYSInventoryItemDefinition.generated.h"

class UXYSInventoryItemInstance;

UCLASS(MinimalAPI, DefaultToInstanced, EditInlineNew, Abstract)
class UXYSInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UXYSInventoryItemInstance* Instance) const{};
};

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class XYSGAME_API UXYSInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UXYSInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UXYSInventoryItemFragment>> Fragments;

	const UXYSInventoryItemFragment* FindFragmentByClass(TSubclassOf<UXYSInventoryItemFragment> FragmentClass) const;
};
