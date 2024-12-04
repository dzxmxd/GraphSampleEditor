// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "GraphSampleGraph.generated.h"

class UGraphSampleGraphSchema;
class UGraphSampleAsset;

UCLASS()
class GRAPHSAMPLEEDITOR_API UGraphSampleGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	static UEdGraph* CreateGraph(UGraphSampleAsset* InGraphSampleAsset);
	static UEdGraph* CreateGraph(UGraphSampleAsset* InGraphSampleAsset, const TSubclassOf<UGraphSampleGraphSchema>& GraphSampleSchema);
	UGraphSampleAsset* GetGraphSampleAsset() const;

	void OnGraphSampleNodeChanged();
};