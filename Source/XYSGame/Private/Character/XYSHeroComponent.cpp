// XiaoYao copyright.


#include "Character/XYSHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "XYSGameplayTags.h"
#include "XYSLogChannels.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Camera/XYSCameraComponent.h"
#include "Camera/XYSCameraMode.h"
#include "Character/XYSCharacter.h"
#include "Character/XYSPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/XYSPlayerController.h"
#include "Input/XYSInputComponent.h"
#include "Misc/UObjectToken.h"
#include "Player/XYSPlayerState.h"

const FName UXYSHeroComponent::NAME_ActorFeatureName("Hero");
const FName UXYSHeroComponent::NAME_BindInputsNow("BindInputsNow");

UXYSHeroComponent::UXYSHeroComponent(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

bool UXYSHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UXYSHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogXYSGame, Error, TEXT("[UXYSHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("XYSHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("XYSHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		RegisterInitStateFeature();
	}
}

void UXYSHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	// 只监听PawnExtension的feature初始化装填，每当它的初始化状态改变时，调用OnActorInitStateChanged
	BindOnActorInitStateChanged(UXYSPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(XYSGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UXYSHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void UXYSHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { XYSGameplayTags::InitState_Spawned, XYSGameplayTags::InitState_DataAvailable, XYSGameplayTags::InitState_DataInitialized, XYSGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UXYSHeroComponent::SetAbilityCameraMode(TSubclassOf<UXYSCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UXYSHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

void UXYSHeroComponent::AddAdditionalInputConfig(const UXYSInputConfig* InputConfig)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UXYSInputComponent* XYSIC = Pawn->FindComponentByClass<UXYSInputComponent>();
		if (ensureMsgf(XYSIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UXYSInputComponent or a subclass of it.")))
		{
			XYSIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ AdditionalBindHandles);
		}
	}
}

void UXYSHeroComponent::RemoveAdditionalInputConfig(const UXYSInputConfig* InputConfig)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UXYSInputComponent* XYSIC = Pawn->FindComponentByClass<UXYSInputComponent>();
		if (ensureMsgf(XYSIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UXYSInputComponent or a subclass of it.")))
		{
			XYSIC->RemoveBinds(AdditionalBindHandles);
		}
	}
}

bool UXYSHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == XYSGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == XYSGameplayTags::InitState_Spawned && DesiredState == XYSGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AXYSPlayerState>())
		{
			return false;
		}
		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		// 如果是server或者本地控制，那么需要controller和playerState都已经注册，并且确定从属关系
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AXYSPlayerController* XYSPC = GetController<AXYSPlayerController>();

			// The input component and local player is required when locally controlled.
			// 如果是本地控制，那么需要LocalPlayer和InputComponent都已经准备好
			
			if (!Pawn->InputComponent || !XYSPC || !XYSPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	// HeroComponent要做的事情已经完成，接下来等待PawnExtension的更新通知
	else if (CurrentState == XYSGameplayTags::InitState_DataAvailable && DesiredState == XYSGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		// 等待PawnExtension已经达到InitState_DataInitialized
		AXYSPlayerState* XYSPS = GetPlayerState<AXYSPlayerState>();

		return XYSPS && Manager->HasFeatureReachedInitState(Pawn, UXYSPawnExtensionComponent::NAME_ActorFeatureName, XYSGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == XYSGameplayTags::InitState_DataInitialized && DesiredState == XYSGameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UXYSHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState)
{
	if (CurrentState == XYSGameplayTags::InitState_Spawned && DesiredState == XYSGameplayTags::InitState_DataAvailable)
	{
		// do nothing
		return;
	}
	if (CurrentState == XYSGameplayTags::InitState_DataAvailable && DesiredState == XYSGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AXYSPlayerState* XYSPS = GetPlayerState<AXYSPlayerState>();
		if (!ensure(Pawn && XYSPS))
		{
			return;
		}
		
		const UXYSPawnData* PawnData = nullptr;

		if (UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UXYSPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(XYSPS->GetXYSAbilitySystemComponent(), XYSPS);
		}

		if (AXYSPlayerController* XYSPC = GetController<AXYSPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (UXYSCameraComponent* CameraComponent = UXYSCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
	if (CurrentState == XYSGameplayTags::InitState_DataInitialized && DesiredState == XYSGameplayTags::InitState_GameplayReady)
	{
		// do nothing
		return;
	}
}

void UXYSHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UXYSPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == XYSGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			// 因为我自身在等待PawnExtension的DataInitialized( HasFeatureReachedInitState )，以此我需要在这里接收消息，来提醒我可以进行后续状态
			CheckDefaultInitialization();
		}
	}
}


void UXYSHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(LocalPlayerSubsystem);

	LocalPlayerSubsystem->ClearAllMappings();

	if (const UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UXYSPawnData* PawnData = PawnExtComp->GetPawnData<UXYSPawnData>())
		{
			if (const UXYSInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						// Actually add the config to the local player							
						LocalPlayerSubsystem->AddMappingContext(IMC, Mapping.Priority, Options);
					}
				}

				UXYSInputComponent* XYSInputComponent = Cast<UXYSInputComponent>(PlayerInputComponent);
				if (ensureAlwaysMsgf(XYSInputComponent, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UXYSInputComponent or a subclass of it.")))
				{
					// add custom key mapping
					XYSInputComponent->AddInputMappings(InputConfig, LocalPlayerSubsystem);

					// bind InputAction with InputTag, so ability can be triggered by: InputAction --> InputTag --> Ability
					XYSInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BaseBindHandles);

					XYSInputComponent->BindNativeAction(InputConfig, XYSGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					XYSInputComponent->BindNativeAction(InputConfig, XYSGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
					// XYSInputComponent->BindNativeAction(InputConfig, XYSGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, false);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UXYSHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UXYSAbilitySystemComponent* XYSASC = PawnExtComp->GetXYSAbilitySystemComponent())
			{
				XYSASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UXYSHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UXYSAbilitySystemComponent* XYSASC = PawnExtComp->GetXYSAbilitySystemComponent())
		{
			XYSASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UXYSHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	if (AXYSCharacter* Character = GetPawn<AXYSCharacter>())
	{
		Character->Input_Move(InputActionValue);
	}
}

void UXYSHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	if (AXYSCharacter* Character = GetPawn<AXYSCharacter>())
	{
		Character->Input_Look(InputActionValue);
	}
}

TSubclassOf<UXYSCameraMode> UXYSHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UXYSPawnExtensionComponent* PawnExtComp = UXYSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UXYSPawnData* PawnData = PawnExtComp->GetPawnData<UXYSPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

/*
void UXYSHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AXYSCharacter* Character = GetPawn<AXYSCharacter>())
	{
		Character->Input_Crouch(InputActionValue);
	}
}
*/























