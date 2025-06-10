// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"

class UFrontendCommonButtonBase;
struct FGameplayTag;
class UWidget_ActivatableBase;
class UWidget_PrimaryLayout;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMainMenuButtonHoveredDelegate, UFrontendCommonButtonBase*, FrontendButton, FText, DescriptionText);

UENUM()
enum EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);

	void PushSoftWidgetClassToStackAsync(const FGameplayTag& InTag, TSoftClassPtr<UWidget_ActivatableBase>& InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AsyncPushWidgetCallback) const;

	UPROPERTY(BlueprintAssignable)
	FOnMainMenuButtonHoveredDelegate OnMainMenuButtonHovered;
	UPROPERTY(BlueprintAssignable)
	FOnMainMenuButtonHoveredDelegate OnMainMenuButtonUnHovered;
	
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InLayout);
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UWidget_PrimaryLayout> CreatedPrimaryLayout;
};
