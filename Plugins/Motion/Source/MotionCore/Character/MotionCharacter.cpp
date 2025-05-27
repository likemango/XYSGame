// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.


#include "MotionCore/Character/MotionCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "KismetTraceUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MotionCore/MotionGameplayTags.h"
#include "MotionCore/Camera/MotionCameraComponent.h"
#include "MotionCore/FunctionLibraries/MotionCurveFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "MotionCore/Input/MotionInputComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class ULocalPlayer;
class UWorld;

AMotionCharacter::AMotionCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMotionCharacterMovementComponent>(
		CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Components
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMotionAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	MovementComponent = Cast<UMotionCharacterMovementComponent>(ACharacter::GetMovementComponent());
	MotionCameraComponent = ObjectInitializer.CreateDefaultSubobject<UMotionCameraComponent>(this, TEXT("MotionCameraComponent"));

	// Giving the camera a location on initialization, so the camera preview in editor looks as one would expect. Gets overriden when playing. 
	MotionCameraComponent->SetWorldLocation(FVector(25.0f, 0.0f, 72.0f));

	CameraRotationCurvesManager = ObjectInitializer.CreateDefaultSubobject<UMotionCurveManager>(this, TEXT("CameraRotationCurvesManager"));
	CameraRotationCurvesManager->ComponentTags.Add("CameraRotationCurvesManager");
	CameraLocationCurvesManager = ObjectInitializer.CreateDefaultSubobject<UMotionCurveManager>(this, TEXT("CameraLocationCurvesManager"));
	CameraLocationCurvesManager->ComponentTags.Add("CameraLocationCurvesManager");

	// Set collision settings for collision feeler
	CollisionFeeler = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CollisionFeeler"));
	CollisionFeeler->SetupAttachment(GetCapsuleComponent());
	CollisionFeeler->CanCharacterStepUpOn = ECB_No;
	CollisionFeeler->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionFeeler->SetCollisionProfileName("Pawn");
	CollisionFeeler->SetCapsuleSize(CollisionFeelerSize, CollisionFeelerSize);

	// Not sure why, but the entire collision detection fails if you don't simulate physics after init, and then immediately disable it.
	GetCapsuleComponent()->SetSimulatePhysics(true);
}

void AMotionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Not sure why, but the entire collision detection fails if you don't simulate physics after init, and then immediately disable it.
	UCapsuleComponent* PlayerCapsule = GetCapsuleComponent();
	ensure(PlayerCapsule);
	PlayerCapsule->SetSimulatePhysics(false);

	// Make sure that collision feeler component is actually registered
	if(CollisionFeeler)
	{
		CollisionFeeler->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CollisionFeeler Component is not valid"));
	}

	// Set up linked layers for skeletal meshes
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
	
	for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
	{
		SkeletalMeshComponent->LinkAnimClassLayers(DefaultAnimLinkedLayerForAllSkeletalMeshes);
	}

	// Add loose gameplay tags
	AbilitySystemComponent->AddLooseGameplayTags(GameplayTags);
}

void AMotionCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	// If the character is not being possessed, stop processing.
	if (Controller == nullptr)
		return;

	// Handle Camera Collision
    HandleCameraCollision(DeltaSeconds);

	// Handle Velocity Events
	if(GetMovementComponent()->Velocity.Length() > 0.0f)
	{
		OnVelocityBeginDelegate.Broadcast();
		bHasVelocityEventFired = false;
	}
	else if(!bHasVelocityEventFired)
	{
		OnVelocityStopDelegate.Broadcast();
		bHasVelocityEventFired = true;
	}
}

