// XiaoYao copyright.


#include "Equipment/XYSEquipmentComponent.h"

#include "AbilitySystemGlobals.h"
#include "XYSLogChannels.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
#include "Equipment/XYSEquipmentDefinition.h"
#include "Equipment/XYSEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////
/// FXYSAppliedEquipmentEntry
FString FXYSAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
/// FXYSEquipmentList
void FXYSEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
 	for (int32 Index : RemovedIndices)
 	{
 		const FXYSAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
 	}
}

void FXYSEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FXYSAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FXYSEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	// for (int32 Index : ChangedIndices)
	// {
	// 	const FGameplayTagStack& Stack = Stacks[Index];
	// 	TagToCountMap[Stack.Tag] = Stack.StackCount;
	// }
}

UXYSAbilitySystemComponent* FXYSEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UXYSAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UXYSEquipmentInstance* FXYSEquipmentList::AddEntry(const TSubclassOf<UXYSEquipmentDefinition>& EquipmentDefinition)
{
	UXYSEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
 	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UXYSEquipmentDefinition* EquipmentCDO = GetDefault<UXYSEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UXYSEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UXYSEquipmentInstance::StaticClass();
	}
	
	FXYSAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UXYSEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (UXYSAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const UXYSAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		UE_LOG(LogXYSInventory, Warning, TEXT("Equip a new equipment but can't find owner's AbilitySystemComponent"))
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	MarkItemDirty(NewEntry);
	return Result;
}

void FXYSEquipmentList::RemoveEntry(UXYSEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FXYSAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UXYSAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
//////////////////////////////////////////////////////////////////////
/// UXYSEquipmentComponent

UXYSEquipmentComponent::UXYSEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UXYSEquipmentComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UXYSEquipmentInstance* UXYSEquipmentComponent::EquipItem(TSubclassOf<UXYSEquipmentDefinition> EquipmentClass)
{
	UXYSEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UXYSEquipmentComponent::UnequipItem(UXYSEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool UXYSEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FXYSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UXYSEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UXYSEquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UXYSEquipmentComponent::UninitializeComponent()
{
	TArray<UXYSEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FXYSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UXYSEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UXYSEquipmentComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing XYSEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FXYSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UXYSEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UXYSEquipmentInstance* UXYSEquipmentComponent::GetFirstInstanceOfType(TSubclassOf<UXYSEquipmentInstance> InstanceType)
{
	for (FXYSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UXYSEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UXYSEquipmentInstance*> UXYSEquipmentComponent::GetEquipmentInstancesOfType(TSubclassOf<UXYSEquipmentInstance> InstanceType) const
{
	TArray<UXYSEquipmentInstance*> Results;
	for (const FXYSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UXYSEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

//////////////////////////////////////////////////////////////////////
