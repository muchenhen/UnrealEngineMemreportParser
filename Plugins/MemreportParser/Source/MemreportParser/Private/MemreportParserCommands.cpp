// Copyright Epic Games, Inc. All Rights Reserved.

#include "MemreportParserCommands.h"

#define LOCTEXT_NAMESPACE "FMemreportParserModule"

void FMemreportParserCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MemreportParser", "Bring up MemreportParser window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
