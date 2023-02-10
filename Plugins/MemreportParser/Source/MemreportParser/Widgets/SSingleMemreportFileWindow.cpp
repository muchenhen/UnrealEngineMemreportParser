#include "SSingleMemreportFileWindow.h"

#include "Framework/Notifications/NotificationManager.h"

#define LOCTEXT_NAMESPACE "MemreportStartPageWindow"

SSingleMemreportFileWindow::~SSingleMemreportFileWindow()
{
}



void SSingleMemreportFileWindow::SetStats(FMemreportFile InMemreportFile)
{
    MemreportFile = InMemreportFile;
}

void SSingleMemreportFileWindow::Open()
{
    const TSharedRef<SWindow> MemreportFileWindow =
       SNew(SWindow)
       .Title(LOCTEXT("MemreportFileWindow", "MemreportFileWindow"))
       .Type(EWindowType::GameWindow)
       .AutoCenter(EAutoCenter::PrimaryWorkArea)
       .ScreenPosition(FVector2D(0, 0))
       .ClientSize(FVector2D(1280, 720))
       .SizingRule(ESizingRule::FixedSize)
       .SupportsMaximize(true)
       .SupportsMinimize(true)
       .HasCloseButton(true)
       .IsInitiallyMaximized(false)
       .IsInitiallyMinimized(false);

    const TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();

    TabManager->UnregisterAllTabSpawners();

    const FName StatTabID = TEXT("StatTab");
    const FName ExportTabID = TEXT("ExportTab");
    const FName TexturesTabID = TEXT("TexturesTab");

    TabManager->RegisterTabSpawner(StatTabID, FOnSpawnTab::CreateSP(this, &SSingleMemreportFileWindow::MakeStatTab));
    TabManager->RegisterTabSpawner(ExportTabID, FOnSpawnTab::CreateSP(this, &SSingleMemreportFileWindow::MakeExportTab));
    TabManager->RegisterTabSpawner(TexturesTabID, FOnSpawnTab::CreateSP(this, &SSingleMemreportFileWindow::MakeTextureTab));

    const TSharedPtr<FTabManager::FLayout> MemreportLayout = FTabManager::NewLayout("MemreportFileWindow_v1.0");
    
    MemreportLayout->AddArea
    (
        FTabManager::NewPrimaryArea()
        ->SetOrientation(Orient_Vertical)
        ->Split
        (
            FTabManager::NewStack()
            ->AddTab(StatTabID, ETabState::OpenedTab)
            ->AddTab(ExportTabID, ETabState::OpenedTab)
            ->AddTab(TexturesTabID, ETabState::OpenedTab)
            ->SetForegroundTab(StatTabID)
        )
    );
    
    FSlateApplication::Get().AddWindow(MemreportFileWindow, true);
    FGlobalTabmanager::Get()->SetRootWindow(MemreportFileWindow);
    FSlateNotificationManager::Get().SetRootWindow(MemreportFileWindow);

    constexpr bool bEmbedTitleAreaContent = false;
    constexpr EOutputCanBeNullptr OutputCanBeNullptr = EOutputCanBeNullptr::Never;
    const TSharedPtr<SWidget> Content = FGlobalTabmanager::Get()->RestoreFrom(MemreportLayout.ToSharedRef(), MemreportFileWindow, bEmbedTitleAreaContent, OutputCanBeNullptr);

    MemreportFileWindow->SetContent(Content.ToSharedRef());

    // Refresh();
}

void SSingleMemreportFileWindow::Refresh() const
{
    if (TextureTab.IsValid())
    {
        TextureTab.Get()->SetTextureMemoryLists(MemreportFile.TextureMemories);
        TextureTab.Get()->SetTextureTotalStat(MemreportFile.TextureTotalStat);
    }
}

TSharedRef<SDockTab> SSingleMemreportFileWindow::MakeStatTab(const FSpawnTabArgs& SpawnTabArgs)
{
    return SNew(SStatTab)
    .StatMemory(MemreportFile.StatMemory);
}

TSharedRef<SDockTab> SSingleMemreportFileWindow::MakeTextureTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TArray<FTextureMemoryPtr> TextureMemories;

    const FTextureTotalStatPtr TextureTotalStat = MakeShareable(new FTextureTotalStat(MemreportFile.TextureTotalStat));
    // 将MemreportFile.TextureMemories 转换为TextureMemories
    for (const FTextureMemory& TextureMemory : MemreportFile.TextureMemories)
    {
        TextureMemories.Add(MakeShareable(new FTextureMemory(TextureMemory)));
    }

    return SNew(STexturesTab)
    .TextureMemories(TextureMemories)
    .TextureTotalStat(TextureTotalStat);
}

TSharedRef<SDockTab> SSingleMemreportFileWindow::MakeExportTab(const FSpawnTabArgs& SpawnTabArgs)
{
    return SNew(SExportTab);
}

#undef LOCTEXT_NAMESPACE
