#include "XYSEditor.h"

#define LOCTEXT_NAMESPACE "FXYSEditorModule"

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Development/XYSDeveloperSettings.h"

class FXYSEditorModule : public IModuleInterface
{
	
public:
	virtual void StartupModule() override
	{
		if (!IsRunningGame())
		{
			if (FSlateApplication::IsInitialized())
			{
				ToolMenusHandle = UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateStatic(&RegisterGameEditorMenus));
			}
		}
	}

	virtual void ShutdownModule() override
	{
	}

	static void RegisterGameEditorMenus()
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		FToolMenuSection& Section = Menu->AddSection("PlayGameExtensions", TAttribute<FText>(),
		                                             FToolMenuInsert("Play", EToolMenuInsertType::After));

		// Uncomment this to add a custom toolbar that is displayed during PIE
		// Useful for making easy access to changing game state artificially, adding cheats, etc
		// FToolMenuEntry BlueprintEntry = FToolMenuEntry::InitComboButton(
		// 	"OpenGameMenu",
		// 	FUIAction(
		// 		FExecuteAction(),
		// 		FCanExecuteAction::CreateStatic(&HasPlayWorld),
		// 		FIsActionChecked(),
		// 		FIsActionButtonVisible::CreateStatic(&HasPlayWorld)),
		// 	FOnGetContent::CreateStatic(&YourCustomMenu),
		// 	LOCTEXT("GameOptions_Label", "Game Options"),
		// 	LOCTEXT("GameOptions_ToolTip", "Game Options"),
		// 	FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.OpenLevelBlueprint")
		// );
		// BlueprintEntry.StyleNameOverride = "CalloutToolbar";
		// Section.AddEntry(BlueprintEntry);

		/* check content button */
		// FToolMenuEntry CheckContentEntry = FToolMenuEntry::InitToolBarButton(
		// 	"CheckContent",
		// 	FUIAction(
		// 		FExecuteAction::CreateStatic(&CheckGameContent_Clicked),
		// 		FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
		// 		FIsActionChecked(),
		// 		FIsActionButtonVisible::CreateStatic(&HasNoPlayWorld)),
		// 	LOCTEXT("CheckContentButton", "Check Content"),
		// 	LOCTEXT("CheckContentDescription",
		// 	        "Runs the Content Validation job on all checked out assets to look for warnings and errors"),
		// 	FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Level")
		// );
		// CheckContentEntry.StyleNameOverride = "CalloutToolbar";
		// Section.AddEntry(CheckContentEntry);

		FToolMenuEntry CommonMapEntry = FToolMenuEntry::InitComboButton(
			"CommonMapOptions",
			FUIAction(
				FExecuteAction(),
				FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
				FIsActionChecked(),
				FIsActionButtonVisible::CreateStatic(&CanShowCommonMaps)),
			FOnGetContent::CreateStatic(&GetCommonMapsDropdown),
			LOCTEXT("CommonMaps_Label", "Common Maps"),
			LOCTEXT("CommonMaps_ToolTip", "Some commonly desired maps while using the editor"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Level")
		);
		CommonMapEntry.StyleNameOverride = "CalloutToolbar";
		Section.AddEntry(CommonMapEntry);
	}

	static void CheckGameContent_Clicked()
	{
		// UEditorValidator::ValidateCheckedOutContent(/*bInteractive=*/true, EDataValidationUsecase::Manual);
	}
	static bool CanShowCommonMaps()
	{
		return HasNoPlayWorld() /*&& !GetDefault<UXYSDeveloperSettings>()->CommonEditorMaps.IsEmpty()*/;
	}

	static TSharedRef<SWidget> GetCommonMapsDropdown()
	{
		FMenuBuilder MenuBuilder(true, nullptr);
	
		for (const FSoftObjectPath& Path : GetDefault<UXYSDeveloperSettings>()->CommonEditorMaps)
		{
			if (!Path.IsValid())
			{
				continue;
			}
		
			const FText DisplayName = FText::FromString(Path.GetAssetName());
			MenuBuilder.AddMenuEntry(
				DisplayName,
				LOCTEXT("CommonPathDescription", "Opens this map in the editor"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateStatic(&OpenCommonMap_Clicked, Path.ToString()),
					FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
					FIsActionChecked(),
					FIsActionButtonVisible::CreateStatic(&HasNoPlayWorld)
				)
			);
		}

		return MenuBuilder.MakeWidget();
	}

	static void OpenCommonMap_Clicked(const FString MapPath)
	{
		if (ensure(MapPath.Len()))
		{
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(MapPath);
		}
	}
	
	static bool HasNoPlayWorld()
	{
		return !HasPlayWorld();
	}

	static bool HasPlayWorld()
	{
		return GEditor->PlayWorld != nullptr;
	}
	
private:
	FDelegateHandle ToolMenusHandle;
};


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FXYSEditorModule, XYSEditor)
