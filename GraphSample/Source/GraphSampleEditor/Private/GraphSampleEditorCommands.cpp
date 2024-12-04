// Copyright https://github.com/dzxmxd/GraphSampleEditor

#include "GraphSampleEditorCommands.h"

#include "GraphSampleEditorStyle.h"

#define LOCTEXT_NAMESPACE "GraphSampleGraphCommands"

FGraphSampleToolbarCommands::FGraphSampleToolbarCommands()
	: TCommands<FGraphSampleToolbarCommands>("GraphSampleToolbar", LOCTEXT("GraphSampleToolbar", "GraphSample Toolbar"), NAME_None, FGraphSampleEditorStyle::GetStyleSetName()) {}

void FGraphSampleToolbarCommands::RegisterCommands()
{
	UI_COMMAND(TestUICommand, "TestUITitle", "TestButtonOnToolBar.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
