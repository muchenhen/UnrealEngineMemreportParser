#include "MemreportStartPageWindow.h"

#include "MemreportParserManager.h"
#include "SSingleMemreportFileWindow.h"
#include "Framework/Notifications/NotificationManager.h"

#define LOCTEXT_NAMESPACE "MemreportStartPageWindow"

TSharedPtr<SMemreportStartPageWindow> SMemreportStartPageWindow::MemreportStartPageWindow;
TSharedPtr<SListView<TSharedPtr<FStatMemory>>> SMemreportStartPageWindow::MemreportListView;

SMemreportStartPageWindow::SMemreportStartPageWindow()
{
}

SMemreportStartPageWindow::~SMemreportStartPageWindow()
{
}

void SMemreportStartPageWindow::Construct(const FArguments& InArgs)
{
}

TSharedPtr<SMemreportStartPageWindow> SMemreportStartPageWindow::GetMemreportStartPageWindow()
{
    if (!MemreportStartPageWindow.IsValid())
    {
        MemreportStartPageWindow = SNew(SMemreportStartPageWindow);
    }
    return MemreportStartPageWindow;
}

void SMemreportStartPageWindow::SetMemreportViewModels(TArray<FStatMemory> StatMemories)
{
    for (auto& StatMemory : StatMemories)
    {
        const TSharedPtr<FStatMemory> Model = MakeShareable(new FStatMemory);
        Model.Get()->FileName = StatMemory.FileName;
        Model.Get()->SinceBootTime = StatMemory.SinceBootTime;
        Model.Get()->Platform = StatMemory.Platform;
        Model.Get()->ProcessPhysicalMemoryUsed = StatMemory.ProcessPhysicalMemoryUsed;
        Model.Get()->ProcessVirtualMemoryUsed = StatMemory.ProcessVirtualMemoryUsed;
        Model.Get()->SmallPoolAllocations = StatMemory.SmallPoolAllocations;
        Model.Get()->SmallPoolOSAllocated = StatMemory.SmallPoolOSAllocated;
        Model.Get()->LargePoolRequestedAllocations = StatMemory.LargePoolRequestedAllocations;
        Model.Get()->LargePoolOSAllocated = StatMemory.LargePoolOSAllocated;
        MemreportViewModels.Add(Model);
    }
    if (MemreportListView.IsValid())
    {
        MemreportListView->RequestListRefresh();
    }
}

TSharedRef<ITableRow> SMemreportStartPageWindow::MemreportList_OnGenerateRow(TSharedPtr<FStatMemory> InMemreportView, const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(SMemreportListRow, InMemreportView, SharedThis(this), OwnerTable);
}

void SMemreportStartPageWindow::MemreportList_OnMouseButtonDoubleClick(TSharedPtr<FStatMemory> StatMemory)
{
    UE_LOG(LogTemp, Warning, TEXT("Double Clicked"));
    const FString FileName = StatMemory.Get()->FileName;
    if (!FileName.IsEmpty())
    {
        if (MemreportFileWindows.Find(FileName))
        {
           MemreportFileWindows[FileName]->Open();
        }
        else
        {
            const TSharedPtr<SSingleMemreportFileWindow> SingleMemreportFileWindow = MakeShared<SSingleMemreportFileWindow>();
            MemreportFileWindows.Add(FileName) = SingleMemreportFileWindow;
            const FMemreportFile FileData = UMemreportParserManager::GetFileData(FileName);
            if (FileData.IsValid())
            {
                SingleMemreportFileWindow->SetStats(FileData);
                SingleMemreportFileWindow->Open();
            }
            else
            {
                UE_LOG(LogMemreportParser, Error, TEXT("%s is Empty!"), *FileName);
            }
        }
    }
}

