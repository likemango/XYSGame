// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSGameplayAbility.h"
#include "XYSGameplayAbility_Reset.generated.h"

USTRUCT(BlueprintType)
struct FXYSPlayerResetMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerPlayerState = nullptr;
};


/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSGameplayAbility_Reset : public UXYSGameplayAbility
{
	GENERATED_BODY()

public:
	UXYSGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
