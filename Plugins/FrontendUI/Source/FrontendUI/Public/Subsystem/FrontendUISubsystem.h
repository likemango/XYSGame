// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSGameUIManagerSubsystem.h"
#include "FrontendTypes/FrontendTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"

class UXYSCommonLocalPlayer;
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

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UWidget_PrimaryLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UWidget_PrimaryLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};


/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendUISubsystem : public UXYSGameUIManagerSubsystem
{
	GENERATED_BODY()

public:
	static UWidget_PrimaryLayout* GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject);
	static UWidget_PrimaryLayout* GetPrimaryGameLayout(APlayerController* PlayerController);
	static UWidget_PrimaryLayout* GetPrimaryGameLayout(ULocalPlayer* LocalPlayer);
	
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);
	
	FSimpleMulticastDelegate& GetPrimaryLayoutAddedDelegate() {return OnPrimaryLayoutAdded;}
	FSimpleMulticastDelegate OnPrimaryLayoutAdded;

	void RegisterAndCallPrimaryLayoutCreated(FSimpleMulticastDelegate::FDelegate Delegate);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer,
		UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

	UCommonActivatableWidget* PushWidgetClassToStackAsync(const FGameplayTag& InTag, TSubclassOf<UCommonActivatableWidget> InSoftWidgetClass,
			TFunction<void(UCommonActivatableWidget*)> AsyncPushWidgetCallback);
	
	void PushSoftWidgetClassToStackAsync(const FGameplayTag& InTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AsyncPushWidgetCallback);

	void PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMessage,
		TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback);

	virtual void NotifyPlayerAdded(UXYSCommonLocalPlayer* LocalPlayer) override;
	virtual void NotifyPlayerDestroyed(UXYSCommonLocalPlayer* LocalPlayer) override;
	virtual void NotifyPlayerRemoved(UXYSCommonLocalPlayer* LocalPlayer) override;

	UWidget_PrimaryLayout* GetRootLayout(const UXYSCommonLocalPlayer* LocalPlayer) const ;
	
	UPROPERTY(BlueprintAssignable)
	FOnMainMenuButtonHoveredDelegate OnMenuButtonHovered;
	UPROPERTY(BlueprintAssignable)
	FOnMainMenuButtonHoveredDelegate OnMenuButtonUnHovered;
	
protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void CreateLayoutWidget(UXYSCommonLocalPlayer* InLocalPlayer);
	TSubclassOf<UWidget_PrimaryLayout> GetLayoutWidgetClass() const;

	void AddLayoutToViewport(UXYSCommonLocalPlayer* InLocalPlayer, UWidget_PrimaryLayout* InRootLayout);
	void RemoveLayoutFromViewport(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout);

	virtual void OnRootLayoutAddedToViewport(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout);
	virtual void OnRootLayoutRemovedFromViewport(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout);
	virtual void OnRootLayoutReleased(UXYSCommonLocalPlayer* LocalPlayer, UWidget_PrimaryLayout* Layout);

private:
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};
