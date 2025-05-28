// Copyright Vinipi Studios 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/EngineTypes.h"
#include "InteractableData.generated.h"

class UInteractionKey;

#define CIP_DEFAULT_KEY_PATH TEXT("/CustomizableInteractionPlugin/Blueprint/Keys/B_CIP_SimpleInteractionKey.B_CIP_SimpleInteractionKey")

UENUM(BlueprintType, meta = (ScriptName="Damage_Type"))
enum class EInteractionType : uint8
{
	EIT_Press UMETA(DisplayName="Press"),
	EIT_Hold UMETA(DisplayName="Hold")
};

/**
 * Used to define info about the Interactable.
 */
USTRUCT(BlueprintType)
struct FInteractableData : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	FInteractableData();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Widget")
	FText DisplayText = FText::FromString("Placeholder Text");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Widget")
	FText TooltipText = FText::FromString("Placeholder Text");

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Detection")
	float MaxHoverDistanceFromCharacter = 350.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Detection")
	float MaxHoverDistanceFromScreenCenter = 50.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Detection")
	float MaxOverlapDistance = 700.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Detection")
	bool bDetectWhenObstructed = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Detection")
	bool bAllowInteractionWhenObstructed = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Detection")
	TEnumAsByte<ECollisionChannel> ObstructionTraceChannel = ECC_Visibility;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	TSoftObjectPtr<UInteractionKey> InteractionKey;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	EInteractionType InteractionType = EInteractionType::EIT_Hold;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	float InteractionDuration = 1.f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	float DelayBetweenInteraction = 0.5f;
};
