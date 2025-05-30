// XiaoYao copyright.


#include "GameModes/XYSGameMode.h"

#include "XYSLogChannels.h"
#include "Character/XYSPawnData.h"
#include "Character/XYSPawnExtensionComponent.h"

AXYSGameMode::AXYSGameMode(const FObjectInitializer& ObjectInitializer)
{
	
}

UClass* AXYSGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (PawnData)
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AXYSGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
                                                                const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				PawnExtComp->SetPawnData(PawnData);
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogXYSGame, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogXYSGame, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}
