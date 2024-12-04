// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"

class SGraphSampleGraphEditor;
class UGraphSampleAsset;

class GRAPHSAMPLEEDITOR_API FGraphSampleAssetEditor : public FAssetEditorToolkit
{
public:
	/**	The tab ids for all the tabs used */
	static const FName DetailsTab;
	static const FName GraphTab;
	static const FName PaletteTab;

protected:
	/** The GraphSample Asset being edited */
	TObjectPtr<UGraphSampleAsset> GraphSampleAsset;

	TSharedPtr<SGraphSampleGraphEditor> GraphEditor;
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedPtr<class SGraphSamplePalette> Palette;

public:
	FGraphSampleAssetEditor();
	virtual ~FGraphSampleAssetEditor() override;

	UGraphSampleAsset* GetGraphSampleAsset() const { return GraphSampleAsset; }
	TSharedPtr<SGraphSampleGraphEditor> GetGraphSampleGraph() const { return GraphEditor; }

	// IToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	// --

	// FAssetEditorToolkit
	virtual void InitToolMenuContext(FToolMenuContext& MenuContext) override;
	virtual void PostRegenerateMenusAndToolbars() override;
	virtual bool CanSaveAsset() const override;
	// --

	bool IsTabFocused(const FTabId& TabId) const;

private:
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> SpawnTab_Palette(const FSpawnTabArgs& Args) const;

public:
	/** Edits the specified GraphSampleAsset object */
	void InitGraphSampleAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);
	virtual void CreateWidgets();

protected:
	void CreateToolbar();
	void BindToolbarCommands();
	void RegisterDelegates();
	void UnregisterDelegates() const;

private:
	void TestUICommandCall() const;
	static bool CanTestUICommandCall();

	void OnGraphSampleNodeChanged() const;
};
