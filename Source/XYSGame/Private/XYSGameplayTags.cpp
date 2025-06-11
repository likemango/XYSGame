// XiaoYao copyright.


#include "XYSGameplayTags.h"

namespace XYSGameplayTags
{
	// GameFrameworkManager init process
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "1: Actor/component has initially spawned and can be extended");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");

	// Native Input
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_Crouch, "InputTag.Ability.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_Jump, "InputTag.Ability.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_Pace, "InputTag.Ability.Pace");

	UE_DEFINE_GAMEPLAY_TAG(CharacterState_Movement_Crouching, "CharacterState.Movement.Crouching");
	UE_DEFINE_GAMEPLAY_TAG(CharacterState_Movement_Pacing, "CharacterState.Movement.Pacing");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath", "An ability with this type tag should not be canceled due to death.");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Type_Action_Jump, "Ability.Type.Action.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Type_Action_Crouch, "Ability.Type.Action.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Type_Action_Pace, "Ability.Type.Action.Pace");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked", "Use to Block all ability inputs in `ProcessAbilityInput`.");
	UE_DEFINE_GAMEPLAY_TAG(TAG_XYS_Inventory_Message_StackChanged, "XYS.Inventory.Message.StackChanged")
	UE_DEFINE_GAMEPLAY_TAG(TAG_XYS_QuickBar_Message_SlotsChanged, "XYS.QuickBar.Message.SlotsChanged")
	UE_DEFINE_GAMEPLAY_TAG(TAG_XYS_QuickBar_Message_ActiveIndexChanged, "XYS.QuickBar.Message.ActiveIndexChanged")
	UE_DEFINE_GAMEPLAY_TAG(TAG_XYS_Elimination_Message, "XYS.Elimination.Message");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");

	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Damage, "Gameplay.Damage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageSelfDestruct, "Gameplay.Damage.SelfDestruct");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_FellOutOfWorld, "Gameplay.Damage.FellOutOfWorld");
	UE_DEFINE_GAMEPLAY_TAG(TAG_XYS_Damage_Message, "XYS.Damage.Message");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Heal, "SetByCaller.Heal", "SetByCaller tag used by healing gameplay effects.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_GodMode, "Cheat.GodMode", "GodMode cheat is active on the owner.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_UnlimitedHealth, "Cheat.UnlimitedHealth", "UnlimitedHealth cheat is active on the owner.");
}