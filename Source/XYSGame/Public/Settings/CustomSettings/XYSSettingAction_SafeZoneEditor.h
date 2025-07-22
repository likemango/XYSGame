// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingAction.h"
#include "GameSettingValueScalarDynamic.h"
#include "XYSSettingAction_SafeZoneEditor.generated.h"


UCLASS()
class XYSGAME_API UXYSSettingValueScalarDynamic_SafeZoneValue : public UGameSettingValueScalarDynamic
{
	GENERATED_BODY()
	
public:
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;
};


UCLASS()
class UXYSSettingAction_SafeZoneEditor : public UGameSettingAction
{
	GENERATED_BODY()
	
public:
	UXYSSettingAction_SafeZoneEditor();
	virtual TArray<UGameSetting*> GetChildSettings() override;

private:
	UPROPERTY()
	TObjectPtr<UXYSSettingValueScalarDynamic_SafeZoneValue> SafeZoneValueSetting;
};