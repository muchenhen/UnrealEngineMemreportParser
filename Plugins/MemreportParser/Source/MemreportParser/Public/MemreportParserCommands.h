// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MemreportParserStyle.h"

class FMemreportParserCommands : public TCommands<FMemreportParserCommands>
{
public:

	FMemreportParserCommands()
		: TCommands<FMemreportParserCommands>(TEXT("MemreportParser"), NSLOCTEXT("Contexts", "MemreportParser", "MemreportParser Plugin"), NAME_None, FMemreportParserStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};