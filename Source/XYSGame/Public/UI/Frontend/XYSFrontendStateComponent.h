// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "ControlFlowNode.h"
#include "Components/GameStateComponent.h"
#include "Interface/FrontendLoadingScreenInterface.h"
#include "XYSFrontendStateComponent.generated.h"


class UWidget_ActivatableBase;
class UXYSExperienceDefinition;
class UCommonActivatableWidget;

UCLASS(Abstract)
class XYSGAME_API UXYSFrontendStateComponent : public UGameStateComponent, public IFrontendLoadingScreenInterface
{
	GENERATED_BODY()
	
public:
	UXYSFrontendStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface

private:
	void OnExperienceLoaded(const UXYSExperienceDefinition* Experience);

	void FlowStep_TryShowPressStartScreen(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow);

	bool bShouldShowLoadingScreen = true;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UWidget_ActivatableBase> PressStartScreenClass;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UWidget_ActivatableBase> MainScreenClass;

	TSharedPtr<FControlFlow> FrontEndFlow;
	
	// If set, this is the in-progress press start screen task
	FControlFlowNodePtr InProgressPressStartScreen;

	FDelegateHandle OnJoinSessionCompleteEventHandle;
};
