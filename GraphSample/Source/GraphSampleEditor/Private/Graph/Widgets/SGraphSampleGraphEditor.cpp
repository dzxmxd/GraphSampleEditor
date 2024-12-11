// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Graph/Widgets/SGraphSampleGraphEditor.h"

#include "GraphSampleAsset.h"
#include "EdGraphUtilities.h"
#include "GraphEditorActions.h"
#include "SNodePanel.h"
#include "Asset/GraphSampleAssetEditor.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/GraphSampleGraphSchema_Actions.h"
#include "Graph/Node/GraphSampleGraphNode.h"
#include "HAL/PlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "GraphSampleGraphEditor"

class UGraphSampleGraphNode;

void SGraphSampleGraphEditor::Construct(const FArguments& InArgs, const TSharedPtr<FGraphSampleAssetEditor> InAssetEditor)
{
	GraphSampleAssetEditor = InAssetEditor;
	GraphSampleAsset = GraphSampleAssetEditor.Pin()->GetGraphSampleAsset();

	DetailsView = InArgs._DetailsView;

	BindGraphCommands();

	SGraphEditor::FArguments Arguments;
	Arguments._AdditionalCommands = CommandList;
	Arguments._Appearance = GetGraphAppearanceInfo();
	Arguments._GraphToEdit = GraphSampleAsset->GetGraph();
	Arguments._GraphEvents = InArgs._GraphEvents;
	Arguments._GraphEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &SGraphSampleGraphEditor::OnNodeDoubleClicked);
	Arguments._AutoExpandActionMenu = true;
	Arguments._GraphEvents.OnSelectionChanged = FOnSelectionChanged::CreateSP(this, &SGraphSampleGraphEditor::OnSelectedNodesChanged);

	SGraphEditor::Construct(Arguments);
}

void SGraphSampleGraphEditor::BindGraphCommands()
{
	FGraphEditorCommands::Register();

	const FGenericCommands& GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl& GraphEditorCommands = FGraphEditorCommands::Get();

	CommandList = MakeShareable(new FUICommandList);

	// Graph commands
	CommandList->MapAction(GraphEditorCommands.CreateComment,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnCreateComment),
		FCanExecuteAction::CreateStatic(&SGraphSampleGraphEditor::CanEdit));

	CommandList->MapAction(GraphEditorCommands.StraightenConnections,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnStraightenConnections));

	CommandList->MapAction(GraphEditorCommands.DeleteAndReconnectNodes,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CanDeleteNodes));

	// Generic Node commands
	CommandList->MapAction(GenericCommands.Undo,
		FExecuteAction::CreateStatic(&SGraphSampleGraphEditor::UndoGraphAction),
		FCanExecuteAction::CreateStatic(&SGraphSampleGraphEditor::CanEdit));

	CommandList->MapAction(GenericCommands.Redo,
		FExecuteAction::CreateStatic(&SGraphSampleGraphEditor::RedoGraphAction),
		FCanExecuteAction::CreateStatic(&SGraphSampleGraphEditor::CanEdit));

	CommandList->MapAction(GenericCommands.SelectAll,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::SelectAllNodes),
		FCanExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CanSelectAllNodes));

	CommandList->MapAction(GenericCommands.Delete,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CanDeleteNodes));

	CommandList->MapAction(GenericCommands.Copy,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CopySelectedNodes),
		FCanExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CanCopyNodes));

	CommandList->MapAction(GenericCommands.Cut,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CutSelectedNodes),
		FCanExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CanCutNodes));

	CommandList->MapAction(GenericCommands.Paste,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::PasteNodes),
		FCanExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CanPasteNodes));

	CommandList->MapAction(GenericCommands.Duplicate,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::DuplicateNodes),
		FCanExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::CanDuplicateNodes));

	// Organisation Commands
	CommandList->MapAction(GraphEditorCommands.AlignNodesTop,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnAlignTop));

	CommandList->MapAction(GraphEditorCommands.AlignNodesMiddle,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnAlignMiddle));

	CommandList->MapAction(GraphEditorCommands.AlignNodesBottom,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnAlignBottom));

	CommandList->MapAction(GraphEditorCommands.AlignNodesLeft,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnAlignLeft));

	CommandList->MapAction(GraphEditorCommands.AlignNodesCenter,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnAlignCenter));

	CommandList->MapAction(GraphEditorCommands.AlignNodesRight,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnAlignRight));

	CommandList->MapAction(GraphEditorCommands.StraightenConnections,
		FExecuteAction::CreateSP(this, &SGraphSampleGraphEditor::OnStraightenConnections));
}

FGraphAppearanceInfo SGraphSampleGraphEditor::GetGraphAppearanceInfo() const
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = GetCornerText();
	return AppearanceInfo;
}

FText SGraphSampleGraphEditor::GetCornerText() const
{
	return LOCTEXT("AppearanceCornerText_GraphSampleAsset", "GraphSample");
}

void SGraphSampleGraphEditor::SelectSingleNode(UEdGraphNode* Node)
{
	ClearSelectionSet();
	SetNodeSelection(Node, true);
}

void SGraphSampleGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& Nodes)
{
	TArray<UObject*> SelectedObjects = Nodes.Array();

	if (Nodes.IsEmpty())
	{
		SelectedObjects.Add(GraphSampleAsset.Get());
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObjects(SelectedObjects);
	}
}

bool SGraphSampleGraphEditor::IsTabFocused() const
{
	return GraphSampleAssetEditor.Pin()->IsTabFocused(FGraphSampleAssetEditor::GraphTab);
}

