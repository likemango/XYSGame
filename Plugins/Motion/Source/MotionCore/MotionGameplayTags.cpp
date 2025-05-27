// Copyright 2021 - 2024, Kia Armani. All Rights Reserved.

#include "MotionGameplayTags.h"

namespace MotionGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Sprint, "InputTag.Sprint", "Sprint input.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_WantsToCrouch, "GameplayTag.WantsToCrouch", "True, if the character wants to crouch.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_IsCrouching, "GameplayTag.IsCrouching", "True, if the character is crouching.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_IsSprinting, "GameplayTag.IsSprinting", "True, if the character is sprinting.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_IsJumping, "GameplayTag.IsJumping", "True, if the character is jumping.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_DisableCameraCollisionCheck, "GameplayTag.DisableCameraCollisionCheck", "If set, the camera collision check is disabled.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_PlayerCannotControlCamera, "GameplayTag.PlayerCannotControlCamera", "If set, the player cannot control the camera via inputs.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_PlayerCannotControlMovement, "GameplayTag.PlayerCannotControlMovement", "If set, the player cannot move the character via inputs.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_PlayerCannotControlAbilities, "GameplayTag.PlayerCannotControlAnyAbilities", "If set, the player can not trigger gameplay ablities via inputs.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_PlayerCannotControlNativeActions, "GameplayTag.PlayerCannotControlNativeActions", "If set, the player can not trigger any native (i.e. sprint, crouch, jump) gameplay actions via inputs.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayTag_MovementHalted, "GameplayTag.MovementHalted", "If set, the player max speed is set to 0.");
	
}
