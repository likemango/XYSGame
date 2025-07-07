// XiaoYao copyright.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFrontend, Log, All);

namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 Key = -1, float TimeToDisplay = 5.f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, Msg);

			UE_LOG(LogFrontend, Log, TEXT("%s"), *Msg);
		}
	}
}
