// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingValueDiscrete.h"
#include "XYSSettingValueDiscrete_PerfStat.generated.h"

enum class EXYSDisplayablePerformanceStat : uint8;
enum class EXYSStatDisplayMode : uint8;

/**
 * 
 */
UCLASS()
class XYSGAME_API UXYSSettingValueDiscrete_PerfStat : public UGameSettingValueDiscrete
{
	GENERATED_BODY()

public:
	UXYSSettingValueDiscrete_PerfStat();

	void SetStat(EXYSDisplayablePerformanceStat InStat);

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	
	void AddMode(FText&& Label, EXYSStatDisplayMode Mode);
protected:
	TArray<FText> Options;
	TArray<EXYSStatDisplayMode> DisplayModes;

	EXYSDisplayablePerformanceStat StatToDisplay;
	EXYSStatDisplayMode InitialMode;
};
