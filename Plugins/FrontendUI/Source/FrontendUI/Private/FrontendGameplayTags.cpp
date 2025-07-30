// XiaoYao copyright.


#include "FrontendGameplayTags.h"

namespace FrontendGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Frontend_PrimaryLayout, TEXT("UI.PrimaryLayout"))
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, TEXT("UI.Layer.Modal"))
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Menu, TEXT("UI.Layer.Menu"))
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, TEXT("UI.Layer.GameMenu"))
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Game, TEXT("UI.Layer.Game"))

	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKey, TEXT("Frontend.Widget.PressAnyKey"))
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenu, TEXT("Frontend.Widget.MainMenu"))
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, TEXT("Frontend.Widget.ConfirmScreen"))
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_StoryScreen, TEXT("Frontend.Widget.StoryScreen"));

	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_OptionScreen, TEXT("Frontend.Widget.OptionScreen"));
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_KeyRemapScreen,"Frontend.Widget.KeyRemapScreen");
	
	//Frontend Options Image
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Image_TestImage,"Frontend.Image.TestImage");
}