// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSGameplayAbility.h"
#include "XYSGameplayAbility_Crouch.generated.h"

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameplayAbility_Crouch : public UXYSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UXYSGameplayAbility_Crouch(const FObjectInitializer& ObjectInitializer);
	
	//~Begin GameplayAbility
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void CharacterCrouchStart();

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void CharacterCrouchEnd();
};
