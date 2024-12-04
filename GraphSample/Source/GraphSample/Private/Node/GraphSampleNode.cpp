// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Node/GraphSampleNode.h"
#include "GraphSampleDelegates.h"

#if WITH_EDITOR
void UGraphSampleNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	if (true /*PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGraphSampleNode, Children)*/)
	{
		GraphSample::GraphSampleNode::Delegates::OnGraphSampleNodeChanged.Broadcast();
	}
}

FString UGraphSampleNode::GetNodeCategory() const
{
	return TEXT("DefaultCategory");
}

FText UGraphSampleNode::GetNodeTitle() const
{
	return FText::FromString(TEXT("DefaultTitle"));
}

FText UGraphSampleNode::GetNodeToolTip() const
{
	return FText::FromString(TEXT("DefaultToolTip"));
}

EGraphSampleNodeState UGraphSampleNode::GetActivationState() const
{
	return ActivationState;
}
#endif