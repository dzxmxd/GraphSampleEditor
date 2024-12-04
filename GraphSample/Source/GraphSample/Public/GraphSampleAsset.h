// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GraphSampleAsset.generated.h"

class UGraphSampleNode;

UCLASS()
class GRAPHSAMPLE_API UGraphSampleAsset : public UObject
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	friend class UGraphSampleGraph;
	UEdGraph* GetGraph() const { return GraphSampleGraph; }
#endif

	UPROPERTY()
	UGraphSampleNode* RootNode;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UEdGraph> GraphSampleGraph;
#endif
};