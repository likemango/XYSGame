// XiaoYao copyright.


#include "Player/XYSPlayerState.h"

#include "XYSLogChannels.h"
#include "AbilitySystem/XYSAbilitySet.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Character/XYSPawnData.h"
#include "GameModes/XYSGameMode.h"
#include "Net/UnrealNetwork.h"

AXYSPlayerState::AXYSPlayerState(const FObjectInitializer& ObjectInitializer)
{

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UXYSAbilitySystemComponent>(this, "AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");

	SetNetUpdateFrequency(100);
}

void AXYSPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}

UAbilitySystemComponent* AXYSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AXYSPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}


void AXYSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AXYSGameMode* XYSGameMode = Cast<AXYSGameMode>(World->GetAuthGameMode());
		check(XYSGameMode);
		SetPawnData(XYSGameMode->PawnData);
	}
}

void AXYSPlayerState::SetPawnData(const UXYSPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority) return;

	if (PawnData)
	{
		UE_LOG(LogXYSGame, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UXYSAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			// 默认的技能，没有用handle引用
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
	// UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_LyraAbilityReady);

	ForceNetUpdate();
}

void AXYSPlayerState::OnRep_PawnData()
{
}
