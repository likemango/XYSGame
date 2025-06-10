// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInv_HUDWidget;
/**
 * 
 */
UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInv_PlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	void CreateHUDWidget();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;
	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;
	
};
