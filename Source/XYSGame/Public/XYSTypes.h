// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "XYSTypes.generated.h"


class UMetaSoundSource;

UENUM(BlueprintType)
enum class EMovementSoundTypes : uint8 {
	Walk_Left			UMETA(DisplayName="Walk_Left"),
	Walk_Right			UMETA(DisplayName="Walk_Right"),
	Sprint_Left			UMETA(DisplayName="Sprint_Left"),
	Sprint_Right		UMETA(DisplayName="Sprint_Right"),
	Crouch_Left			UMETA(DisplayName="Crouch_Left"),
	Crouch_Right		UMETA(DisplayName="Crouch_Right"),
	Jump				UMETA(DisplayName="Jump"),
	Land				UMETA(DisplayName="Land"),
};

/*
 * Structure describing the content of a Movement Sound.
 * */
USTRUCT(BlueprintType)
struct FStructMovementSound : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Movement Sound")
	EMovementSoundTypes Type = EMovementSoundTypes::Walk_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Movement Sound")
	UMetaSoundSource* MetaSoundSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Movement Sound")
	TEnumAsByte<EPhysicalSurface> Surface = SurfaceType1;
};