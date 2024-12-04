// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "GraphSampleEditorHelper.h"

#include "Asset/GraphSampleAssetEditor.h"
#include "Graph/GraphSampleGraph.h"
#include "GraphSampleAsset.h"
#include "Graph/Node/GraphSampleGraphNode.h"
#include "Node/GraphSampleNode.h"
#include "Toolkits/ToolkitManager.h"

#define LOCTEXT_NAMESPACE "GraphSampleEditorHelper"

TSharedPtr<FGraphSampleAssetEditor> FGraphSampleEditorHelper::GetGraphSampleAssetEditor(const UEdGraph* Graph)
{
	check(Graph);

	TSharedPtr<FGraphSampleAssetEditor> GraphSampleAssetEditor;
	if (const UGraphSampleAsset* GraphSampleAsset = Cast<const UGraphSampleGraph>(Graph)->GetGraphSampleAsset())
	{
		const TSharedPtr<IToolkit> FoundAssetEditor = FToolkitManager::Get().FindEditorForAsset(GraphSampleAsset);
		if (FoundAssetEditor.IsValid())
		{
			GraphSampleAssetEditor = StaticCastSharedPtr<FGraphSampleAssetEditor>(FoundAssetEditor);
		}
	}
	return GraphSampleAssetEditor;
}

TSharedPtr<SGraphSampleGraphEditor> FGraphSampleEditorHelper::GetGraphSampleGraphEditor(const UEdGraph* Graph)
{
	TSharedPtr<SGraphSampleGraphEditor> GraphSampleGraphEditor;

	const TSharedPtr<FGraphSampleAssetEditor> GraphSampleEditor = GetGraphSampleAssetEditor(Graph);
	if (GraphSampleEditor.IsValid())
	{
		GraphSampleGraphEditor = GraphSampleEditor->GetGraphSampleGraph();
	}

	return GraphSampleGraphEditor;
}

#undef LOCTEXT_NAMESPACE
