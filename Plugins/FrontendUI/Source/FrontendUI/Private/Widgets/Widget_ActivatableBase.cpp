// XiaoYao copyright.


#include "Widgets/Widget_ActivatableBase.h"
#include "Controller/FrontendPlayerController.h"

AFrontendPlayerController* UWidget_ActivatableBase::GetOwingFrontendPlayerController()
{
	if (!CachedOwningPlayerController.IsValid())
	{
		CachedOwningPlayerController = GetOwningPlayer<AFrontendPlayerController>();
		ensure(CachedOwningPlayerController.IsValid());
	}
	return (CachedOwningPlayerController.IsValid() ? CachedOwningPlayerController.Get() : nullptr);
}
