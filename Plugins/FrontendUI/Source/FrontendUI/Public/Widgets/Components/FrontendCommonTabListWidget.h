// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendCommonTabListWidget.generated.h"

class UFrontendCommonButtonBase;
/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendCommonTabListWidget : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);
	
protected:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	
private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontendTabListSettings", meta=(AllowPrivateAccess="true", ClampMin="1", ClampMax="10"))
	int32 TabCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontendTabListSettings", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UFrontendCommonButtonBase> TabButtonEntryWidgetClass;
#endif
	
};

