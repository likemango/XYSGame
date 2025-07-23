// XiaoYao copyright.


#include "Player/XYSCheatManager.h"

#include "GameFramework/Pawn.h"
#include "Player/XYSPlayerController.h"
#include "Player/XYSDebugCameraController.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Console.h"
#include "GameFramework/HUD.h"
#include "System/XYSAssetManager.h"
#include "System/XYSGameData.h"
#include "XYSGameplayTags.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Character/XYSHealthComponent.h"
#include "Character/XYSPawnExtensionComponent.h"
#include "System/XYSSystemStatics.h"
#include "Development/XYSDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(XYSCheatManager)

DEFINE_LOG_CATEGORY(LogXYSCheat);

namespace XYSCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	
	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling(
		TEXT("XYSCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode(
		TEXT("XYSCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat);
};


UXYSCheatManager::UXYSCheatManager()
{
	DebugCameraControllerClass = AXYSDebugCameraController::StaticClass();
}

void UXYSCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

#if WITH_EDITOR
	if (GIsEditor)
	{
		APlayerController* PC = GetOuterAPlayerController();
		for (const FXYSCheatToRun& CheatRow : GetDefault<UXYSDeveloperSettings>()->CheatsToRun)
		{
			if (CheatRow.Phase == ECheatExecutionTime::OnCheatManagerCreated)
			{
				PC->ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
			}
		}
	}
#endif

	if (XYSCheat::bStartInGodMode)
	{
		God();	
	}
}

void UXYSCheatManager::CheatOutputText(const FString& TextToOutput)
{
#if USING_CHEAT_MANAGER
	// Output to the console.
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportConsole)
	{
		GEngine->GameViewport->ViewportConsole->OutputText(TextToOutput);
	}

	// Output to log.
	UE_LOG(LogXYSCheat, Display, TEXT("%s"), *TextToOutput);
#endif // USING_CHEAT_MANAGER
}

void UXYSCheatManager::Cheat(const FString& Msg)
{
	if (AXYSPlayerController* XYSPC = Cast<AXYSPlayerController>(GetOuterAPlayerController()))
	{
		XYSPC->ServerCheat(Msg.Left(128));
	}
}

void UXYSCheatManager::CheatAll(const FString& Msg)
{
	if (AXYSPlayerController* XYSPC = Cast<AXYSPlayerController>(GetOuterAPlayerController()))
	{
		XYSPC->ServerCheatAll(Msg.Left(128));
	}
}

void UXYSCheatManager::PlayNextGame()
{
	UXYSSystemStatics::PlayNextGame(this);
}

void UXYSCheatManager::EnableDebugCamera()
{
	Super::EnableDebugCamera();
}

void UXYSCheatManager::DisableDebugCamera()
{
	FVector DebugCameraLocation;
	FRotator DebugCameraRotation;

	ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* OriginalPC = nullptr;

	if (DebugCC)
	{
		OriginalPC = DebugCC->OriginalControllerRef;
		DebugCC->GetPlayerViewPoint(DebugCameraLocation, DebugCameraRotation);
	}

	Super::DisableDebugCamera();

	if (OriginalPC && OriginalPC->PlayerCameraManager && (OriginalPC->PlayerCameraManager->CameraStyle == XYSCheat::NAME_Fixed))
	{
		OriginalPC->SetInitialLocationAndRotation(DebugCameraLocation, DebugCameraRotation);

		OriginalPC->PlayerCameraManager->ViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->ViewTarget.POV.Rotation = DebugCameraRotation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Rotation = DebugCameraRotation;
	}
}

bool UXYSCheatManager::InDebugCamera() const
{
	return (Cast<ADebugCameraController>(GetOuter()) ? true : false);
}

void UXYSCheatManager::EnableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(XYSCheat::NAME_Fixed);
	}
}

void UXYSCheatManager::DisableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(NAME_Default);
	}
}

bool UXYSCheatManager::InFixedCamera() const
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	const APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		return (PC->PlayerCameraManager->CameraStyle == XYSCheat::NAME_Fixed);
	}

	return false;
}

void UXYSCheatManager::ToggleFixedCamera()
{
	if (InFixedCamera())
	{
		DisableFixedCamera();
	}
	else
	{
		EnableFixedCamera();
	}
}

void UXYSCheatManager::CycleDebugCameras()
{
	if (!XYSCheat::bEnableDebugCameraCycling)
	{
		return;
	}
	
	if (InDebugCamera())
	{
		EnableFixedCamera();
		DisableDebugCamera();
	}
	else if (InFixedCamera())
	{
		DisableFixedCamera();
		DisableDebugCamera();
	}
	else
	{
		EnableDebugCamera();
		DisableFixedCamera();
	}
}

void UXYSCheatManager::CycleAbilitySystemDebug()
{
	APlayerController* PC = Cast<APlayerController>(GetOuterAPlayerController());

	if (PC && PC->MyHUD)
	{
		if (!PC->MyHUD->bShowDebugInfo || !PC->MyHUD->DebugDisplay.Contains(TEXT("AbilitySystem")))
		{
			PC->MyHUD->ShowDebug(TEXT("AbilitySystem"));
		}

		PC->ConsoleCommand(TEXT("AbilitySystem.Debug.NextCategory"));
	}
}

