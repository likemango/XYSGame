// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/FrontendLoadingScreenInterface.h"


// Add default functionality here for any IFrontendLoadingScreenInterface functions that are not pure virtual.
bool IFrontendLoadingScreenInterface::ShouldShowLoadingScreen(UObject* TestObject, FString& OutReason)
{
	if (TestObject != nullptr)
	{
		if (IFrontendLoadingScreenInterface* LoadObserver = Cast<IFrontendLoadingScreenInterface>(TestObject))
		{
			FString ObserverReason;
			if (LoadObserver->ShouldShowLoadingScreen(/*out*/ ObserverReason))
			{
				if (ensureMsgf(!ObserverReason.IsEmpty(), TEXT("%s failed to set a reason why it wants to show the loading screen"), *GetPathNameSafe(TestObject)))
				{
					OutReason = ObserverReason;
				}
				return true;
			}
		}
	}

	return false;
}
