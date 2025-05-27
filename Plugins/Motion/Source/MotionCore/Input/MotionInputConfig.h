// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MotionInputConfig.generated.h"

class UGameplayAbility;
class UInputAction;
class UObject;
struct FFrame;

USTRUCT(BlueprintType)
struct FMotionInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FMotionGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Motion")
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion")
	TObjectPtr<const UInputAction> InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Motion")
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class UMotionInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UMotionInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Motion")
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UFUNCTION(BlueprintCallable, Category = "Motion")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion | Input", Meta = (TitleProperty = "InputAction"))
	TArray<FMotionInputAction> TaggedNativeActions;

	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion | Input", Meta = (TitleProperty = "InputAction"))
	TArray<FMotionGameplayAbility> TaggedAbilityActions;
};
