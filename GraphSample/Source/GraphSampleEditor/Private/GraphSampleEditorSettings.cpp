// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "GraphSampleEditorSettings.h"

UGraphSampleEditorSettings::UGraphSampleEditorSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTypeStringColor = FLinearColor::White;
	bDefaultSaveToJson = true;
}
