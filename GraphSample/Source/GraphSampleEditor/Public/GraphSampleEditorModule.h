// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"


class IAssetTypeActions;
class UGraphSampleAsset;
class FGraphSampleAssetEditor;

class GRAPHSAMPLEEDITOR_API FGraphSampleEditorModule : public IModuleInterface
{
public:
	static EAssetTypeCategories::Type GraphSampleAssetCategory;
	static const FGraphSampleEditorModule* Get();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	static TSharedRef<FGraphSampleAssetEditor> CreateGraphSampleAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UGraphSampleAsset* GraphSampleAsset);

private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetActions;

	void RegisterAssets();
	void UnregisterAssets();
	void RegisterDelegates();
	void UnregisterDelegates();
};
