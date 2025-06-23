// XiaoYao copyright.


#include "Equipment/XYSEquipmentInstance.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Equipment/XYSEquipmentDefinition.h"
#include "Net/UnrealNetwork.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif // UE_WITH_IRIS

#include "Character/XYSCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSEquipmentInstance)

class FLifetimeProperty;
class UClass;
class USceneComponent;

UXYSEquipmentInstance::UXYSEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UXYSEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void UXYSEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActorsAttachData);
}

#if UE_WITH_IRIS
void UXYSEquipmentInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;

	// Build descriptors and allocate PropertyReplicationFragments for this object
	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
#endif // UE_WITH_IRIS

APawn* UXYSEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UXYSEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

TArray<AActor*> UXYSEquipmentInstance::GetSpawnedActors() const
{
	TArray<AActor*> Result;

	for (const FXYSSpawnedActorAttachData& SpawnedActorAttachData : SpawnedActorsAttachData)
	{
		if (SpawnedActorAttachData.SpawnedActor)
		{
			Result.Add(SpawnedActorAttachData.SpawnedActor);
		}
	}
	return Result;
}

void UXYSEquipmentInstance::SpawnEquipmentActors(const TArray<FXYSEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		for (const FXYSEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);

			FXYSSpawnedActorAttachData SpawnedActorToAttach;
			SpawnedActorToAttach.SpawnedActor = NewActor;
			SpawnedActorToAttach.FPAttachData = SpawnInfo.FPAttachData;
			SpawnedActorToAttach.TPAttachData = SpawnInfo.TPAttachData;
			
			SpawnedActorsAttachData.Add(SpawnedActorToAttach);
			OnRep_SpawnedActorsAttachData();
		}
	}
}

void UXYSEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : GetSpawnedActors())
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	SpawnedActorsAttachData.Reset();
}

void UXYSEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UXYSEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

// void UXYSEquipmentInstance::ClientAttachActorToMesh_Implementation(AActor* ActorToAttach, const FXYSAttachData& AttachData)
// {
// 	if (APawn* OwningPawn = GetPawn())
// 	{
// 		if (AXYSCharacter* Char = Cast<AXYSCharacter>(OwningPawn))
// 		{
// 			USceneComponent* AttachTargetFP = Char->GetFPUpperMesh();
// 			ActorToAttach->SetActorRelativeTransform(AttachData.FPRelativeAttachTransform);
// 			ActorToAttach->AttachToComponent(AttachTargetFP, FAttachmentTransformRules(AttachData.FPAttachmentRule, false), AttachData.FPAttachSocket);
// 		}
// 	}
// }

void UXYSEquipmentInstance::OnRep_Instigator()
{
}

void UXYSEquipmentInstance::OnRep_SpawnedActorsAttachData()
{
	if (AXYSCharacter* Character = Cast<AXYSCharacter>(GetTypedPawn(AXYSCharacter::StaticClass())))
	{
		for (const FXYSSpawnedActorAttachData& SpawnedActorAttachData : SpawnedActorsAttachData)
		{
			if (!SpawnedActorAttachData.SpawnedActor)
				continue;
			
			if (Character->IsLocallyControlled())
			{
				USceneComponent* AttachTargetFP = Character->GetFPUpperMesh();
				SpawnedActorAttachData.SpawnedActor->SetActorRelativeTransform(SpawnedActorAttachData.FPAttachData.RelativeTransform);
				SpawnedActorAttachData.SpawnedActor->AttachToComponent(AttachTargetFP, FAttachmentTransformRules::KeepRelativeTransform, SpawnedActorAttachData.FPAttachData.AttachSocket);
			}
			else
			{
				USceneComponent* AttachTargetTP = Character->GetTPMesh();
				SpawnedActorAttachData.SpawnedActor->SetActorRelativeTransform(SpawnedActorAttachData.TPAttachData.RelativeTransform);
				SpawnedActorAttachData.SpawnedActor->AttachToComponent(AttachTargetTP, FAttachmentTransformRules::KeepRelativeTransform, SpawnedActorAttachData.TPAttachData.AttachSocket);
			}
		}
	}
}

