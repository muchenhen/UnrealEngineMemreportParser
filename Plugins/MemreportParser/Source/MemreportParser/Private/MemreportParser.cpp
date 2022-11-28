// Copyright Epic Games, Inc. All Rights Reserved.

#include "MemreportParser.h"

#include "DesktopPlatformModule.h"
#include "EditorDirectories.h"
#include "IDesktopPlatform.h"
#include "MemreportParserStyle.h"
#include "MemreportParserCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "MemreportParserManager.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

static const FName MemreportParserTabName("MemreportParser");

SMemreportStartPageWindow* FMemreportParserModule::MemreportStartPageWindow = nullptr;

#define LOCTEXT_NAMESPACE "FMemreportParserModule"

void FMemreportParserModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FMemreportParserStyle::Initialize();
	FMemreportParserStyle::ReloadTextures();

	FMemreportParserCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMemreportParserCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMemreportParserModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMemreportParserModule::RegisterMenus));

    MemreportStartPageWindow = SMemreportStartPageWindow::GetMemreportStartPageWindow().Get();
}

void FMemreportParserModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    UMemreportParserManager::OnCloseParser();

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMemreportParserStyle::Shutdown();

	FMemreportParserCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MemreportParserTabName);
}

TSharedRef<SDockTab> FMemreportParserModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs) const
{

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMemreportParserModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("MemreportParser.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab);
}

FReply FMemreportParserModule::OnClickChooseFileFolder() const
{
    FString OutputDirectory;
    const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
    const FString Title = LOCTEXT("DatasmithDirProducerFolderTitle", "Choose a folder").ToString();
    const FString DefaultLocation(FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT));

    const FString OpenPath = FPaths::ProjectContentDir();
    const FString DefaultDirectory = FPaths::ConvertRelativePathToFull(OpenPath);
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
        ParentWindowHandle,
        Title,
        DefaultDirectory,
        OutputDirectory);
    UMemreportParserManager::SetFileFolder(OutputDirectory);
    UMemreportParserManager::LoadFiles();
    MemreportStartPageWindow->SetMemreportViewModels(UMemreportParserManager::GetStats());
    
    return FReply::Handled();
}

FReply FMemreportParserModule::OnClickSaveObjectsCSVFile()
{
    UMemreportParserManager::SaveObjListToCSV();
    return FReply::Handled();
}

FReply FMemreportParserModule::SaveSpawnedActorsCSVFile()
{
    UMemreportParserManager::SaveSpawnedActorsToCSV();
    return FReply::Handled();
}

FReply FMemreportParserModule::OnClickSaveConfigCacheMemory()
{
    UMemreportParserManager::SaveConfigCacheMemoryToCSV();
    return FReply::Handled();
}

void FMemreportParserModule::PluginButtonClicked()
{
	// FGlobalTabmanager::Get()->TryInvokeTab(MemreportParserTabName);
    constexpr float WindowWidth = 1280.0f;
    constexpr float WindowHeight = 720.0f;
    // const float DPIScaleFactor = FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(10.0f, 10.0f);

    TSharedRef<FTabManager::FLayout> DefaultLayout = FTabManager::NewLayout("MemreportParser_v1.0");
    const TSharedRef<SWindow> RootWindow =
        SNew(SWindow)
        .AutoCenter(EAutoCenter::PreferredWorkArea)
        .Title(NSLOCTEXT("TraceInsightsModule", "UnrealInsightsBrowserAppName", "Unreal Insights Session Browser"))
        .IsInitiallyMaximized(false)
        .ClientSize(FVector2D(WindowWidth, WindowHeight))
        .SupportsMaximize(true)
        .SupportsMinimize(true)
        [
            SNew(SBox)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .HAlign(HAlign_Left)
                .VAlign(VAlign_Center)
                .FillHeight(0.1f)
                .Padding(0.0f, 0.0f, 0.0f, 2.0f)
                [
                    SNew(SButton)
                     .Text(LOCTEXT("OnClickChooseFileFolder", "Choose File Folder"))
                     .HAlign(HAlign_Fill)
                     .VAlign(VAlign_Fill)
                     // .DesiredSizeScale(2)
                     .OnClicked_Raw(this, &FMemreportParserModule::OnClickChooseFileFolder)
                ]

                + SVerticalBox::Slot()
                .HAlign(HAlign_Fill)
                .VAlign(VAlign_Fill)
                .FillHeight(0.9f)
                [
                    SNew(SVerticalBox)
                    +SVerticalBox::Slot()
                   .HAlign(HAlign_Fill)
                   .VAlign(VAlign_Fill)
                   .Padding(3.0f, 3.0f)
                   [
                       SNew(SBox)
                        [
                            SNew(SBorder)
                            .Padding(8.0f)
                            [
                                MemreportStartPageWindow->ConstructFilesPanel()
                            ]
                        ]
                   ]
                ]
            ]
        ];
        
    FSlateApplication::Get().AddWindow(RootWindow, true);

    FGlobalTabmanager::Get()->SetRootWindow(RootWindow);
    FSlateNotificationManager::Get().SetRootWindow(RootWindow);

}

void FMemreportParserModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMemreportParserCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMemreportParserCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMemreportParserModule, MemreportParser)