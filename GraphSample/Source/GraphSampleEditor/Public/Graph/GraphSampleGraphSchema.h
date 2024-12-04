// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "GraphSampleGraphSchema.generated.h"

DECLARE_MULTICAST_DELEGATE(FGraphSampleGraphSchemaRefresh);

UCLASS()
class GRAPHSAMPLEEDITOR_API UGraphSampleGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	// EdGraphSchema
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool ShouldHidePinDefaultValue(UEdGraphPin* Pin) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual FText GetPinDisplayName(const UEdGraphPin* Pin) const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	virtual int32 GetNodeSelectionCount(const UEdGraph* Graph) const override;
	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	virtual void OnPinConnectionDoubleCicked(UEdGraphPin* PinA, UEdGraphPin* PinB, const FVector2D& GraphPosition) const override;
	// --

public:
	static FGraphSampleGraphSchemaRefresh OnNodeListChanged;

	static void GetCommentAction(FGraphActionMenuBuilder& ActionMenuBuilder, const UEdGraph* CurrentGraph = nullptr);
	static TArray<TSharedPtr<FString>> GetGraphSampleNodeCategories();
	static void GetPaletteActions(FGraphActionMenuBuilder& ActionMenuBuilder, const FString& CategoryName);
	static void GetGraphSampleNodeActions(FGraphActionMenuBuilder& ActionMenuBuilder, const FString& CategoryName);
	static UClass* GetMappingGraphNodeClass(const UClass* GraphSampleNodeClass);
};