void UXYSCheatManager::CancelActivatedAbilities()
{
	if (UXYSAbilitySystemComponent* XYSASC = GetPlayerAbilitySystemComponent())
	{
		const bool bReplicateCancelAbility = true;
		XYSASC->CancelInputActivatedAbilities(bReplicateCancelAbility);
	}
}

void UXYSCheatManager::AddTagToSelf(FString TagName)
{
	FGameplayTag Tag = XYSGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UXYSAbilitySystemComponent* XYSASC = GetPlayerAbilitySystemComponent())
		{
			XYSASC->AddDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogXYSCheat, Display, TEXT("AddTagToSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UXYSCheatManager::RemoveTagFromSelf(FString TagName)
{
	FGameplayTag Tag = XYSGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UXYSAbilitySystemComponent* XYSASC = GetPlayerAbilitySystemComponent())
		{
			XYSASC->RemoveDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogXYSCheat, Display, TEXT("RemoveTagFromSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UXYSCheatManager::DamageSelf(float DamageAmount)
{
	if (UXYSAbilitySystemComponent* XYSASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerDamage(XYSASC, DamageAmount);
	}
}

void UXYSCheatManager::DamageTarget(float DamageAmount)
{
	if (AXYSPlayerController* XYSPC = Cast<AXYSPlayerController>(GetOuterAPlayerController()))
	{
		if (XYSPC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			XYSPC->ServerCheat(FString::Printf(TEXT("DamageTarget %.2f"), DamageAmount));
			return;
		}

		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(XYSPC, TargetHitResult);

		if (UXYSAbilitySystemComponent* XYSTargetASC = Cast<UXYSAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerDamage(XYSTargetASC, DamageAmount);
		}
	}
}

void UXYSCheatManager::ApplySetByCallerDamage(UXYSAbilitySystemComponent* XYSASC, float DamageAmount)
{
	check(XYSASC);

	TSubclassOf<UGameplayEffect> DamageGE = UXYSAssetManager::GetSubclass(UXYSGameData::Get().DamageGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = XYSASC->MakeOutgoingSpec(DamageGE, 1.0f, XYSASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(XYSGameplayTags::SetByCaller_Damage, DamageAmount);
		XYSASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UXYSCheatManager::HealSelf(float HealAmount)
{
	if (UXYSAbilitySystemComponent* XYSASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerHeal(XYSASC, HealAmount);
	}
}

void UXYSCheatManager::HealTarget(float HealAmount)
{
	if (AXYSPlayerController* XYSPC = Cast<AXYSPlayerController>(GetOuterAPlayerController()))
	{
		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(XYSPC, TargetHitResult);

		if (UXYSAbilitySystemComponent* XYSTargetASC = Cast<UXYSAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerHeal(XYSTargetASC, HealAmount);
		}
	}
}

void UXYSCheatManager::ApplySetByCallerHeal(UXYSAbilitySystemComponent* XYSASC, float HealAmount)
{
	check(XYSASC);

	TSubclassOf<UGameplayEffect> HealGE = UXYSAssetManager::GetSubclass(UXYSGameData::Get().HealGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = XYSASC->MakeOutgoingSpec(HealGE, 1.0f, XYSASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(XYSGameplayTags::SetByCaller_Heal, HealAmount);
		XYSASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

UXYSAbilitySystemComponent* UXYSCheatManager::GetPlayerAbilitySystemComponent() const
{
	if (AXYSPlayerController* XYSPC = Cast<AXYSPlayerController>(GetOuterAPlayerController()))
	{
		return XYSPC->GetXYSAbilitySystemComponent();
	}
	return nullptr;
}

void UXYSCheatManager::DamageSelfDestruct()
{
	if (AXYSPlayerController* XYSPC = Cast<AXYSPlayerController>(GetOuterAPlayerController()))
	{
 		if (const UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(XYSPC->GetPawn()))
		{
			if (PawnExtComp->HasReachedInitState(XYSGameplayTags::InitState_GameplayReady))
			{
				if (UXYSHealthComponent* HealthComponent = UXYSHealthComponent::FindHealthComponent(XYSPC->GetPawn()))
				{
					HealthComponent->DamageSelfDestruct();
				}
			}
		}
	}
}

void UXYSCheatManager::God()
{
	if (AXYSPlayerController* XYSPC = Cast<AXYSPlayerController>(GetOuterAPlayerController()))
	{
		if (XYSPC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			XYSPC->ServerCheat(FString::Printf(TEXT("God")));
			return;
		}

		if (UXYSAbilitySystemComponent* XYSASC = XYSPC->GetXYSAbilitySystemComponent())
		{
			const FGameplayTag Tag = XYSGameplayTags::Cheat_GodMode;
			const bool bHasTag = XYSASC->HasMatchingGameplayTag(Tag);

			if (bHasTag)
			{
				XYSASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				XYSASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

void UXYSCheatManager::UnlimitedHealth(int32 Enabled)
{
	if (UXYSAbilitySystemComponent* XYSASC = GetPlayerAbilitySystemComponent())
	{
		const FGameplayTag Tag = XYSGameplayTags::Cheat_UnlimitedHealth;
		const bool bHasTag = XYSASC->HasMatchingGameplayTag(Tag);

		if ((Enabled == -1) || ((Enabled > 0) && !bHasTag) || ((Enabled == 0) && bHasTag))
		{
			if (bHasTag)
			{
				XYSASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				XYSASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

