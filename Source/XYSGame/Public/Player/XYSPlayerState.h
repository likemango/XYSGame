// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "XYSPlayerState.generated.h"

class UXYSPawnData;
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

	// ~Actor interface
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	// ~End
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState")
	UXYSAbilitySystemComponent* GetXYSAbilitySystemComponent() const { return AbilitySystemComponent;}
	UAttributeSet* GetAttributeSet() const;

	void SetPawnData(const UXYSPawnData* InPawnData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS|PlayerState")
	TObjectPtr<UXYSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GAS|PlayerState")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UXYSPawnData> PawnData;

	UFUNCTION()
	void OnRep_PawnData();
};
