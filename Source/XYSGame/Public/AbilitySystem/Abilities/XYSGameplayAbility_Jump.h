// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/XYSGameplayAbility.h"
#include "XYSGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameplayAbility_Jump : public UXYSGameplayAbility
{
	GENERATED_BODY()

public:
	UXYSGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer);

protected:

	//~Begin GameplayAbility
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void CharacterJumpEnd();
};
