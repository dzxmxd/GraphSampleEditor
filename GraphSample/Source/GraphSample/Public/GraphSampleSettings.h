// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GraphSampleSettings.generated.h"

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "GraphSample"))
class GRAPHSAMPLE_API UGraphSampleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static UGraphSampleSettings* Get() { return StaticClass()->GetDefaultObject<UGraphSampleSettings>(); }

	UPROPERTY(config, EditAnywhere, Category = "GraphSample", meta = (ToolTip = "TestSettingsParams"))
	FString TestSettingsParam;
};