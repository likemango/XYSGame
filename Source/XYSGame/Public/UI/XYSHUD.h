// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XYSHUD.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class XYSGAME_API AXYSHUD : public AHUD
{
	GENERATED_BODY()

public:
	AXYSHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UObject interface
	virtual void PreInitializeComponents() override;
	//~End of UObject interface

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AHUD interface
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
	//~End of AHUD interface
};
