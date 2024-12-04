// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GraphSampleAssetEditorContext.generated.h"

class FGraphSampleAssetEditor;
class UGraphSampleAsset;

UCLASS()
class GRAPHSAMPLEEDITOR_API UGraphSampleAssetEditorContext : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Tool Menus")
	UGraphSampleAsset* GetGraphSampleAsset() const;

	TWeakPtr<FGraphSampleAssetEditor> GraphSampleAssetEditor;
};
