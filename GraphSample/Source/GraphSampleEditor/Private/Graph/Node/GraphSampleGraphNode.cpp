// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Graph/Node/GraphSampleGraphNode.h"

#include "EdGraphSchema_K2.h"
#include "GraphEditorActions.h"
#include "SourceCodeNavigation.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/GraphSampleGraph.h"
#include "Graph/GraphSampleGraphSchema.h"
#include "Graph/Widgets/SGraphSampleGraphNode.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Node/GraphSampleNode.h"

#define LOCTEXT_NAMESPACE "GraphSampleGraphNode"

void UGraphSampleGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	const FGenericCommands& GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl& GraphCommands = FGraphEditorCommands::Get();

	if (Context->Pin)
	{
		return Super::GetNodeContextMenuActions(Menu, Context);
	}
	else if (Context->Node)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("GraphSampleGraphNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
			Section.AddMenuEntry(GenericCommands.Delete);
			Section.AddMenuEntry(GenericCommands.Cut);
			Section.AddMenuEntry(GenericCommands.Copy);
			Section.AddMenuEntry(GenericCommands.Duplicate);
			Section.AddMenuEntry(GraphCommands.BreakNodeLinks);
		}
		{
			FToolMenuSection& Section = Menu->AddSection("GraphSampleGraphNodeOrganisation", LOCTEXT("NodeOrganisation", "Organisation"));
			Section.AddSubMenu("Alignment", LOCTEXT("AlignmentHeader", "Alignment"), FText(), FNewToolMenuDelegate::CreateLambda([](UToolMenu* SubMenu)
			{
				FToolMenuSection& SubMenuSection = SubMenu->AddSection("EdGraphSchemaAlignment", LOCTEXT("AlignHeader", "Align"));
				SubMenuSection.AddMenuEntry(FGraphEditorCommands::Get().AlignNodesTop);
				SubMenuSection.AddMenuEntry(FGraphEditorCommands::Get().AlignNodesMiddle);
				SubMenuSection.AddMenuEntry(FGraphEditorCommands::Get().AlignNodesBottom);
				SubMenuSection.AddMenuEntry(FGraphEditorCommands::Get().AlignNodesLeft);
				SubMenuSection.AddMenuEntry(FGraphEditorCommands::Get().AlignNodesCenter);
				SubMenuSection.AddMenuEntry(FGraphEditorCommands::Get().AlignNodesRight);
				SubMenuSection.AddMenuEntry(FGraphEditorCommands::Get().StraightenConnections);
			}));
		}
	}
}

FText UGraphSampleGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (GraphSampleNode != nullptr)
	{
		return GraphSampleNode->GetNodeTitle();
	}
	return FText::GetEmpty();
}

void UGraphSampleGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, NAME_None);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, NAME_None);
}

FLinearColor UGraphSampleGraphNode::GetNodeTitleColor() const
{
	// Provide color registration here
	/*if (const FLinearColor* Color = UGraphSampleEditorSettings::Get()->NodeTypeColors.Find(GraphSampleNode->NodeType))
	{
		return *Color;
	}*/
	return Super::GetNodeTitleColor();
}

FSlateIcon UGraphSampleGraphNode::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon();
}

bool UGraphSampleGraphNode::CanUserDeleteNode() const
{
	if (GraphSampleNode == nullptr)
	{
		// Add some test here
		return true;
	}
	return true;
}

bool UGraphSampleGraphNode::CanDuplicateNode() const
{
	return CanUserDeleteNode();
}

void UGraphSampleGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	if (FromPin != nullptr)
	{
		const UGraphSampleGraphSchema* Schema = CastChecked<UGraphSampleGraphSchema>(GetSchema());

		TSet<UEdGraphNode*> NodeList;

		// auto-connect from dragged pin to first compatible pin on the new node
		for (UEdGraphPin* Pin : Pins)
		{
			check(Pin);
			FPinConnectionResponse Response = Schema->CanCreateConnection(FromPin, Pin);
			if (CONNECT_RESPONSE_MAKE == Response.Response)
			{
				if (Schema->TryCreateConnection(FromPin, Pin))
				{
					NodeList.Add(FromPin->GetOwningNode());
					NodeList.Add(this);
				}
				break;
			}
			else if (CONNECT_RESPONSE_BREAK_OTHERS_A == Response.Response)
			{
				InsertNewNode(FromPin, Pin, NodeList);
				break;
			}
		}

		// Send all nodes that received a new pin connection a notification
		for (auto It = NodeList.CreateConstIterator(); It; ++It)
		{
			UEdGraphNode* Node = (*It);
			Node->NodeConnectionListChanged();
		}
	}
}

