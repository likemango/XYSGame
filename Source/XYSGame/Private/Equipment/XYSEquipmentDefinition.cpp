// XiaoYao copyright.


#include "Equipment/XYSEquipmentDefinition.h"

#include "Equipment/XYSEquipmentInstance.h"

UXYSEquipmentDefinition::UXYSEquipmentDefinition(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InstanceType = UXYSEquipmentInstance::StaticClass();
}
