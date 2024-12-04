// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GraphSampleEditorSettings.generated.h"

UCLASS(Config = Editor, defaultconfig, meta = (DisplayName = "GraphSample Editor"))
class GRAPHSAMPLEEDITOR_API UGraphSampleEditorSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:
	static UGraphSampleEditorSettings* Get() { return StaticClass()->GetDefaultObject<UGraphSampleEditorSettings>(); }

	UPROPERTY(EditAnywhere, config, Category = "GraphSample")
	FLinearColor NodeTypeStringColor;

	UPROPERTY(EditAnywhere, Config, Category = "GraphSample")
	bool bDefaultSaveToJson;
};
