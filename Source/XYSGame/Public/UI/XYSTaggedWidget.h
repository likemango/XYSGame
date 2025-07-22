// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "XYSTaggedWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class XYSGAME_API UXYSTaggedWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UXYSTaggedWidget(const FObjectInitializer& ObjectInitializer);

	//~UWidget interface
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	//~End of UWidget interface

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

protected:
	/** If the owning player has any of these tags, this widget will be hidden (using HiddenVisibility) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	FGameplayTagContainer HiddenByTags;

	/** The visibility to use when this widget is shown (not hidden by gameplay tags). */
	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility ShownVisibility = ESlateVisibility::Visible;

	/** The visibility to use when this widget is hidden by gameplay tags. */
	UPROPERTY(EditAnywhere, Category = "HUD")
	ESlateVisibility HiddenVisibility = ESlateVisibility::Collapsed;

	/** Do we want to be visible (ignoring tags)? */
	bool bWantsToBeVisible = true;

private:
	void OnWatchedTagsChanged();
};