void SGraphSampleGraphEditor::OnCreateComment() const
{
	FGraphSampleGraphSchemaAction_NewComment CommentAction;
	CommentAction.PerformAction(GraphSampleAsset->GetGraph(), nullptr, GetPasteLocation());
}

bool SGraphSampleGraphEditor::CanEdit()
{
	return GEditor->PlayWorld == nullptr;
}

void SGraphSampleGraphEditor::DeleteSelectedNodes()
{
	const FScopedTransaction Transaction(LOCTEXT("DeleteSelectedNode", "Delete Selected Node"));
	GetCurrentGraph()->Modify();
	GraphSampleAsset->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);
		if (Node->CanUserDeleteNode())
		{
			if (const UGraphSampleGraphNode* GraphSampleGraphNode = Cast<UGraphSampleGraphNode>(Node))
			{
				// Possible need to organize connections here
			}

			GetCurrentGraph()->GetSchema()->BreakNodeLinks(*Node);
			Node->DestroyNode();
		}
	}
}

void SGraphSampleGraphEditor::DeleteSelectedDuplicableNodes()
{
	// Cache off the old selection
	const FGraphPanelSelectionSet OldSelectedNodes = GetSelectedNodes();

	// Clear the selection and only select the nodes that can be duplicated
	FGraphPanelSelectionSet RemainingNodes;
	ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIt(OldSelectedNodes); SelectedIt; ++SelectedIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIt))
		{
			if (Node->CanDuplicateNode())
			{
				SetNodeSelection(Node, true);
			}
			else
			{
				RemainingNodes.Add(Node);
			}
		}
	}

	// Delete the duplicable nodes
	DeleteSelectedNodes();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIt(RemainingNodes); SelectedIt; ++SelectedIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIt))
		{
			SetNodeSelection(Node, true);
		}
	}
}

bool SGraphSampleGraphEditor::CanDeleteNodes() const
{
	if (CanEdit() && IsTabFocused())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			if (const UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
			{
				if (!Node->CanUserDeleteNode())
				{
					return false;
				}
			}
		}

		return SelectedNodes.Num() > 0;
	}

	return false;
}

void SGraphSampleGraphEditor::UndoGraphAction()
{
	GEditor->UndoTransaction();
}

void SGraphSampleGraphEditor::RedoGraphAction()
{
	GEditor->RedoTransaction();
}

bool SGraphSampleGraphEditor::CanSelectAllNodes() const
{
	return true;
}

void SGraphSampleGraphEditor::CopySelectedNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIt(SelectedNodes); SelectedIt; ++SelectedIt)
	{
		if (UGraphSampleGraphNode* Node = Cast<UGraphSampleGraphNode>(*SelectedIt))
		{
			Node->PrepareForCopying();
		}
	}

	// Export the selected nodes and place the text on the clipboard
	FString ExportedText;
	FEdGraphUtilities::ExportNodesToText(SelectedNodes, /*out*/ ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool SGraphSampleGraphEditor::CanCopyNodes() const
{
	if (CanEdit() && IsTabFocused())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator SelectedIt(SelectedNodes); SelectedIt; ++SelectedIt)
		{
			const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIt);
			if (Node && Node->CanDuplicateNode())
			{
				return true;
			}
		}
	}

	return false;
}

void SGraphSampleGraphEditor::CutSelectedNodes()
{
	CopySelectedNodes();

	// Cut should only delete nodes that can be duplicated
	DeleteSelectedDuplicableNodes();
}

bool SGraphSampleGraphEditor::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void SGraphSampleGraphEditor::PasteNodes()
{
	PasteNodesHere(GetPasteLocation());
}

void SGraphSampleGraphEditor::PasteNodesHere(const FVector2D& Location)
{
	// Undo/Redo support
	const FScopedTransaction Transaction(LOCTEXT("PasteNode", "Paste Node"));
	GraphSampleAsset->GetGraph()->Modify();
	GraphSampleAsset->Modify();

	// Clear the selection set (newly pasted stuff will be selected)
	ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(GraphSampleAsset->GetGraph(), TextToImport, /*out*/ PastedNodes);

	// Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f, 0.0f);

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		const UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}

	if (PastedNodes.Num() > 0)
	{
		const float InvNumNodes = 1.0f / static_cast<float>(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;

		// Give new node a different Guid from the old one
		Node->CreateNewGuid();

		// Select the newly pasted stuff
		SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

		Node->SnapToGrid(SNodePanel::GetSnapGridSize());
	}

	// Update UI
	NotifyGraphChanged();

	GraphSampleAsset->PostEditChange();
	GraphSampleAsset->MarkPackageDirty();
}

bool SGraphSampleGraphEditor::CanPasteNodes() const
{
	if (CanEdit() && IsTabFocused())
	{
		FString ClipboardContent;
		FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

		return FEdGraphUtilities::CanImportNodesFromText(GraphSampleAsset->GetGraph(), ClipboardContent);
	}

	return false;
}

void SGraphSampleGraphEditor::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool SGraphSampleGraphEditor::CanDuplicateNodes() const
{
	return CanCopyNodes();
}

void SGraphSampleGraphEditor::OnNodeDoubleClicked(UEdGraphNode* EdGraphNode) const
{
	if (const UGraphSampleGraphNode* GraphSampleGraphNode = Cast<UGraphSampleGraphNode>(EdGraphNode))
	{
		GraphSampleGraphNode->OnNodeDoubleClicked();
	}
}

#undef LOCTEXT_NAMESPACE