void AMotionCharacter::HandleCameraCollision(float DeltaSeconds) const
{
    if(AbilitySystemComponent->HasMatchingGameplayTag(MotionGameplayTags::GameplayTag_DisableCameraCollisionCheck))
    {
    	return;
    }
        
        
	UCapsuleComponent* PlayerCapsule = GetCapsuleComponent();
	ensure(PlayerCapsule);

	AController* PlayerController = GetController();
	ensure(PlayerController);

	float CapsuleRadius, CapsuleHeight;
	PlayerCapsule->GetScaledCapsuleSize(CapsuleRadius, CapsuleHeight);

	// Update collision feeler location to be in front of the camera.
	FVector ActorEyesLocation = GetActorLocation() + FVector(0.f,0.f, CapsuleHeight - 24.0f);
	const FRotator ActorRotation = PlayerController->K2_GetActorRotation();
	const FVector CollisionFeelerPosition = (MotionCameraComponent->GetForwardVector() * CollisionFeelerCameraDistance)
		+ ActorEyesLocation
		+ ActorRotation.RotateVector(UMotionCurveFunctionLibrary::GetRuntimeVectorCurveValue(MotionCameraComponent->CameraTargetOffset, ActorRotation.Pitch));

	FHitResult CollisionMoveHitResult;
	CollisionFeeler->SetWorldLocation(CollisionFeelerPosition, true, &CollisionMoveHitResult, ETeleportType::None);

	// Perform a sweep to detect potential collisions
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		CollisionFeelerPosition,
		CollisionFeelerPosition - FVector(0.0f, 0.0f, CollisionFeelerTraceExtraSize),
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(CollisionFeelerSize, CollisionFeelerSize + CollisionFeelerTraceExtraSize),
		CollisionParams
	);

	// Ignore self-collision
	if (!bHit || HitResult.GetActor() == this)
		return;

	// If we *just* started penetrating geometry, we need to account for how the values in HitResult are being used instead.
	// See HitResult.bStartPenetrating for more details.
	FVector TargetLocation;
	if (HitResult.bStartPenetrating)
	{
		TargetLocation = HitResult.Normal * CollisionFeelerPlayerPushbackDistance + PlayerCapsule->GetComponentLocation();
	}
	else
	{
		TargetLocation = (MotionCameraComponent->GetComponentLocation() - HitResult.ImpactPoint).GetSafeNormal(0.0001f) * CollisionFeelerPlayerPushbackDistance + PlayerCapsule->GetComponentLocation();
	}

	// Gradual adjustment for smoother collision response
	FVector CurrentLocation = PlayerCapsule->GetComponentLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaSeconds, CollisionFeelerPlayerPushbackInterpSpeed);

	// Limit the maximum distance moved in one tick to avoid harsh corrections
	if (float MaxMovementPerTick = CollisionFeelerPlayerPushbackDistance * 0.5f; FVector::Dist(CurrentLocation, NewLocation) > MaxMovementPerTick)
	{
		FVector Direction = (NewLocation - CurrentLocation).GetSafeNormal();
		NewLocation = CurrentLocation + Direction * MaxMovementPerTick;
	}

	// Set new location to calculated position
	PlayerCapsule->SetWorldLocation(NewLocation);
}

void AMotionCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AMotionCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMotionCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}

