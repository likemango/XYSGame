// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XYSCharacter.h"

#include "InputActionValue.h"
#include "AbilitySystem/XYSAbilitySystemComponent.h"
#include "Character/XYSCharacterMovementComponent.h"
#include "Camera/XYSCameraComponent.h"
#include "Character/XYSHeroComponent.h"
#include "Character/XYSPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"


AXYSCharacter::AXYSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UXYSCharacterMovementComponent>(CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetNetCullDistanceSquared(900000000.0f);

	UXYSCharacterMovementComponent* XYSMoveComp = CastChecked<UXYSCharacterMovementComponent>(GetCharacterMovement());
	XYSMoveComp->GravityScale = 1.0f;
	XYSMoveComp->MaxAcceleration = 2400.0f;
	XYSMoveComp->BrakingFrictionFactor = 1.0f;
	XYSMoveComp->BrakingFriction = 6.0f;
	XYSMoveComp->GroundFriction = 8.0f;
	XYSMoveComp->BrakingDecelerationWalking = 1400.0f;
	XYSMoveComp->bUseControllerDesiredRotation = false;
	XYSMoveComp->bOrientRotationToMovement = false;
	// XYSMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	XYSMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	XYSMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	XYSMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	// XYSMoveComp->SetCrouchedHalfHeight(65.0f);

	PawnExtComponent = CreateDefaultSubobject<UXYSPawnExtensionComponent>(TEXT("PawnExtComponent"));
	// PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	// PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	HeroComponent = CreateDefaultSubobject<UXYSHeroComponent>(TEXT("HeroComponent"));
	
	bUseControllerRotationYaw = true;
	// XYSCameraComponent = CreateDefaultSubobject<UXYSCameraComponent>(TEXT("XYSCameraComponent"));
	// XYSCameraComponent->SetupAttachment(GetMesh(), CameraAttachSocket);
	// XYSCameraComponent->bUsePawnControlRotation = true;
}

void AXYSCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->LinkAnimClassLayers(DefaultAnimLinkedLayerForAllSkeletalMeshes);
}

UXYSAbilitySystemComponent* AXYSCharacter::GetXYSAbilitySystemComponent() const
{
	return Cast<UXYSAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AXYSCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetXYSAbilitySystemComponent();
}

void AXYSCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!Controller) return;

	const APawn* Pawn = Controller->GetPawn();
	// const FRotator ActorRotation = Pawn->GetActorRotation();
	const FVector PawnVelocity = Pawn->GetVelocity();

	// const FVector LocalAcceleration = ActorRotation.UnrotateVector(XYSMovementComponent->GetCurrentAcceleration());
	// const FVector LocalVelocity = ActorRotation.UnrotateVector(FVector(PawnVelocity.X, PawnVelocity.Y, 0.f));
	const FVector LocalAcceleration = XYSMovementComponent->GetCurrentAcceleration();
	const FVector LocalVelocity = FVector(PawnVelocity.X, PawnVelocity.Y, 0.f);
	const double DotProduct = FVector::DotProduct(LocalAcceleration.GetSafeNormal(0.0001f),LocalVelocity.GetSafeNormal(0.0001f));

	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + LocalAcceleration * 100.f, 2, FLinearColor::Red);
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + LocalVelocity * 100.f, 2, FLinearColor::Blue);
	
	const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
	const FRotator ControllerRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	
	const double LocalAcceleration2D = LocalAcceleration.Size2D();
	const double LocalVelocity2D = LocalVelocity.Size2D();

	if (LocalAcceleration2D > 0.1f && LocalVelocity2D < 200.f
		&& DotProduct >= -0.6f && DotProduct <= 0.6f)
	{
		// 认为角色与墙壁碰撞
		return;
	}

	if (MoveValue.X != 0.f)
	{
		const FVector MovementDirection = ControllerRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, MoveValue.X);
	}
	if (MoveValue.Y != 0.f)
	{
		const FVector MovementDirection = ControllerRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, MoveValue.Y);
	}
}

void AXYSCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	if (!Controller) return;

	const FVector2D LookValue = InputActionValue.Get<FVector2D>();
	if (LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X * TurnRateCamera * GetWorld()->GetDeltaSeconds());
	}
	if (LookValue.Y != 0.f)
	{
		AddControllerPitchInput(LookValue.Y * TurnRateCamera * GetWorld()->GetDeltaSeconds());
	}
}

// void AXYSCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
// {
// 	if (!Controller || !XYSMovementComponent) return;
//
// 	if (XYSMovementComponent->bWantsToCrouch || bIsCrouched)
// 	{
// 		UnCrouch();
// 	}
// 	else if (XYSMovementComponent->IsMovingOnGround())
// 	{
// 		Crouch();
// 	}
// }

// void AXYSCharacter::Input_Jump(const FInputActionValue& InputActionValue)
// {
// 	if (!Controller) return;
//
// 	if (XYSMovementComponent->bWantsToCrouch || bIsCrouched)
// 	{
// 		UnCrouch();
// 	}
// 	else if (CanJump())
// 	{
// 		Jump();
// 	}
// }

void AXYSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

const FCharacterGroundInfo& AXYSCharacter::GetGroundInfo()
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

void AXYSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void AXYSCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();
}

void AXYSCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void AXYSCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}
