// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "XYSAbilitySystemGlobals.generated.h"

struct FGameplayEffectContext;

/**
 * 
 */
UCLASS(Config=Game)
class XYSGAME_API UXYSAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	UXYSAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
};
