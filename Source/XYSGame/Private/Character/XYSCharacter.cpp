// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XYSCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/XYSCharacterMovementComponent.h"
#include "Camera/XYSCameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/XYSEnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"


AXYSCharacter::AXYSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UXYSCharacterMovementComponent>(CharacterMovementComponentName))
{

	bUseControllerRotationYaw = true;

	XYSCameraComponent = CreateDefaultSubobject<UXYSCameraComponent>(TEXT("XYSCameraComponent"));
	XYSCameraComponent->SetupAttachment(GetMesh(), CameraAttachSocket);
	XYSCameraComponent->bUsePawnControlRotation = true;
	
	XYSMovementComponent = Cast<UXYSCharacterMovementComponent>(GetCharacterMovement());
	XYSMovementComponent->bOrientRotationToMovement = false;
	XYSMovementComponent->bUseControllerDesiredRotation = false;
}

void AXYSCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->LinkAnimClassLayers(DefaultAnimLinkedLayerForAllSkeletalMeshes);
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

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + LocalAcceleration * 100.f, 2, FLinearColor::Red);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + LocalVelocity * 100.f, 2, FLinearColor::Blue);
	
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

void AXYSCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (!Controller || !XYSMovementComponent) return;

	if (XYSMovementComponent->bWantsToCrouch || bIsCrouched)
	{
		UnCrouch();
	}
	else if (XYSMovementComponent->IsMovingOnGround())
	{
		Crouch();
	}
}

void AXYSCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	if (!Controller) return;

	if (XYSMovementComponent->bWantsToCrouch || bIsCrouched)
	{
		UnCrouch();
	}
	else if (CanJump())
	{
		Jump();
	}
}

void AXYSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	UXYSEnhancedInputComponent* EnhancedInputComponent = Cast<UXYSEnhancedInputComponent>(InputComponent);
	ensureMsgf(EnhancedInputComponent, TEXT("Unexpected Input Component class! "
										 "The Gameplay Abilities will not be bound to their inputs. Change the input component to UXYSEnhancedInputComponent or a subclass of it."));
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	check(EnhancedInputLocalPlayerSubsystem);

	EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);

	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this,  &AXYSCharacter::Input_Move);
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this,  &AXYSCharacter::Input_Look);
	EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Triggered, this,  &AXYSCharacter::Input_Crouch);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this,  &AXYSCharacter::Input_Jump);
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
