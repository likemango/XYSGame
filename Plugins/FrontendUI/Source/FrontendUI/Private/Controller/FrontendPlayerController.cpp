// XiaoYao copyright.


#include "Controller/FrontendPlayerController.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void AFrontendPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FoundCamera;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Default"), FoundCamera);

	check(FoundCamera.Num() > 0);

	SetViewTarget(FoundCamera[0]);

	UFrontendGameUserSettings* GameUserSettings = UFrontendGameUserSettings::Get();

	if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();
	}
}
