// XiaoYao copyright.


#include "UI/Frontend/XYSFrontendStateComponent.h"
#include "Widgets/Widget_ActivatableBase.h"

#include "ControlFlowManager.h"
#include "FrontendGameplayTags.h"
#include "GameModes/XYSExperienceManagerComponent.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "System/XYSGameInstance.h"

UXYSFrontendStateComponent::UXYSFrontendStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UXYSFrontendStateComponent::BeginPlay()
{
	Super::BeginPlay();

	AGameStateBase* GameState = GetGameState<AGameStateBase>();
	check(GameState);
	UXYSExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UXYSExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded_HighPriority(FOnXYSExperienceLoaded::FDelegate::CreateUObject
		(this, &UXYSFrontendStateComponent::OnExperienceLoaded));
}

void UXYSFrontendStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UXYSFrontendStateComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (bShouldShowLoadingScreen)
	{
		OutReason = TEXT("Frontend Flow Pending...");

		if (FrontEndFlow.IsValid())
		{
			const TOptional<FString> StepDebugName = FrontEndFlow->GetCurrentStepDebugName();
			if (StepDebugName.IsSet())
			{
				OutReason = StepDebugName.GetValue();
			}
		}
		return true;
	}
	return false;
}

void UXYSFrontendStateComponent::OnExperienceLoaded(const UXYSExperienceDefinition* Experience)
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("FrontendFlow"))
		.QueueStep(TEXT("Wait For PrimaryLayout Initialization"), this, &ThisClass::FlowStep_WaitForPrimaryLayoutInitialization)
		.QueueStep(TEXT("Try Show Press Start Screen"), this, &ThisClass::FlowStep_TryShowPressStartScreen)
		// .QueueStep(TEXT("Try Join Requested Session"), this, &ThisClass::FlowStep_TryJoinRequestedSession)
		.QueueStep(TEXT("Try Show Main Screen"), this, &ThisClass::FlowStep_TryShowMainScreen);

	Flow.ExecuteFlow();

	FrontEndFlow = Flow.AsShared();
}

void UXYSFrontendStateComponent::FlowStep_WaitForPrimaryLayoutInitialization(FControlFlowNodeRef SubFlow)
{
	UFrontendUISubsystem* UISubsystem = UFrontendUISubsystem::Get(this);
	ensure(UISubsystem);

	UISubsystem->RegisterAndCallPrimaryLayoutCreated(FSimpleMulticastDelegate::FDelegate::CreateLambda([SubFlow]
	{
		SubFlow->ContinueFlow();
	}));
}

void UXYSFrontendStateComponent::FlowStep_TryShowPressStartScreen(FControlFlowNodeRef SubFlow)
{
	// Add the Press Start screen, move to the next flow when it deactivates.
	if (!GetWorld() || !GetWorld()->GetGameInstance())
		return;
	
	if (UFrontendUISubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UFrontendUISubsystem>())
	{
		UISubsystem->PushSoftWidgetClassToStackAsync(FrontendGameplayTags::Frontend_WidgetStack_Menu, PressStartScreenClass,
			[this, SubFlow](EAsyncPushWidgetState State, UCommonActivatableWidget* Screen) {
			switch (State)
			{
			case EAsyncPushWidgetState::AfterPush:
				bShouldShowLoadingScreen = false;
				Screen->OnDeactivated().AddWeakLambda(this, [this, SubFlow]() {
					SubFlow->ContinueFlow();
				});
				break;
			}
		});
	}
}


void UXYSFrontendStateComponent::FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow)
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
		return;
	
	if (UFrontendUISubsystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UFrontendUISubsystem>())
	{
		UISubsystem->PushSoftWidgetClassToStackAsync(FrontendGameplayTags::Frontend_WidgetStack_Menu, MainScreenClass,
			[this, SubFlow](EAsyncPushWidgetState State, UCommonActivatableWidget* Screen) {
			switch (State)
			{
			case EAsyncPushWidgetState::AfterPush:
				bShouldShowLoadingScreen = false;
				SubFlow->ContinueFlow();
				return;
			}
		});
	}
}
