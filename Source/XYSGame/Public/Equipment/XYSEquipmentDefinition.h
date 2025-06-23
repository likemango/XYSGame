// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XYSEquipmentDefinition.generated.h"

class UXYSAbilitySet;
class UXYSEquipmentInstance;

USTRUCT()
struct FXYSAttachData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket{};

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform RelativeTransform{};
};

USTRUCT()
struct FXYSEquipmentActorToSpawn
{
	GENERATED_BODY()

	FXYSEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;
	
	UPROPERTY(EditAnywhere, Category=Equipment)
	FXYSAttachData FPAttachData{};

	UPROPERTY(EditAnywhere, Category=Equipment)
	FXYSAttachData TPAttachData{};
};

/**
 * 
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class XYSGAME_API UXYSEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UXYSEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UXYSEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UXYSAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FXYSEquipmentActorToSpawn> ActorsToSpawn;
};