TSharedRef<SWidget> SMemreportStartPageWindow::ConstructFilesPanel()
{
    TSharedRef<SWidget> Widget =
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
          .AutoHeight()
          .HAlign(HAlign_Left)
          .Padding(0.0f, 2.0f)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("SessionsPanelTitle", "Memreport Files"))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
            .ColorAndOpacity(FLinearColor::Gray)
        ]

        + SVerticalBox::Slot()
          .HAlign(HAlign_Fill)
          .VAlign(VAlign_Fill)
          .Padding(0.0f, 1.0f, 0.0f, 2.0f)
        [
            SAssignNew(MemreportListView, SListView<TSharedPtr<FStatMemory>>)
            .IsFocusable(true)
            .ItemHeight(20.0f)
            .SelectionMode(ESelectionMode::Single)
            // .OnSelectionChanged(this, &SStartPageWindow::TraceList_OnSelectionChanged)
            .OnMouseButtonDoubleClick(this, &SMemreportStartPageWindow::MemreportList_OnMouseButtonDoubleClick)
            .ListItemsSource(&MemreportViewModels)
            .OnGenerateRow(this, &SMemreportStartPageWindow::MemreportList_OnGenerateRow)
            .ConsumeMouseWheel(EConsumeMouseWheel::Always)
            //.OnContextMenuOpening(FOnContextMenuOpening::CreateSP(this, &SStartPageWindow::TraceList_GetContextMenu))
            .HeaderRow
            (
                SNew(SHeaderRow)

                + SHeaderRow::Column(FName(TEXT("FileName")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("FileNameColumn", "File Name"))

                + SHeaderRow::Column(FName(TEXT("Platform")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("PlatformColumn", "Platform"))

                + SHeaderRow::Column(FName(TEXT("SinceBootTime")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("BootTimeColumn", "Boot Time"))

                + SHeaderRow::Column(FName(TEXT("ProcessPhysicalMemoryUsed")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("PhysicalMemoryUsedColumn", "Physical Memory Used"))

                + SHeaderRow::Column(FName(TEXT("ProcessVirtualMemoryUsed")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("VirtualMemoryUsedColumn", "Virtual Memory Used"))

                + SHeaderRow::Column(FName(TEXT("SmallPoolAllocations")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("SmallPoolAllocationsColumn", "Small Pool Allocations"))

                + SHeaderRow::Column(FName(TEXT("SmallPoolOSAllocated")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("SmallPoolOSAllocatedColumn", "Small Pool OS Allocated"))

                + SHeaderRow::Column(FName(TEXT("LargePoolRequestedAllocations")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("LargePoolRequestedAllocationsColumn", "Large Pool Requested Allocations"))

                + SHeaderRow::Column(FName(TEXT("LargePoolOSAllocated")))
                  .FillWidth(1.0f)
                  .DefaultLabel(LOCTEXT("LargePoolOSAllocatedColumn", "Large Pool OS Allocated"))
                )
    ];

    return Widget;
}

/*--------------------------------------------------------------------------------
 * SMemreportListRow
 *-------------------------------------------------------------------------------
 */

FText SMemreportListRow::GetFileName() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->FileName);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetBootTime() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->SinceBootTime);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetPlatform() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->Platform);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetPhysicalMemoryUsed() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->ProcessPhysicalMemoryUsed);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetVirtualMemoryUsed() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->ProcessVirtualMemoryUsed);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetSmallPoolAllocations() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->SmallPoolAllocations);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetSmallPoolOSAllocated() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->SmallPoolOSAllocated);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetLargePoolRequestedAllocations() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->LargePoolRequestedAllocations);
    }
    return FText::GetEmpty();
}

FText SMemreportListRow::GetLargePoolOSAllocated() const
{
    const TSharedPtr<FStatMemory> Pin = WeakTrace.Pin();
    if (Pin.IsValid())
    {
        return FText::FromString(Pin->LargePoolOSAllocated);
    }
    return FText::GetEmpty();
}

void SMemreportListRow::Construct(const FArguments& InArgs, TSharedPtr<FStatMemory> InTrace, TSharedRef<SMemreportStartPageWindow> InParentWidget, const TSharedRef<STableViewBase>& InOwnerTableView)
{
    WeakTrace = MoveTemp(InTrace);
    WeakParentWidget = InParentWidget;

    SMultiColumnTableRow<TSharedPtr<FStatMemory>>::Construct(FSuperRowType::FArguments(), InOwnerTableView);
}

TSharedRef<SWidget> SMemreportListRow::GenerateWidgetForColumn(const FName& ColumnName)
{
    if (ColumnName == FName(TEXT("FileName")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetFileName)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("SinceBootTime")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetBootTime)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("Platform")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetPlatform)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("ProcessPhysicalMemoryUsed")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetPhysicalMemoryUsed)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("ProcessVirtualMemoryUsed")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetVirtualMemoryUsed)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("SmallPoolAllocations")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetSmallPoolAllocations)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("SmallPoolOSAllocated")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetSmallPoolOSAllocated)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("LargePoolRequestedAllocations")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetLargePoolRequestedAllocations)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == FName(TEXT("LargePoolOSAllocated")))
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &SMemreportListRow::GetLargePoolOSAllocated)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    return SNew(STextBlock).Text(LOCTEXT("UnknownColumn", "Unknown Column"));
}

#undef LOCTEXT_NAMESPACE
