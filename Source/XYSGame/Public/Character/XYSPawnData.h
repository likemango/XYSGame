// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "XYSPawnData.generated.h"

class UXYSInputConfig;
class UXYSAbilityTagRelationshipMapping;
class UXYSAbilitySet;
/**
 * Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "XYS Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class XYSGAME_API UXYSPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UXYSPawnData(const FObjectInitializer& ObjectInitializer);

	// Class to instantiate for this pawn (should usually derive from ALyraPawn or ALyraCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TObjectPtr<UXYSAbilitySet>> AbilitySets;
	
	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityTagRelationship")
	TObjectPtr<UXYSAbilityTagRelationshipMapping> TagRelationshipMapping;
	
	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Input")
	TObjectPtr<UXYSInputConfig> InputConfig;

};
