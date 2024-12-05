// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Node/GraphSampleNode_Reroute.h"

#if WITH_EDITOR
FString UGraphSampleNode_Reroute::GetNodeCategory() const
{
	return TEXT("Reroute");
}

FText UGraphSampleNode_Reroute::GetNodeTitle() const
{
	return FText::FromString(TEXT("Reroute"));
}

FText UGraphSampleNode_Reroute::GetNodeToolTip() const
{
	return FText::FromString(TEXT("Reroute"));
}
#endif