// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "GraphEditor.h"
#include "CoreMinimal.h"

class FGraphSampleAssetEditor;
class UGraphSampleAsset;

class GRAPHSAMPLEEDITOR_API SGraphSampleGraphEditor : public SGraphEditor
{
public:
	SLATE_BEGIN_ARGS(SGraphSampleGraphEditor) {}
		SLATE_ARGUMENT(SGraphEditor::FGraphEditorEvents, GraphEvents)
		SLATE_ARGUMENT(TSharedPtr<IDetailsView>, DetailsView)
	SLATE_END_ARGS()

protected:
	TWeakObjectPtr<UGraphSampleAsset> GraphSampleAsset;
	TWeakPtr<FGraphSampleAssetEditor> GraphSampleAssetEditor;
	TSharedPtr<IDetailsView> DetailsView;
	TSharedPtr<FUICommandList> CommandList;

public:
	void Construct(const FArguments& InArgs, const TSharedPtr<FGraphSampleAssetEditor> InAssetEditor);
	void BindGraphCommands();
	FGraphAppearanceInfo GetGraphAppearanceInfo() const;
	FText GetCornerText() const;
	void SelectSingleNode(UEdGraphNode* Node);
	void OnSelectedNodesChanged(const TSet<UObject*>& Nodes);
	bool IsTabFocused() const;


	/** Callback */
	void OnCreateComment() const;
	static bool CanEdit();
	void DeleteSelectedNodes();
	void DeleteSelectedDuplicableNodes();
	bool CanDeleteNodes() const;
	static void UndoGraphAction();
	static void RedoGraphAction();
	bool CanSelectAllNodes() const;
	void CopySelectedNodes() const;
	bool CanCopyNodes() const;
	void CutSelectedNodes();
	bool CanCutNodes() const;
	void PasteNodes();
	void PasteNodesHere(const FVector2D& Location);
	bool CanPasteNodes() const;
	void DuplicateNodes();
	bool CanDuplicateNodes() const;
	void OnNodeDoubleClicked(UEdGraphNode* EdGraphNode) const;
};