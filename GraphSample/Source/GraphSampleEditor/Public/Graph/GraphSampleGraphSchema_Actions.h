// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "CoreMinimal.h"
#include "Node/GraphSampleGraphNode.h"
#include "Node/GraphSampleNode.h"
#include "GraphSampleGraphSchema_Actions.generated.h"

class UGraphSampleGraphNode;

USTRUCT()
struct GRAPHSAMPLEEDITOR_API FGraphSampleGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	UPROPERTY()
	class UClass* NodeClass;

	static FName StaticGetTypeId()
	{
		static FName Type("FGraphSampleGraphSchemaAction_NewNode");
		return Type;
	}

	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FGraphSampleGraphSchemaAction_NewNode()
		: FEdGraphSchemaAction()
		, NodeClass(nullptr) {}

	explicit FGraphSampleGraphSchemaAction_NewNode(UClass* Node)
		: FEdGraphSchemaAction()
		, NodeClass(Node) {}

	explicit FGraphSampleGraphSchemaAction_NewNode(const UGraphSampleNode* InNodeTemplate)
		: FEdGraphSchemaAction(FText::FromString(InNodeTemplate->GetNodeCategory()), InNodeTemplate->GetNodeTitle(), InNodeTemplate->GetNodeToolTip(), 0, FText::GetEmpty())
		, NodeClass(InNodeTemplate->GetClass())
	{
		// Default using title to search
		// SearchText = NodeTemplate->GetNodeTitle().ToString() + NodeTemplate->GetNodeToolTip().ToString();
	}

	// FEdGraphSchemaAction
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	// --

	static UGraphSampleGraphNode* CreateNode(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const UClass* NodeClass, const FVector2D Location, const bool bSelectNewNode = true);
};

USTRUCT()
struct GRAPHSAMPLEEDITOR_API FGraphSampleGraphSchemaAction_NewComment : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()

	// Simple type info
	static FName StaticGetTypeId()
	{
		static FName Type("FGraphSampleGraphSchemaAction_NewComment");
		return Type;
	}

	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FGraphSampleGraphSchemaAction_NewComment()
		: FEdGraphSchemaAction() {}

	FGraphSampleGraphSchemaAction_NewComment(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping) {}

	// FEdGraphSchemaAction
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	// --
};