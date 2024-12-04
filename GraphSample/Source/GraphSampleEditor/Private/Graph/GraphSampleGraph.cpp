// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Graph/GraphSampleGraph.h"

#include "GraphSampleAsset.h"
#include "Graph/GraphSampleGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

UEdGraph* UGraphSampleGraph::CreateGraph(UGraphSampleAsset* InGraphSampleAsset)
{
	return CreateGraph(InGraphSampleAsset, UGraphSampleGraphSchema::StaticClass());
}

UEdGraph* UGraphSampleGraph::CreateGraph(UGraphSampleAsset* InGraphSampleAsset, const TSubclassOf<UGraphSampleGraphSchema>& GraphSampleSchema)
{
	check(GraphSampleSchema);
	UEdGraph* NewGraph = CastChecked<UGraphSampleGraph>(FBlueprintEditorUtils::CreateNewGraph(InGraphSampleAsset, NAME_None, StaticClass(), GraphSampleSchema));
	NewGraph->bAllowDeletion = false;
	InGraphSampleAsset->GraphSampleGraph = NewGraph;
	NewGraph->GetSchema()->CreateDefaultNodesForGraph(*NewGraph);
	return NewGraph;
}

UGraphSampleAsset* UGraphSampleGraph::GetGraphSampleAsset() const
{
	return GetTypedOuter<UGraphSampleAsset>();
}

void UGraphSampleGraph::OnGraphSampleNodeChanged()
{
	NotifyGraphChanged();
}
