// XiaoYao copyright.

#pragma once

#include "FrontendTypes.generated.h"

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	OK,
	YesNo,
	OKCancel,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};