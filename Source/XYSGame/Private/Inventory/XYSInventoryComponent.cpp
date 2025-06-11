// XiaoYao copyright.


#include "Inventory/XYSInventoryComponent.h"

#include "XYSGameplayTags.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Inventory/XYSInventoryItemDefinition.h"
#include "Inventory/XYSInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////
/// FXYSInventoryEntry
FString FXYSInventoryEntry::GetDebugString() const
{
	TSubclassOf<UXYSInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	FString Msg = FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
	// UE_LOG(LogXYSInventory, Log, TEXT("%s"), *Msg);
	return Msg;
}

//////////////////////////////////
/// FXYSInventoryList
TArray<UXYSInventoryItemInstance*> FXYSInventoryList::GetAllItems() const
{
	TArray<UXYSInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FXYSInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

void FXYSInventoryList::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FXYSInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FXYSInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FXYSInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FXYSInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FXYSInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

UXYSInventoryItemInstance* FXYSInventoryList::AddEntry(TSubclassOf<UXYSInventoryItemDefinition> ItemDefClass, int32 StackCount)
{
	UXYSInventoryItemInstance* Result = nullptr;

	check(ItemDefClass != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FXYSInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UXYSInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDefClass);
	for (UXYSInventoryItemFragment* Fragment : GetDefault<UXYSInventoryItemDefinition>(ItemDefClass)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const UXYSInventoryItemDefinition* ItemCDO = GetDefault<UXYSInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FXYSInventoryList::AddEntry(UXYSInventoryItemInstance* Instance)
{
	unimplemented();
}

void FXYSInventoryList::RemoveEntry(UXYSInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FXYSInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FXYSInventoryList::BroadcastChangeMessage(const FXYSInventoryEntry& Entry, int32 OldCount, int32 NewCount) const
{
	FXYSInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(XYSGameplayTags::TAG_XYS_Inventory_Message_StackChanged, Message);
}


//////////////////////////////////
/// UXYSInventoryComponent
UXYSInventoryComponent::UXYSInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UXYSInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool UXYSInventoryComponent::CanAddItemDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UXYSInventoryItemInstance* UXYSInventoryComponent::AddItemDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef,int32 StackCount)
{
	UXYSInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UXYSInventoryComponent::AddItemInstance(UXYSInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UXYSInventoryComponent::RemoveItemInstance(UXYSInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);
	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UXYSInventoryItemInstance*> UXYSInventoryComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UXYSInventoryItemInstance* UXYSInventoryComponent::FindFirstItemStackByDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef) const
{
	for (const FXYSInventoryEntry& Entry : InventoryList.Entries)
	{
		UXYSInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UXYSInventoryComponent::GetTotalItemCountByDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FXYSInventoryEntry& Entry : InventoryList.Entries)
	{
		UXYSInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}
	return TotalCount;
}

bool UXYSInventoryComponent::ConsumeItemsByDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef,int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UXYSInventoryItemInstance* Instance = FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

void UXYSInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing UXYSInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FXYSInventoryEntry& Entry : InventoryList.Entries)
		{
			UXYSInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

bool UXYSInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FXYSInventoryEntry& Entry : InventoryList.Entries)
	{
		UXYSInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

//////////////////////////////////

