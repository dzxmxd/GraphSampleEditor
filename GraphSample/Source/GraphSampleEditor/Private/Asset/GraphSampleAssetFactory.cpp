// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Asset/GraphSampleAssetFactory.h"
#include "GraphSampleAsset.h"
#include "Graph/GraphSampleGraph.h"

#define LOCTEXT_NAMESPACE "GraphSampleAssetFactory"

UGraphSampleAssetFactory::UGraphSampleAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UGraphSampleAsset::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

bool UGraphSampleAssetFactory::ConfigureProperties()
{
	const FText TitleText = LOCTEXT("CreateGraphSampleAssetOptions", "Pick GraphSample Asset Class");
	return true;
}

UObject* UGraphSampleAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UGraphSampleAsset* NewGraphSampleAsset = NewObject<UGraphSampleAsset>(InParent, SupportedClass, Name, Flags | RF_Transactional, Context);
	UGraphSampleGraph::CreateGraph(NewGraphSampleAsset);
	return NewGraphSampleAsset;
}

#undef LOCTEXT_NAMESPACE
