// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"

class SGraphSampleGraphEditor;
class FGraphSampleAssetEditor;

class GRAPHSAMPLEEDITOR_API FGraphSampleEditorHelper
{
public:
	FGraphSampleEditorHelper() {}

	static TSharedPtr<FGraphSampleAssetEditor> GetGraphSampleAssetEditor(const UEdGraph* Graph);
	static TSharedPtr<SGraphSampleGraphEditor> GetGraphSampleGraphEditor(const UEdGraph* Graph);
};
