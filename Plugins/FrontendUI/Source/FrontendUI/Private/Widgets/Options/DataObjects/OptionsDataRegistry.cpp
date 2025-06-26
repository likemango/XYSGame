// XiaoYao copyright.


#include "Widgets/Options/DataObjects/OptionsDataRegistry.h"

#include "FrontendGameplayTags.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	//This is the full code for constructing data interactor helperAdd commentMore actions
	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = 
	MakeShared<FOptionsDataInteractionHelper>(
		GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,GetCurrentGameDifficulty)
	);*/
	
	//Game Difficulty
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</> Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
		GameDifficulty->AddDynamicOption(TEXT("Easy"),FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"),FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"),FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Very Hard"),FText::FromString(TEXT("Very Hard")));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplySettingsImmediately(true);
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}
	//Test ItemAdd commentMore actions
	{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName(TEXT("TestItem")));
		TestItem->SetDataDisplayName(FText::FromString(TEXT("Test Image Item")));
		TestItem->SetSoftDescriptionImage(UFrontendBlueprintFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
		TestItem->SetDescriptionRichText(FText::FromString(TEXT("The image to display can be specified in the project settings. It can be anything the developer assigned in there")));

		GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	//Volume CategoryAdd commentMore actions
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		//Test ItemAdd commentMore actions
		{
			UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
			TestItem->SetDataID(FName(TEXT("TestItem")));
			TestItem->SetDataDisplayName(FText::FromString(TEXT("Test Image Item")));

			VolumeCategoryCollection->AddChildListData(TestItem);
		}
	}

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& TabID)
{
	UListDataObject_Collection** FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate([TabID](const UListDataObject_Collection* Collection)->bool
	{
		if (Collection && Collection->GetDataID() == TabID)
			return true;
		 return false;
	});
	checkf(FoundTabCollectionPtr,TEXT("No valid tab found under the ID %s"), *(*FoundTabCollectionPtr)->GetDataID().ToString());
	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObject_Base*> FoundAllTabs;
	FindChildListDataRecursively(FoundTabCollection, FoundAllTabs);
	return FoundAllTabs;
}

void UOptionsDataRegistry::FindChildListDataRecursively(const UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData,OutFoundChildListData);
		}
	}
}
