// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XYSAbilitySystemComponent.generated.h"

class UXYSAbilityTagRelationshipMapping;
/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	/** Sets the current tag relationship mapping, if null it will clear it out */
	void SetTagRelationshipMapping(UXYSAbilityTagRelationshipMapping* NewMapping);


protected:
	// If set, this table is used to look up tag relationships for activate and cancel
	UPROPERTY()
	TObjectPtr<UXYSAbilityTagRelationshipMapping> TagRelationshipMapping;
};
