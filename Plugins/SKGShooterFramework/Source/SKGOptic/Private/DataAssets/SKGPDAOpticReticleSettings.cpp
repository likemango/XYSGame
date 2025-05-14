// Copyright 2023, Dakota Dawe, All rights reserved


#include "DataAssets/SKGPDAOpticReticleSettings.h"
#include "Components/SKGOpticSceneCaptureComponent.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

void FSKGOpticReticleSettings::ConstructDynamicMaterials(UObject* WorldContextObject, USKGOpticSceneCaptureComponent* OptionalOpticSceneCaptureComponent)
{
	const float Brightness = GetReticleBrightness();
	for (FSKGOpticReticle& ReticleMaterial : ReticleMaterials)
	{
		ReticleMaterial.DynamicReticleMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(WorldContextObject, ReticleMaterial.ReticleMaterial);	
		ReticleMaterial.DynamicReticleMaterial->SetScalarParameterValue(ReticleBrightnessParameterName, Brightness);
		if (OptionalOpticSceneCaptureComponent)
		{
			OptionalOpticSceneCaptureComponent->SetupReticleMaterial(ReticleMaterial);
		}
	}

	if (UnAimedReticleMaterial.ReticleMaterial)
	{
		UnAimedReticleMaterial.DynamicReticleMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(WorldContextObject, UnAimedReticleMaterial.ReticleMaterial);	
		UnAimedReticleMaterial.DynamicReticleMaterial->SetScalarParameterValue(ReticleBrightnessParameterName, Brightness);
		if (OptionalOpticSceneCaptureComponent)
		{
			OptionalOpticSceneCaptureComponent->SetupReticleMaterial(UnAimedReticleMaterial);
		}
	}
}

bool FSKGOpticReticleSettings::IncreaseReticleBrightnessSetting()
{
	if (bUsingReticleNightVisionBrightness)
	{
		if (++CurrentReticleNightVisionBrightnessIndex >= ReticleNightVisionBrightnessSettings.Num())
		{
			CurrentReticleNightVisionBrightnessIndex = ReticleNightVisionBrightnessSettings.Num() - 1;
			return false;
		}
	}
	else
	{
		if (++CurrentReticleBrightnessIndex >= ReticleBrightnessSettings.Num())
		{
			CurrentReticleBrightnessIndex = ReticleBrightnessSettings.Num() - 1;
			return false;
		}
	}
	return true;
}

bool FSKGOpticReticleSettings::DecreaseReticleBrightnessSetting()
{
	if (bUsingReticleNightVisionBrightness)
	{
		if (!CurrentReticleNightVisionBrightnessIndex - 1 < 0)
		{
			--CurrentReticleNightVisionBrightnessIndex;
			return true;
		}
	}
	else
	{
		if (!CurrentReticleBrightnessIndex - 1 < 0)
		{
			--CurrentReticleBrightnessIndex;
			return true;
		}
	}
	return false;
}