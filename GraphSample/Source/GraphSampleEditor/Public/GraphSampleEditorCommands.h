// Copyright https://github.com/dzxmxd/GraphSampleEditor

#pragma once

#include "CoreMinimal.h"

class GRAPHSAMPLEEDITOR_API FGraphSampleToolbarCommands : public TCommands<FGraphSampleToolbarCommands>
{
public:
	FGraphSampleToolbarCommands();

	TSharedPtr<FUICommandInfo> TestUICommand;

	virtual void RegisterCommands() override;
};
