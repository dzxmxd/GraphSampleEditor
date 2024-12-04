// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "GraphSampleNode.h"
#include "TestGraphSampleChildNode.generated.h"

UCLASS()
class GRAPHSAMPLE_API UTestGraphSampleChildNode : public UGraphSampleNode
{
	GENERATED_BODY()

#if WITH_EDITOR
	virtual FString GetNodeCategory() const override;
	virtual FText GetNodeTitle() const override;
	virtual FText GetNodeToolTip() const override;
#endif
};