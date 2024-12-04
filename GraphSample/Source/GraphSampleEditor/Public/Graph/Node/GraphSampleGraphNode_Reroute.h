// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "GraphSampleGraphNode.h"
#include "GraphSampleGraphNode_Reroute.generated.h"

/** When runtime logic is added, passthrough is sufficient here */
UCLASS()
class GRAPHSAMPLEEDITOR_API UGraphSampleGraphNode_Reroute : public UGraphSampleGraphNode
{
	GENERATED_BODY()

	// UEdGraphNode
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual bool ShouldDrawNodeAsControlPointOnly(int32& OutInputPinIndex, int32& OutOutputPinIndex) const override;
	// --

public:
	UEdGraphPin* GetInputPin() const;
	UEdGraphPin* GetOutputPin() const;
};
