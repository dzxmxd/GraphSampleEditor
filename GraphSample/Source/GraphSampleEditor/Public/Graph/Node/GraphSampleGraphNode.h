// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "GraphSampleGraphNode.generated.h"

enum EGraphSampleNodeState : uint8;
class UGraphSampleAsset;
class UGraphSampleNode;

UCLASS()
class GRAPHSAMPLEEDITOR_API UGraphSampleGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
	UGraphSampleNode* GraphSampleNode;

	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	void InsertNewNode(UEdGraphPin* FromPin, UEdGraphPin* NewLinkPin, TSet<UEdGraphNode*>& OutNodeList);
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

	void SetGraphSampleNode(UGraphSampleNode* InGraphSampleNode);
	UGraphSampleAsset* GetGraphSampleAsset() const;
	FString GetNodePopupInfo() const;
	void OnNodeDoubleClicked() const;
	EGraphSampleNodeState GetActivationState() const;
};