// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"

class UCommonActivatableWidget;
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

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer,
		UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

	UCommonActivatableWidget* PushWidgetClassToStackAsync(const FGameplayTag& InTag, TSubclassOf<UCommonActivatableWidget> InSoftWidgetClass,
			TFunction<void(UCommonActivatableWidget*)> AsyncPushWidgetCallback);
	
	void PushSoftWidgetClassToStackAsync(const FGameplayTag& InTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AsyncPushWidgetCallback);

	void PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMessage,
		TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback);

	virtual void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	
	UPROPERTY(BlueprintAssignable)
	FOnMainMenuButtonHoveredDelegate OnMenuButtonHovered;
	UPROPERTY(BlueprintAssignable)
	FOnMainMenuButtonHoveredDelegate OnMenuButtonUnHovered;
	
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	// UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InLayout);
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UWidget_PrimaryLayout> CreatedPrimaryLayout;
};
