// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "XYSInputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

// USTRUCT(BlueprintType)
// struct FInputMappingContextAndPriority
// {
// 	GENERATED_BODY()
//
// 	/* todo: 临时修改为强引用，后面考虑使用GF来动态处理 */
// 	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client,Server"))
// 	TObjectPtr<UInputMappingContext> InputMapping;
//
// 	// Higher priority input mappings will be prioritized over mappings with a lower priority.
// 	UPROPERTY(EditAnywhere, Category="Input")
// 	int32 Priority = 0;
// 	
// 	// /** If true, then this mapping context will be registered with the settings when this game feature action is registered. */
// 	// UPROPERTY(EditAnywhere, Category="Input")
// 	// bool bRegisterWithSettings = true;
// };


USTRUCT(BlueprintType)
struct FXYSInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 * on-mutable data asset that contains input configuration properties.
 */
UCLASS(BlueprintType, Const)
class XYSGAME_API UXYSInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UXYSInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FXYSInputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FXYSInputAction> AbilityInputActions;	
};
