// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "XYSPlayerState.generated.h"

class UXYSAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class XYSGAME_API AXYSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXYSPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState")
	UXYSAbilitySystemComponent* GetXYSAbilitySystemComponent() const { return AbilitySystemComponent;}
	
	UAttributeSet* GetAttributeSet() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS|PlayerState")
	TObjectPtr<UXYSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS|PlayerState")
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
