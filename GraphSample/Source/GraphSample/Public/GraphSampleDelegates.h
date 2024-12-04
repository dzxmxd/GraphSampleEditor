// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"

namespace GraphSample::GraphSampleNode::Delegates
{
#if WITH_EDITOR
	DECLARE_MULTICAST_DELEGATE(FOnGraphSampleNodeChanged);
	extern GRAPHSAMPLE_API FOnGraphSampleNodeChanged OnGraphSampleNodeChanged;
#endif
}; // GraphSample::GraphSampleNode::Delegates