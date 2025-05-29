// XiaoYao copyright.


#include "Character/XYSHeroComponent.h"

UXYSHeroComponent::UXYSHeroComponent(const FObjectInitializer& ObjectInitializer)
{
}

bool UXYSHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState) const
{
	check(Manager);

	
}

void UXYSHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
}

void UXYSHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

void UXYSHeroComponent::CheckDefaultInitialization()
{
	IGameFrameworkInitStateInterface::CheckDefaultInitialization();
}

void UXYSHeroComponent::OnRegister()
{
	Super::OnRegister();

	
}

void UXYSHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UXYSHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UXYSHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
}
