// XiaoYao copyright.


#include "Controller/FrontendPlayerController.h"

#include "Kismet/GameplayStatics.h"

void AFrontendPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FoundCamera;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Default"), FoundCamera);

	check(FoundCamera.Num() > 0);

	SetViewTarget(FoundCamera[0]);
}