void UGraphSampleGraphNode::InsertNewNode(UEdGraphPin* FromPin, UEdGraphPin* NewLinkPin, TSet<UEdGraphNode*>& OutNodeList)
{
	const UGraphSampleGraphSchema* Schema = CastChecked<UGraphSampleGraphSchema>(GetSchema());

	// The pin we are creating from already has a connection that needs to be broken. We want to "insert" the new node in between, so that the output of the new node is hooked up too
	UEdGraphPin* OldLinkedPin = FromPin->LinkedTo[0];
	check(OldLinkedPin);

	FromPin->BreakAllPinLinks();

	// Hook up the old linked pin to the first valid output pin on the new node
	for (int32 PinIndex = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		UEdGraphPin* OutputExecPin = Pins[PinIndex];
		check(OutputExecPin);
		if (CONNECT_RESPONSE_MAKE == Schema->CanCreateConnection(OldLinkedPin, OutputExecPin).Response)
		{
			if (Schema->TryCreateConnection(OldLinkedPin, OutputExecPin))
			{
				OutNodeList.Add(OldLinkedPin->GetOwningNode());
				OutNodeList.Add(this);
			}
			break;
		}
	}

	if (Schema->TryCreateConnection(FromPin, NewLinkPin))
	{
		OutNodeList.Add(FromPin->GetOwningNode());
		OutNodeList.Add(this);
	}
}

TSharedPtr<SGraphNode> UGraphSampleGraphNode::CreateVisualWidget()
{
	return SNew(SGraphSampleGraphNode, this);
}

void UGraphSampleGraphNode::SetGraphSampleNode(UGraphSampleNode* InGraphSampleNode)
{
	GraphSampleNode = InGraphSampleNode;
}

UGraphSampleAsset* UGraphSampleGraphNode::GetGraphSampleAsset() const
{
	UGraphSampleAsset* GraphSampleAsset = CastChecked<UGraphSampleGraph>(GetGraph())->GetGraphSampleAsset();
	return GraphSampleAsset;
}

FString UGraphSampleGraphNode::GetNodePopupInfo() const
{
	return GraphSampleNode ? GraphSampleNode->GetNodeCategory() : TEXT("");
}

void UGraphSampleGraphNode::OnNodeDoubleClicked() const
{
	if (GraphSampleNode != nullptr)
	{
		if (GraphSampleNode->GetClass()->IsNative())
		{
			if (FSourceCodeNavigation::CanNavigateToClass(GraphSampleNode->GetClass()))
			{
				if (FSourceCodeNavigation::NavigateToClass(GraphSampleNode->GetClass()))
				{
					return;
				}
			}

			// Failing that, fall back to the older method which will still get the file open assuming it exists
			FString NativeParentClassHeaderPath;
			if (FSourceCodeNavigation::FindClassHeaderPath(GraphSampleNode->GetClass(), NativeParentClassHeaderPath) && (IFileManager::Get().FileSize(*NativeParentClassHeaderPath) != INDEX_NONE))
			{
				const FString AbsNativeParentClassHeaderPath = FPaths::ConvertRelativePathToFull(NativeParentClassHeaderPath);
				FSourceCodeNavigation::OpenSourceFile(AbsNativeParentClassHeaderPath);
			}
		}
		else
		{
			FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GraphSampleNode->GetClass());
		}
	}
}

EGraphSampleNodeState UGraphSampleGraphNode::GetActivationState() const
{
	if (GraphSampleNode != nullptr)
	{
		// TODO(wxd) Maybe need to find a real execute instance.
		return GraphSampleNode->GetActivationState();
	}
	return EGraphSampleNodeState::Default;
}

#undef LOCTEXT_NAMESPACE
