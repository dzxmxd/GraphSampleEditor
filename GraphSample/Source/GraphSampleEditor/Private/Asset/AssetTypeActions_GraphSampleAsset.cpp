// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Asset/AssetTypeActions_GraphSampleAsset.h"

#include "GraphSampleAsset.h"
#include "GraphSampleEditorModule.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_GraphSampleAsset"

FText FAssetTypeActions_GraphSampleAsset::GetName() const
{
	return LOCTEXT("AssetTypeActions_GraphSampleAsset", "GraphSample Asset");
}

uint32 FAssetTypeActions_GraphSampleAsset::GetCategories()
{
	return FGraphSampleEditorModule::GraphSampleAssetCategory;
}

UClass* FAssetTypeActions_GraphSampleAsset::GetSupportedClass() const
{
	return UGraphSampleAsset::StaticClass();
}

void FAssetTypeActions_GraphSampleAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UGraphSampleAsset* GraphSampleAsset = Cast<UGraphSampleAsset>(*ObjIt))
		{
			const FGraphSampleEditorModule* GraphSampleModule = &FModuleManager::LoadModuleChecked<FGraphSampleEditorModule>("GraphSampleEditor");
			GraphSampleModule->CreateGraphSampleAssetEditor(Mode, EditWithinLevelEditor, GraphSampleAsset);
		}
	}
}

#undef LOCTEXT_NAMESPACE
