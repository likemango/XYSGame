// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/XYSAbilitySet.h"
#include "Components/ActorComponent.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "XYSEquipmentComponent.generated.h"

class UXYSEquipmentDefinition;
class UXYSEquipmentInstance;
struct FXYSEquipmentList;
class UXYSEquipmentComponent;


/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FXYSAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FXYSAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FXYSEquipmentList;
	friend UXYSEquipmentComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UXYSEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UXYSEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FXYSAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FXYSEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FXYSEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FXYSEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FXYSAppliedEquipmentEntry, FXYSEquipmentList>(Entries, DeltaParms, *this);
	}

	UXYSEquipmentInstance* AddEntry(const TSubclassOf<UXYSEquipmentDefinition>& EquipmentDefinition);
	void RemoveEntry(UXYSEquipmentInstance* Instance);

private:
	UXYSAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UXYSEquipmentComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FXYSAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FXYSEquipmentList> : public TStructOpsTypeTraitsBase2<FXYSEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};


UCLASS(BlueprintType, Const)
class XYSGAME_API UXYSEquipmentComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UXYSEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UXYSEquipmentInstance* EquipItem(TSubclassOf<UXYSEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UXYSEquipmentInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UXYSEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UXYSEquipmentInstance> InstanceType);

	/** Returns all equipped instances of a given type, or an empty array if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UXYSEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UXYSEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FXYSEquipmentList EquipmentList;
};
