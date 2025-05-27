// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MOTIONCORE_API UMotionCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Map of walkspeeds that, added together, result in GlobalMaxWalkSpeed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
		TMap<FString, float> WalkSpeeds;

	/* Combined walk speed of all active walk speeds. This speed is the player's current max. walk speed. It cannot be modified via Blueprints. */
	UPROPERTY(BlueprintReadOnly, Category = "Motion")
		float GlobalMaxWalkSpeed;

	/* Adds a walk speed to the array of active walk speeds. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		void AddWalkSpeed(FString Identifier, float Value);

	/* Removes a walk speed to the array of active walk speeds. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		void RemoveWalkSpeed(FString Identifier);

	/* Calls OverlapBlockingTestByChannel with a sphere. */
	UFUNCTION(BlueprintCallable, Category = "Motion")
		bool OverlapBlockingTestByChannel(const FVector &Pos, const float CollisionShapeHeight) const;

	virtual float GetMaxSpeed() const override;

private:
	/* Default Max Walk Speed the character was configured with before it was modified. */
	float DefaultMaxWalkSpeedOnInit;
};

inline bool UMotionCharacterMovementComponent::OverlapBlockingTestByChannel(const FVector &Pos, const float CollisionShapeHeight) const
{
	const UWorld* World = GetWorld();
	const FCollisionShape CapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, CollisionShapeHeight);
	FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CapsuleTrace), false, GetOwner());
	FCollisionResponseParams ResponseParam;

	InitCollisionParams(CapsuleParams, ResponseParam);
	
	return World->OverlapBlockingTestByChannel(Pos, FQuat::Identity, UpdatedComponent->GetCollisionObjectType(), CapsuleShape, CapsuleParams, ResponseParam);
}