// Called to bind functionality to input
void AMotionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);

	if(DefaultInputMappings.Num() > 0)
	{
		for (const auto Mapping : DefaultInputMappings)
		{
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = false;
			InputSubsystem->AddMappingContext(Mapping, 0, Options);
		}
	}
	
	if (InputConfig)
	{
		if (UMotionInputComponent* MotionInputComponent = Cast<UMotionInputComponent>(PlayerInputComponent); ensureMsgf(MotionInputComponent, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UMotionInputComponent or a subclass of it.")))
		{
			TArray<uint32> BindHandles;
			MotionInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
			
			MotionInputComponent->BindActionByTag(InputConfig, MotionGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AMotionCharacter::Input_Move);
			MotionInputComponent->BindActionByTag(InputConfig, MotionGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &AMotionCharacter::Input_Look);
			MotionInputComponent->BindActionByTag(InputConfig, MotionGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &AMotionCharacter::Input_Look);
			MotionInputComponent->BindActionByTag(InputConfig, MotionGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &AMotionCharacter::Input_Crouch);
			MotionInputComponent->BindActionByTag(InputConfig, MotionGameplayTags::InputTag_Jump, ETriggerEvent::Triggered, this, &AMotionCharacter::Input_Jump);
		}
	}
}

void AMotionCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    if(!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(MotionGameplayTags::GameplayTag_PlayerCannotControlMovement))
    {
    	return;
    }

	if (Controller != nullptr)
	{
		const APawn* Pawn = Controller->GetPawn();
		const FRotator ActorRotation = Pawn->GetActorRotation();
		const FVector PawnVelocity = Pawn->GetVelocity();
		
		const FVector LocalAcceleration = ActorRotation.UnrotateVector(MovementComponent->GetCurrentAcceleration());
		const FVector LocalVelocity = ActorRotation.UnrotateVector(FVector {PawnVelocity.X, PawnVelocity.Y, 0.0f});
		
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		const double DotProduct = FVector::DotProduct(LocalAcceleration.GetSafeNormal(0.0001), LocalVelocity.GetSafeNormal(0.0001));

		const double LocalAcceleration2D = LocalAcceleration.Size2D();
		const double LocalVelocity2D = LocalVelocity.Size2D();

		if(LocalAcceleration2D > 0.1f
			&& LocalVelocity2D < 200.0f
			&& DotProduct >= -0.6f
			&& DotProduct <= 0.6f)
		{
			return;
		}
		
		if (MoveValue.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, MoveValue.X);
		}
	 
		if (MoveValue.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, MoveValue.Y);
		}
	}
}

void AMotionCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    if(!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(MotionGameplayTags::GameplayTag_PlayerCannotControlCamera))
    {
    	return;
    }
        
	if (Controller != nullptr)
	{
		const FVector2D LookValue = InputActionValue.Get<FVector2D>();
	 
		if (LookValue.X != 0.0f)
		{
			AddControllerYawInput(LookValue.X * TurnRateCamera * GetWorld()->GetDeltaSeconds());
		}
	 
		if (LookValue.Y != 0.0f)
		{
			AddControllerPitchInput(LookValue.Y * TurnRateCamera * GetWorld()->GetDeltaSeconds());
		}
	}
}

void AMotionCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UMotionAbilitySystemComponent* MotionAbilitySystemComponent = Cast<UMotionAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
        if(AbilitySystemComponent->HasMatchingGameplayTag(MotionGameplayTags::GameplayTag_PlayerCannotControlAbilities))
        {
        	return;
        }
            
		MotionAbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void AMotionCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UMotionAbilitySystemComponent* MotionAbilitySystemComponent = Cast<UMotionAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
	    if(AbilitySystemComponent->HasMatchingGameplayTag(MotionGameplayTags::GameplayTag_PlayerCannotControlAbilities))
	    {
	    	return;
	    }
            
		MotionAbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}

const FCharacterGroundInfo& AMotionCharacter::GetGroundInfo()
{
	if (GFrameCounter == CachedGroundInfo.LastUpdateFrame)
	{
		return CachedGroundInfo;
	}

	if (auto CharacterMovementComponent = GetCharacterMovement(); CharacterMovementComponent->IsWalking())
	{
		CachedGroundInfo.GroundHitResult = CharacterMovementComponent->CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		check(CapsuleComp);

		const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (CharacterMovementComponent->UpdatedComponent ? CharacterMovementComponent->UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - 100000.0f - CapsuleHalfHeight));

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(MotionCharacter_GetGroundInfo), false, this);
		FCollisionResponseParams ResponseParam;
		CharacterMovementComponent->InitCollisionParams(QueryParams, ResponseParam);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

		CachedGroundInfo.GroundHitResult = HitResult;
		CachedGroundInfo.GroundDistance = 100000.0f;
	
		if (HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;

	return CachedGroundInfo;
}

