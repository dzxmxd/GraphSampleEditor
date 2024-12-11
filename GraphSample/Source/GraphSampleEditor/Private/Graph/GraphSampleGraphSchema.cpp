// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Graph/GraphSampleGraphSchema.h"

#include "GraphSampleAsset.h"
#include "GraphSampleEditorHelper.h"
#include "Graph/GraphSampleGraph.h"
#include "Graph/GraphSampleGraphSchema_Actions.h"
#include "Graph/Node/GraphSampleGraphNode_Reroute.h"
#include "Graph/Widgets/SGraphSampleGraphEditor.h"
#include "Node/GraphSampleNode_Reroute.h"

#define LOCTEXT_NAMESPACE "GraphSampleGraphSchema"

FGraphSampleGraphSchemaRefresh UGraphSampleGraphSchema::OnNodeListChanged;

void UGraphSampleGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	GetGraphSampleNodeActions(ContextMenuBuilder, FString());
	GetCommentAction(ContextMenuBuilder, ContextMenuBuilder.CurrentGraph);
}

void UGraphSampleGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	UGraphSampleGraphNode* NewGraphNode = FGraphSampleGraphSchemaAction_NewNode::CreateNode(&Graph, nullptr, nullptr, FVector2D::ZeroVector);
	SetNodeMetaData(NewGraphNode, FNodeMetadata::DefaultGraphNode);

	if (const UGraphSampleGraph* GraphSampleGraph = Cast<UGraphSampleGraph>(&Graph))
	{
		if (UGraphSampleAsset* CurAsset = GraphSampleGraph->GetGraphSampleAsset())
		{
			CurAsset->RootNode = NewGraphNode->GraphSampleNode;
		}
	}
	// NewGraphNode->MakeAutomaticallyPlacedGhostNode();
}

const FPinConnectionResponse UGraphSampleGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	const UGraphSampleGraphNode* OwningNodeA = Cast<UGraphSampleGraphNode>(PinA->GetOwningNodeUnchecked());
	const UGraphSampleGraphNode* OwningNodeB = Cast<UGraphSampleGraphNode>(PinB->GetOwningNodeUnchecked());

	if (!OwningNodeA || !OwningNodeB)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid nodes"));
	}

	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both are on the same node"));
	}

	if (PinA->bOrphanedPin || PinB->bOrphanedPin)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot make new connections to orphaned pin"));
	}

	// Compare the directions
	const UEdGraphPin* InputPin = nullptr;
	const UEdGraphPin* OutputPin = nullptr;

	if (!CategorizePinsByDirection(PinA, PinB, /*out*/ InputPin, /*out*/ OutputPin))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Directions are not compatible"));
	}

	// Break existing connections on Inputs only
	if (InputPin->LinkedTo.Num() > 0)
	{
		const ECanCreateConnectionResponse ReplyBreakInputs = (InputPin == PinA ? CONNECT_RESPONSE_BREAK_OTHERS_A : CONNECT_RESPONSE_BREAK_OTHERS_B);
		return FPinConnectionResponse(ReplyBreakInputs, TEXT("Replace existing connections"));
	}

	// Composite allow all connections
	if (const UGraphSampleGraphNode* OutputNode = Cast<UGraphSampleGraphNode>(OutputPin->GetOwningNodeUnchecked()))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Composite allow all connections"));
	}

	// Break existing connections on outputs only - multiple input connections are acceptable
	if (OutputPin->LinkedTo.Num() > 0)
	{
		const ECanCreateConnectionResponse ReplyBreakInputs = (OutputPin == PinA ? CONNECT_RESPONSE_BREAK_OTHERS_A : CONNECT_RESPONSE_BREAK_OTHERS_B);
		return FPinConnectionResponse(ReplyBreakInputs, TEXT("Replace existing connections"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool UGraphSampleGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	const bool bModified = UEdGraphSchema::TryCreateConnection(A, B);

	if (bModified)
	{
		A->GetOwningNode()->GetGraph()->NotifyGraphChanged();
	}

	return bModified;
}

bool UGraphSampleGraphSchema::ShouldHidePinDefaultValue(UEdGraphPin* Pin) const
{
	return false;
}

FLinearColor UGraphSampleGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

FText UGraphSampleGraphSchema::GetPinDisplayName(const UEdGraphPin* Pin) const
{
	return Super::GetPinDisplayName(Pin);
}

void UGraphSampleGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	Super::BreakNodeLinks(TargetNode);
}

void UGraphSampleGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
}

int32 UGraphSampleGraphSchema::GetNodeSelectionCount(const UEdGraph* Graph) const
{
	return FGraphSampleEditorHelper::GetGraphSampleGraphEditor(Graph)->GetNumberOfSelectedNodes();
}

TSharedPtr<FEdGraphSchemaAction> UGraphSampleGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FGraphSampleGraphSchemaAction_NewComment));
}

