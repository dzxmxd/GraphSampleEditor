// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Graph/GraphSampleGraphSchema_Actions.h"

#include "GraphSampleAsset.h"
#include "GraphSampleEditorHelper.h"
#include "EdGraphNode_Comment.h"
#include "Graph/GraphSampleGraph.h"
#include "Graph/Widgets/SGraphSampleGraphEditor.h"

#define LOCTEXT_NAMESPACE "GraphSampleGraphSchema_Actions"

UEdGraphNode* FGraphSampleGraphSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /* = true*/)
{
	// prevent adding new nodes while playing
	if (GEditor->PlayWorld != nullptr)
	{
		return nullptr;
	}

	if (NodeClass)
	{
		return CreateNode(ParentGraph, FromPin, NodeClass, Location, bSelectNewNode);
	}

	return nullptr;
}

UGraphSampleGraphNode* FGraphSampleGraphSchemaAction_NewNode::CreateNode(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const UClass* NodeClass, const FVector2D Location, const bool bSelectNewNode /*= true*/)
{
	const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Node"));

	if (NodeClass == nullptr)
	{
		NodeClass = UGraphSampleNode::StaticClass();
	}

	ParentGraph->Modify();
	if (FromPin)
	{
		FromPin->Modify();
	}

	UGraphSampleAsset* GraphSampleAsset = CastChecked<UGraphSampleGraph>(ParentGraph)->GetGraphSampleAsset();
	GraphSampleAsset->Modify();

	// create new Graph node
	// If there are multiple graph nodes, need to be processed input uclass here
	UGraphSampleGraphNode* NewGraphNode = NewObject<UGraphSampleGraphNode>(ParentGraph, UGraphSampleGraphNode::StaticClass(), NAME_None, RF_Transactional);
	UGraphSampleNode* NewGraphSampleNode = NewObject<UGraphSampleNode>(GraphSampleAsset, NodeClass, NAME_None, RF_Transactional);
	NewGraphNode->SetGraphSampleNode(NewGraphSampleNode);

	// register to the graph
	NewGraphNode->CreateNewGuid();
	ParentGraph->AddNode(NewGraphNode, false, bSelectNewNode);

	// create pins and connections
	NewGraphNode->AllocateDefaultPins();
	NewGraphNode->AutowireNewNode(FromPin);

	// set position
	NewGraphNode->NodePosX = Location.X;
	NewGraphNode->NodePosY = Location.Y;

	// call notifies
	NewGraphNode->PostPlacedNewNode();
	ParentGraph->NotifyGraphChanged();

	GraphSampleAsset->PostEditChange();

	// select in editor UI
	if (bSelectNewNode)
	{
		const TSharedPtr<SGraphSampleGraphEditor> GraphSampleGraphEditor = FGraphSampleEditorHelper::GetGraphSampleGraphEditor(ParentGraph);
		if (GraphSampleGraphEditor.IsValid())
		{
			GraphSampleGraphEditor->SelectSingleNode(NewGraphNode);
		}
	}

	return NewGraphNode;
}

/////////////////////////////////////////////////////
// Comment Node

UEdGraphNode* FGraphSampleGraphSchemaAction_NewComment::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, const bool bSelectNewNode/* = true*/)
{
	// prevent adding new nodes while playing
	if (GEditor->PlayWorld != nullptr)
	{
		return nullptr;
	}

	UEdGraphNode_Comment* CommentTemplate = NewObject<UEdGraphNode_Comment>();
	FVector2D SpawnLocation = Location;

	const TSharedPtr<SGraphSampleGraphEditor> GraphSampleGraphEditor = FGraphSampleEditorHelper::GetGraphSampleGraphEditor(ParentGraph);
	if (GraphSampleGraphEditor.IsValid())
	{
		FSlateRect Bounds;
		if (GraphSampleGraphEditor->GetBoundsForSelectedNodes(Bounds, 50.0f))
		{
			CommentTemplate->SetBounds(Bounds);
			SpawnLocation.X = CommentTemplate->NodePosX;
			SpawnLocation.Y = CommentTemplate->NodePosY;
		}
	}

	return FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(ParentGraph, CommentTemplate, SpawnLocation);
}

#undef LOCTEXT_NAMESPACE
