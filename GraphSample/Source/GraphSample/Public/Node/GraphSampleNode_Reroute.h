// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "GraphSampleNode.h"
#include "GraphSampleNode_Reroute.generated.h"

UCLASS(NotBlueprintable, meta = (DisplayName = "Reroute"))
class GRAPHSAMPLE_API UGraphSampleNode_Reroute : public UGraphSampleNode
{
	GENERATED_BODY()

#if WITH_EDITOR
	virtual FString GetNodeCategory() const;
	virtual FText GetNodeTitle() const;
	virtual FText GetNodeToolTip() const;
#endif
};