void UGraphSampleGraphSchema::OnPinConnectionDoubleCicked(UEdGraphPin* PinA, UEdGraphPin* PinB, const FVector2D& GraphPosition) const
{
	const FScopedTransaction Transaction(LOCTEXT("CreateGraphSampleRerouteNodeOnWire", "Create GraphSample Reroute Node"));

	const FVector2D NodeSpacerSize(42.0f, 24.0f);
	const FVector2D KnotTopLeft = GraphPosition - (NodeSpacerSize * 0.5f);

	UEdGraph* ParentGraph = PinA->GetOwningNode()->GetGraph();

	if (const UGraphSampleGraphNode* NewGraphNode =
		FGraphSampleGraphSchemaAction_NewNode::CreateNode(ParentGraph, nullptr, UGraphSampleNode_Reroute::StaticClass(), KnotTopLeft, false))
	{
		if (const UGraphSampleGraphNode_Reroute* NewRerouteGraphNode = Cast<UGraphSampleGraphNode_Reroute>(NewGraphNode))
		{
			TryCreateConnection(PinA, (PinA->Direction == EGPD_Output) ? NewRerouteGraphNode->GetInputPin() : NewRerouteGraphNode->GetOutputPin());
			TryCreateConnection(PinB, (PinB->Direction == EGPD_Output) ? NewRerouteGraphNode->GetInputPin() : NewRerouteGraphNode->GetOutputPin());
		}
	}
}

void UGraphSampleGraphSchema::GetCommentAction(FGraphActionMenuBuilder& ActionMenuBuilder, const UEdGraph* CurrentGraph)
{
	if (!ActionMenuBuilder.FromPin)
	{
		const FText MenuDescription = LOCTEXT("AddCommentAction", "Add Comment...");
		const FText ToolTip = LOCTEXT("CreateCommentToolTip", "Creates a comment.");

		const TSharedPtr<FGraphSampleGraphSchemaAction_NewComment> NewAction(new FGraphSampleGraphSchemaAction_NewComment(FText::GetEmpty(), MenuDescription, ToolTip, 0));
		ActionMenuBuilder.AddAction(NewAction);
	}
}

TArray<TSharedPtr<FString>> UGraphSampleGraphSchema::GetGraphSampleNodeCategories()
{
	TSet<FString> UnsortedCategories;
	TArray<UClass*> GraphSampleNodes;
	GetDerivedClasses(UGraphSampleNode::StaticClass(), GraphSampleNodes);
	for (const UClass* FocusesClass : GraphSampleNodes)
	{
		if (const UGraphSampleNode* FocusesGraphNode = FocusesClass->GetDefaultObject<UGraphSampleNode>())
		{
			UnsortedCategories.Emplace(FocusesGraphNode->GetNodeCategory());
		}
	}

	TArray<FString> SortedCategories = UnsortedCategories.Array();
	SortedCategories.Sort();

	// Maybe need add BP extension categories here

	// create list of categories
	TArray<TSharedPtr<FString>> Result;
	for (const FString& Category : SortedCategories)
	{
		if (!Category.IsEmpty())
		{
			Result.Emplace(MakeShareable(new FString(Category)));
		}
	}

	return Result;
}

void UGraphSampleGraphSchema::GetPaletteActions(FGraphActionMenuBuilder& ActionMenuBuilder, const FString& CategoryName)
{
	GetGraphSampleNodeActions(ActionMenuBuilder, CategoryName);
	GetCommentAction(ActionMenuBuilder);
}

void UGraphSampleGraphSchema::GetGraphSampleNodeActions(FGraphActionMenuBuilder& ActionMenuBuilder, const FString& CategoryName)
{
	// In most cases, base node considered as an abstract node, for test add base node here
	if (const UGraphSampleNode* BaseGraphNode = UGraphSampleNode::StaticClass()->GetDefaultObject<UGraphSampleNode>())
	{
		const TSharedPtr<FGraphSampleGraphSchemaAction_NewNode> NewNodeAction(new FGraphSampleGraphSchemaAction_NewNode(BaseGraphNode));
		ActionMenuBuilder.AddAction(NewNodeAction);
	}

	TArray<UClass*> GraphSampleNodes;
	GetDerivedClasses(UGraphSampleNode::StaticClass(), GraphSampleNodes);
	for (const UClass* FocusesClass : GraphSampleNodes)
	{
		if (const UGraphSampleNode* FocusesGraphNode = FocusesClass->GetDefaultObject<UGraphSampleNode>())
		{
			if (CategoryName.IsEmpty() || CategoryName.Equals(FocusesGraphNode->GetNodeCategory()))
			{
				const TSharedPtr<FGraphSampleGraphSchemaAction_NewNode> NewNodeAction(new FGraphSampleGraphSchemaAction_NewNode(FocusesGraphNode));
				ActionMenuBuilder.AddAction(NewNodeAction);
			}
		}
	}

	// Maybe need add BP extension categories here
}

UClass* UGraphSampleGraphSchema::GetMappingGraphNodeClass(const UClass* GraphSampleNodeClass)
{
	// You can also use TMap to map GraphNodeClass and NodeClass associations
	if (GraphSampleNodeClass == UGraphSampleNode_Reroute::StaticClass())
	{
		return UGraphSampleGraphNode_Reroute::StaticClass();
	}
	return UGraphSampleGraphNode::StaticClass();
}
#undef LOCTEXT_NAMESPACE // "GraphSampleGraphSchema"