// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Node/TestGraphSampleChildNode.h"

#if WITH_EDITOR
FString UTestGraphSampleChildNode::GetNodeCategory() const
{
	return TEXT("ChildNodeCategory");
}

FText UTestGraphSampleChildNode::GetNodeTitle() const
{
	return FText::FromString(TEXT("ChildNodeTitle"));
}

FText UTestGraphSampleChildNode::GetNodeToolTip() const
{
	return FText::FromString(TEXT("ChildNodeToolTip"));
}
#endif
