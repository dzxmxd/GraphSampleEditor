// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UGraphSampleGraphNode;

class GRAPHSAMPLEEDITOR_API SGraphSampleGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphSampleGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UGraphSampleGraphNode* InNode);

protected:
	virtual const FSlateBrush* GetShadowBrush(bool bSelected) const override;
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	virtual void UpdateGraphNode() override;
	const FSlateBrush* GetNodeTitleIcon() const;

protected:
	UGraphSampleGraphNode* GraphSampleGraphNode = nullptr;
};