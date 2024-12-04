// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "Factories/Factory.h"
#include "CoreMinimal.h"
#include "GraphSampleAssetFactory.generated.h"

UCLASS(HideCategories = Object)
class GRAPHSAMPLEEDITOR_API UGraphSampleAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
