// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "Asset/GraphSampleAssetEditorContext.h"

#include "Asset/GraphSampleAssetEditor.h"

UGraphSampleAsset* UGraphSampleAssetEditorContext::GetGraphSampleAsset() const
{
	return GraphSampleAssetEditor.IsValid() ? GraphSampleAssetEditor.Pin()->GetGraphSampleAsset() : nullptr;
}
