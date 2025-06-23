// XiaoYao copyright.


#include "Character/XYSPawnExtensionComponent.h"

#include "Character/XYSPawnData.h"
#include "XYSGameplayTags.h"
#include "XYSLogChannels.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSPawnExtensionComponent)

const FName UXYSPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UXYSPawnExtensionComponent::UXYSPawnExtensionComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
	AbilitySystemComponent = nullptr;
}

void UXYSPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UXYSPawnExtensionComponent, PawnData);
}

void UXYSPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf(Pawn != nullptr,  TEXT("XYSHeroExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));
	
	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(UXYSPawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one XYSHeroExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));

	// Register with the init state system early, this will only work if this is a game world
	// 创建FActorFeatureData，并添加自己要实现的feature和实现者(自己)
	RegisterInitStateFeature();
}

void UXYSPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 当其他任何feature初始化状态发生改变时，回调：OnActorInitStateChanged
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// Notifies state manager that we have spawned, then try rest of default initialization
	ensure(TryToChangeInitState(XYSGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UXYSPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void UXYSPawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	// 除了自己之外的所有feature的implementer调用 CheckDefaultInitialization(相当于是更新自己依赖的其他feature)  
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = { XYSGameplayTags::InitState_Spawned, XYSGameplayTags::InitState_DataAvailable, XYSGameplayTags::InitState_DataInitialized, XYSGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

bool UXYSPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState) const
{
	check(Manager)

	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == XYSGameplayTags::InitState_Spawned)
	{
		// As long as we are on a valid pawn, we count as spawned
		return Pawn != nullptr;
	}
	if (CurrentState == XYSGameplayTags::InitState_Spawned && DesiredState == XYSGameplayTags::InitState_DataAvailable)
	{
		if (!PawnData)
			return false;

		if (Pawn->HasAuthority() || Pawn->IsLocallyControlled())
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
				return false;
		}
		return true;
	}
	if (CurrentState == XYSGameplayTags::InitState_DataAvailable && DesiredState == XYSGameplayTags::InitState_DataInitialized)
	{
		if ( Manager->HaveAllFeaturesReachedInitState(Pawn, XYSGameplayTags::InitState_DataAvailable))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (CurrentState == XYSGameplayTags::InitState_DataInitialized && DesiredState == XYSGameplayTags::InitState_GameplayReady)
	{
		// 说明自己和所有组件各自的feature都已经加载完成，允许进入GameplayReady
		return true;
	}
	
	return false;
}

void UXYSPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager,FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (DesiredState == XYSGameplayTags::InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change
	}
	if (DesiredState == XYSGameplayTags::InitState_GameplayReady)
	{
		UE_LOG(LogXYSAbilitySystem, Warning, TEXT("Extension is under InitState_GameplayReady"))
		OnPawnExtensionReady.Broadcast();
	}
}

void UXYSPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// 我监听所有feature的状态更新，当其他feature状态更新时，我判断一下我是否可以更新了
		if (Params.FeatureState == XYSGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UXYSPawnExtensionComponent::SetPawnData(const UXYSPawnData* InPawnData)
{
	check(InPawnData != nullptr);

	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
		return;

	if (PawnData)
	{
		UE_LOG(LogXYSGame, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
}

void UXYSPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

void UXYSPawnExtensionComponent::InitializeAbilitySystem(UXYSAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		// The ability system component hasn't changed.
		return;
	}

	// ASC是允许替换和更新的
	if (AbilitySystemComponent)
	{
		// Clean up the old ability system component.
		UninitializeAbilitySystem();
	}
	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InASC->GetAvatarActor();
	UE_LOG(LogXYSGame, Verbose, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "), *GetNameSafe(InASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAvatar));

	if ((ExistingAvatar != nullptr) && (ExistingAvatar != Pawn))
	{
		UE_LOG(LogXYSGame, Log, TEXT("Existing avatar (authority=%d)"), ExistingAvatar->HasAuthority() ? 1 : 0);

		// ASC上已经有一个avatar了，并且不是我这个pawn，因此我们要踢掉它，并将它换成我们这个；
		// 在客户端上由于较大延迟，如果新的pawn已经生成并且走到这里了，但是旧的pawn还没有被清理，就会出现这个情况
		// 不可能在服务器上发生
		ensure(!ExistingAvatar->HasAuthority());

		if (UXYSPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
		{
			OtherExtensionComponent->UninitializeAbilitySystem();
		}
	}
	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	if (ensure(PawnData))
	{
		InASC->SetTagRelationshipMapping(PawnData->TagRelationshipMapping);
	}

	OnAbilitySystemInitialized.Broadcast();
}

void UXYSPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		FGameplayTagContainer AbilityTypesToIgnore;
		AbilityTypesToIgnore.AddTag(XYSGameplayTags::Ability_Behavior_SurvivesDeath);

		AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
		// todo
		// AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void UXYSPawnExtensionComponent::HandleControllerChanged()
{
	if (AbilitySystemComponent && (AbilitySystemComponent->GetAvatarActor() == GetPawnChecked<APawn>()))
	{
		ensure(AbilitySystemComponent->AbilityActorInfo->OwnerActor == AbilitySystemComponent->GetOwnerActor());
		if (AbilitySystemComponent->GetOwnerActor() == nullptr)
		{
			UninitializeAbilitySystem();
		}
		else
		{
			// 更新ASC的ActorInfo(FGameplayAbilityActorInfo)，包括Controller/SkeletonMesh/AnimInstance等等
			AbilitySystemComponent->RefreshAbilityActorInfo();
		}
	}
	CheckDefaultInitialization();
}

void UXYSPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}

void UXYSPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UXYSPawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}
	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void UXYSPawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}
