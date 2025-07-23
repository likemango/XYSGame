// XiaoYao copyright.


#include "AbilitySystem/XYSAbilitySystemComponent.h"

#include "XYSGameplayTags.h"
#include "XYSLogChannels.h"
#include "AbilitySystem/XYSAbilityTagRelationshipMapping.h"
#include "AbilitySystem/XYSGlobalAbilitySystem.h"
#include "AbilitySystem/Abilities/XYSGameplayAbility.h"
#include "Animation/XYSAnimInstance.h"
#include "Character/XYSCharacter.h"
#include "System/XYSAssetManager.h"
#include "System/XYSGameData.h"

UXYSAbilitySystemComponent::UXYSAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	InputStartActivationRecords.Reset();

	FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UXYSAbilitySystemComponent::SetTagRelationshipMapping(UXYSAbilityTagRelationshipMapping* NewMapping)
{
	TagRelationshipMapping = NewMapping;
}

void UXYSAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);

				// 当一个按键周期中，只有已经释放过，才会再次加入; 并且记录没有激活过
				if (!InputStartActivationRecords.Contains(AbilitySpec.Handle))
					InputStartActivationRecords.Add(AbilitySpec.Handle, false);
			}
		}
	}
}

void UXYSAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);

				// 移除时无需判断
				InputStartActivationRecords.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UXYSAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(XYSGameplayTags::TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();
	//@TODO: See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops
	
	// Process all abilities that activate when the input is held.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UXYSGameplayAbility* XYSAbilityCDO = Cast<UXYSGameplayAbility>(AbilitySpec->Ability);
				if (XYSAbilityCDO && XYSAbilityCDO->GetActivationPolicy() == EXYSAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	// Process all abilities that had their input pressed this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UXYSGameplayAbility* XYSAbilityCDO = Cast<UXYSGameplayAbility>(AbilitySpec->Ability);

					if (XYSAbilityCDO && XYSAbilityCDO->GetActivationPolicy() == EXYSAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// Process all abilities that had their input start on this press process.
	for (TTuple<FGameplayAbilitySpecHandle, bool>& Pair : InputStartActivationRecords)
	{
		const FGameplayAbilitySpecHandle& SpecHandle = Pair.Key;
		// 如果已经尝试激活过，那么不执行
		if (Pair.Value) continue;
		
		Pair.Value = true;
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UXYSGameplayAbility* XYSAbilityCDO = Cast<UXYSGameplayAbility>(AbilitySpec->Ability);

					if (XYSAbilityCDO && XYSAbilityCDO->GetActivationPolicy() == EXYSAbilityActivationPolicy::OnInputStarted)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}
	
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// Clear the cached ability handles.
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UXYSAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UXYSAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this](const UXYSGameplayAbility* XYSAbility, FGameplayAbilitySpecHandle Handle)
	{
		const EXYSAbilityActivationPolicy ActivationPolicy = XYSAbility->GetActivationPolicy();
		return ((ActivationPolicy == EXYSAbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == EXYSAbilityActivationPolicy::WhileInputActive));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

bool UXYSAbilitySystemComponent::IsActivationGroupBlocked(EXYSAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case EXYSAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.
		bBlocked = false;
		break;

	case EXYSAbilityActivationGroup::Exclusive_Replaceable:
	case EXYSAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking.
		bBlocked = (ActivationGroupCounts[(uint8)EXYSAbilityActivationGroup::Exclusive_Blocking] > 0);
		break;

	default:
		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	return bBlocked;
}

void UXYSAbilitySystemComponent::AddAbilityToActivationGroup(EXYSAbilityActivationGroup Group, UXYSGameplayAbility* XYSAbility)
{
	check(XYSAbility);
	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);

	ActivationGroupCounts[(uint8)Group]++;

	const bool bReplicateCancelAbility = false;

	switch (Group)
	{
	case EXYSAbilityActivationGroup::Independent:
		// Independent abilities do not cancel any other abilities.
		break;

	case EXYSAbilityActivationGroup::Exclusive_Replaceable:
	case EXYSAbilityActivationGroup::Exclusive_Blocking:
		CancelActivationGroupAbilities(EXYSAbilityActivationGroup::Exclusive_Replaceable, XYSAbility, bReplicateCancelAbility);
		break;

	default:
		checkf(false, TEXT("AddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)EXYSAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)EXYSAbilityActivationGroup::Exclusive_Blocking];
	if (!ensure(ExclusiveCount <= 1))
	{
		UE_LOG(LogXYSAbilitySystem, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."));
	}
}

void UXYSAbilitySystemComponent::RemoveAbilityFromActivationGroup(EXYSAbilityActivationGroup Group,UXYSGameplayAbility* XYSAbility)
{
	check(XYSAbility);
	check(ActivationGroupCounts[(uint8)Group] > 0);

	ActivationGroupCounts[(uint8)Group]--;
}

void UXYSAbilitySystemComponent::CancelActivationGroupAbilities(EXYSAbilityActivationGroup Group,
																UXYSGameplayAbility* IgnoreXYSAbility, bool bReplicateCancelAbility)
{
	TShouldCancelAbilityFunc ShouldCancelFunc = [this, Group, IgnoreXYSAbility](const UXYSGameplayAbility* XYSAbility, FGameplayAbilitySpecHandle Handle)
	{
		return ((XYSAbility->GetActivationGroup() == Group) && (XYSAbility != IgnoreXYSAbility));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UXYSAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle,
	FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	if (ReplicatedData.IsValid())
	{
		OutTargetDataHandle = ReplicatedData->TargetData;
	}
}

void UXYSAbilitySystemComponent::RegisterGameplayTagChangedEvent()
{
	RegisterGenericGameplayTagEvent().AddUObject(this, &UXYSAbilitySystemComponent::OnAbilitySystemTagChanged);
}

void UXYSAbilitySystemComponent::OnAbilitySystemTagChanged(const FGameplayTag InTag, int32 TagCount) const
{
	if (InTag.MatchesTagExact(XYSGameplayTags::CharacterState_Movement_Crouching))
	{
		ensureAlwaysMsgf(TagCount <= 1, TEXT("Tag: CharacterState_Movement_Crouching tag is more then one!"));
		if (TagCount == 1 || TagCount == 0)
		{
			UE_LOG(LogAbilitySystemComponent, Warning, TEXT("Player is enter crouching"));
			if (AXYSCharacter* XYSCharacter = Cast<AXYSCharacter>(GetAvatarActor()))
			{
				if (XYSCharacter->IsLocallyControlled())
				{
					XYSCharacter->StartFPMeshCrouchStateChange();
				}
			}
		}
	}
	
}

FActiveGameplayEffectHandle UXYSAbilitySystemComponent::AddDynamicTagGameplayEffect(UPARAM(Categories=("CharacterState.Movement")) FGameplayTag Tag)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle;
	const TSubclassOf<UGameplayEffect> DynamicTagGE = UXYSAssetManager::GetSubclass(UXYSGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogXYSAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to find DynamicTagGameplayEffect [%s]."), *UXYSGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return ActiveGameplayEffectHandle;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DynamicTagGE, 1.0f, MakeEffectContext());
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	if (!Spec)
	{
		UE_LOG(LogXYSAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to make outgoing spec for [%s]."), *GetNameSafe(DynamicTagGE));
		return ActiveGameplayEffectHandle;
	}

	Spec->DynamicGrantedTags.AddTag(Tag);

	ActiveGameplayEffectHandle = ApplyGameplayEffectSpecToSelf(*Spec);
	return ActiveGameplayEffectHandle;
}

void UXYSAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	const TSubclassOf<UGameplayEffect> DynamicTagGE = UXYSAssetManager::GetSubclass(UXYSGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogXYSAbilitySystem, Warning, TEXT("RemoveDynamicTagGameplayEffect: Unable to find gameplay effect [%s]."), *UXYSGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return;
	}

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(Tag));
	Query.EffectDefinition = DynamicTagGE;

	RemoveActiveEffects(Query);
}

void UXYSAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
	
	// We don't support UGameplayAbility::bReplicateInputDirectly.(提高性能，不需要每个按键都同步)
	// Use replicated events instead so that the WaitInputPress ability task works.(广播消息给Server才能让WaitInputPress task生效)
	if (Spec.IsActive())
	{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
PRAGMA_ENABLE_DEPRECATION_WARNINGS
		
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
	}
}

void UXYSAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.(提高性能，不需要每个按键都同步)
	// Use replicated events instead so that the WaitInputPress ability task works.(广播消息给Server才能让WaitInputPress task生效)
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
	}
}

void UXYSAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// Notify all abilities that a new pawn avatar has been set
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
						ensureMsgf(AbilitySpec.Ability && AbilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced, TEXT("InitAbilityActorInfo: All Abilities should be Instanced (NonInstanced is being deprecated due to usability issues)."));
			PRAGMA_ENABLE_DEPRECATION_WARNINGS
	
						TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			for (UGameplayAbility* AbilityInstance : Instances)
			{
				UXYSGameplayAbility* XYSAbilityInstance = Cast<UXYSGameplayAbility>(AbilityInstance);
				if (XYSAbilityInstance)
				{
					// Ability instances may be missing for replays
					XYSAbilityInstance->OnPawnAvatarSet();
				}
			}
		}

		// Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.
		if (UXYSGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UXYSGlobalAbilitySystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(this);
		}

		if (UXYSAnimInstance* XYSAnimInst = Cast<UXYSAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			XYSAnimInst->InitializeWithAbilitySystem(this);
		}

		TryActivateAbilitiesOnSpawn();
	}
}

void UXYSAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	if (UXYSGameplayAbility* XYSAbility = Cast<UXYSGameplayAbility>(Ability))
	{
		AddAbilityToActivationGroup(XYSAbility->GetActivationGroup(), XYSAbility);
	}
}

void UXYSAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	if (APawn* Avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (!Avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
		{
			ClientNotifyAbilityFailed(Ability, FailureReason);
			return;
		}
	}

	HandleAbilityFailed(Ability, FailureReason);
}

void UXYSAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	if (UXYSGameplayAbility* XYSAbility = Cast<UXYSGameplayAbility>(Ability))
	{
		RemoveAbilityFromActivationGroup(XYSAbility->GetActivationGroup(), XYSAbility);
	}
}

void UXYSAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags,
	UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags,
	bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	if (TagRelationshipMapping)
	{
		// Use the mapping to expand the ability tags into block and cancel tag
		TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
	}

	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, ModifiedBlockTags, bExecuteCancelTags, ModifiedCancelTags);

	//@TODO: Apply any special logic like blocking input or movement
}

void UXYSAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags,
	UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
{
	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);

	//@TODO: Apply any special logic like blocking input or movement
}

void UXYSAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const UXYSGameplayAbility* XYSAbilityCDO = Cast<UXYSGameplayAbility>(AbilitySpec.Ability))
		{
			XYSAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}

void UXYSAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		UXYSGameplayAbility* XYSAbilityCDO = Cast<UXYSGameplayAbility>(AbilitySpec.Ability);
		if (!XYSAbilityCDO)
		{
			UE_LOG(LogXYSAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Non-XYSGameplayAbility %s was Granted to ASC. Skipping."), *AbilitySpec.Ability.GetName());
			continue;
		}

		PRAGMA_DISABLE_DEPRECATION_WARNINGS
				ensureMsgf(AbilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced, TEXT("CancelAbilitiesByFunc: All Abilities should be Instanced (NonInstanced is being deprecated due to usability issues)."));
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
			
		// Cancel all the spawned instances.
		TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
		for (UGameplayAbility* AbilityInstance : Instances)
		{
			UXYSGameplayAbility* XYSAbilityInstance = CastChecked<UXYSGameplayAbility>(AbilityInstance);

			if (ShouldCancelFunc(XYSAbilityInstance, AbilitySpec.Handle))
			{
				if (XYSAbilityInstance->CanBeCanceled())
				{
					XYSAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), XYSAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
				}
				else
				{
					UE_LOG(LogXYSAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *XYSAbilityInstance->GetName());
				}
			}
		}
	}
}

void UXYSAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	HandleAbilityFailed(Ability, FailureReason);
}

void UXYSAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability,
	const FGameplayTagContainer& FailureReason)
{
	UE_LOG(LogXYSAbilitySystem, Warning, TEXT("Ability %s failed to activate (tags: %s)"), *GetPathNameSafe(Ability), *FailureReason.ToString());

	if (const UXYSGameplayAbility* XYSAbility = Cast<const UXYSGameplayAbility>(Ability))
	{
		XYSAbility->OnAbilityFailedToActivate(FailureReason);
	}
}

















