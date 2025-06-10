// XiaoYao copyright.


#include "Controller/Inv_PlayerController.h"

#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

AInv_PlayerController::AInv_PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogInventory, Log, TEXT("Using Inv_PlayerController"));
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	CreateHUDWidget();
}

void AInv_PlayerController::CreateHUDWidget()
{
	if (IsLocalController())
	{
		HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
		if (IsValid(HUDWidget))
		{
			HUDWidget->AddToViewport();
		}
	}
}

