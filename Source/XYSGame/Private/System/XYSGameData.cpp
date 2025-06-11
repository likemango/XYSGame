// XiaoYao copyright.


#include "System/XYSGameData.h"

#include "System/XYSAssetManager.h"

UXYSGameData::UXYSGameData()
{
}

const UXYSGameData& UXYSGameData::UXYSGameData::Get()
{
	return UXYSAssetManager::Get().GetGameData();
}
