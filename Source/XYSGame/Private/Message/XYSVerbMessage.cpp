// XiaoYao copyright.


#include "Message/XYSVerbMessage.h"

FString FXYSVerbMessage::ToString() const
{
	FString HumanReadableMessage;
	FXYSVerbMessage::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, this, /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
	return HumanReadableMessage;
}
