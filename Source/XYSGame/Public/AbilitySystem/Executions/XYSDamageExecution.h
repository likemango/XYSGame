// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "XYSDamageExecution.generated.h"

/**
 * Execution used by gameplay effects to apply damage to the health attributes.
 */
UCLASS()
class XYSGAME_API UXYSDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UXYSDamageExecution();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
