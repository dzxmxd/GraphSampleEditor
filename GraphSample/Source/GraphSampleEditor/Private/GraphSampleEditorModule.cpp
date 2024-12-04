// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "GraphSampleEditorModule.h"

#include "GraphSampleAsset.h"
#include "Asset/GraphSampleAssetEditor.h"
#include "AssetToolsModule.h"
#include "GraphSampleEditorStyle.h"
#include "Asset/AssetTypeActions_GraphSampleAsset.h"

#define LOCTEXT_NAMESPACE "GraphSampleEditorModule"

EAssetTypeCategories::Type FGraphSampleEditorModule::GraphSampleAssetCategory = static_cast<EAssetTypeCategories::Type>(0);

const FGraphSampleEditorModule* FGraphSampleEditorModule::Get()
{
	const FGraphSampleEditorModule* GraphSampleModule = FModuleManager::LoadModulePtr<FGraphSampleEditorModule>("GraphSampleEditor");
	if (GraphSampleModule == nullptr)
	{
		const FString ErrorStr = FString::Printf(TEXT("Get GraphSampleEditorModule failed, maybe using in wrong LoadingPhase."));
		UE_LOG(LogTemp, Error, TEXT("%s:%d %s"), *FString(__FUNCTION__), __LINE__, *ErrorStr);
	}
	return GraphSampleModule;
}

void FGraphSampleEditorModule::StartupModule()
{
	FGraphSampleEditorStyle::Initialize();
	RegisterAssets();
	RegisterDelegates();
}

void FGraphSampleEditorModule::ShutdownModule()
{
	FGraphSampleEditorStyle::Shutdown();
	UnregisterAssets();
	UnregisterDelegates();
}

TSharedRef<FGraphSampleAssetEditor> FGraphSampleEditorModule::CreateGraphSampleAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UGraphSampleAsset* GraphSampleAsset)
{
	TSharedRef<FGraphSampleAssetEditor> NewGraphSampleAssetEditor(new FGraphSampleAssetEditor());
	NewGraphSampleAssetEditor->InitGraphSampleAssetEditor(Mode, InitToolkitHost, GraphSampleAsset);
	return NewGraphSampleAssetEditor;
}

void FGraphSampleEditorModule::RegisterAssets()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	if (GraphSampleAssetCategory == EAssetTypeCategories::None)
	{
		GraphSampleAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("GraphSample")), LOCTEXT("GraphSampleEditorModule_GraphSampleCategory", "GraphSample"));
	}

	const TSharedRef<IAssetTypeActions> GraphSampleAssetActions = MakeShareable(new FAssetTypeActions_GraphSampleAsset());
	RegisteredAssetActions.Add(GraphSampleAssetActions);
	AssetTools.RegisterAssetTypeActions(GraphSampleAssetActions);
}

void FGraphSampleEditorModule::UnregisterAssets()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (const TSharedRef<IAssetTypeActions>& TypeAction : RegisteredAssetActions)
		{
			AssetTools.UnregisterAssetTypeActions(TypeAction);
		}
	}

	RegisteredAssetActions.Empty();
}

void FGraphSampleEditorModule::RegisterDelegates()
{
}

void FGraphSampleEditorModule::UnregisterDelegates()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGraphSampleEditorModule, GraphSampleEditor)
