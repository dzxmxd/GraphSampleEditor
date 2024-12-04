// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Graph/Node/GraphSampleGraphNode_Reroute.h"

#include "SGraphNodeKnot.h"

TSharedPtr<SGraphNode> UGraphSampleGraphNode_Reroute::CreateVisualWidget()
{
	return SNew(SGraphNodeKnot, this);
}

bool UGraphSampleGraphNode_Reroute::ShouldDrawNodeAsControlPointOnly(int32& OutInputPinIndex, int32& OutOutputPinIndex) const
{
	OutInputPinIndex = 0;
	OutOutputPinIndex = 1;
	return true;
}

UEdGraphPin* UGraphSampleGraphNode_Reroute::GetInputPin() const
{
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Input)
		{
			return Pin;
		}
	}

	return nullptr;
}

UEdGraphPin* UGraphSampleGraphNode_Reroute::GetOutputPin() const
{
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output)
		{
			return Pin;
		}
	}

	return nullptr;
}
