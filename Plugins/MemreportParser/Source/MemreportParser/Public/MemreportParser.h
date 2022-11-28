// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MemreportParser/Widgets/MemreportStartPageWindow.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMemreportParserModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
    
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs) const;

	FReply OnClickChooseFileFolder() const;

    FReply OnClickSaveObjectsCSVFile();

    FReply SaveSpawnedActorsCSVFile();

    FReply OnClickSaveConfigCacheMemory();

private:
	TSharedPtr<class FUICommandList> PluginCommands;

    static SMemreportStartPageWindow* MemreportStartPageWindow;
};