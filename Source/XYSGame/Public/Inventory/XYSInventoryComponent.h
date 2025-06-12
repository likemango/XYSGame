// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "XYSInventoryComponent.generated.h"


class UXYSInventoryComponent;
class UXYSInventoryItemDefinition;
struct FXYSInventoryList;
class UXYSInventoryItemInstance;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FXYSInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UXYSInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

USTRUCT(BlueprintType)
struct FXYSInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FXYSInventoryEntry()
	{
	}

	FString GetDebugString() const;

private:
	friend FXYSInventoryList;
	friend UXYSInventoryComponent;

	UPROPERTY()
	TObjectPtr<UXYSInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FXYSInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FXYSInventoryList() : OwnerComponent(nullptr)
	{
	}

	FXYSInventoryList(UActorComponent* OwnerComponent) : OwnerComponent(OwnerComponent)
	{
	}

	TArray<UXYSInventoryItemInstance*> GetAllItems() const;

	//~FastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FXYSInventoryEntry, FXYSInventoryList>(
			Entries, DeltaParms, *this);
	}

	UXYSInventoryItemInstance* AddEntry(TSubclassOf<UXYSInventoryItemDefinition> ItemDefClass, int32 StackCount);
	void AddEntry(UXYSInventoryItemInstance* Instance);

	void RemoveEntry(UXYSInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(const FXYSInventoryEntry& Entry, int32 OldCount, int32 NewCount) const;

private:
	friend UXYSInventoryComponent;

	UPROPERTY()
	TArray<FXYSInventoryEntry> Entries;
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FXYSInventoryList> : public TStructOpsTypeTraitsBase2<FXYSInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};



UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class XYSGAME_API UXYSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UXYSInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool CanAddItemDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UXYSInventoryItemInstance* AddItemDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef,int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UXYSInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UXYSInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UXYSInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UXYSInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef) const;

	int32 GetTotalItemCountByDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UXYSInventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface

	//~ActorComponent
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//

private:
	UPROPERTY(Replicated)
	FXYSInventoryList InventoryList;
};
