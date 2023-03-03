#pragma once
#include "SSingleMemreportFileWindow.h"
#include "MemreportParser/Public/MemreportStructs.h"


struct MEMREPORTPARSER_API FInsightsManagerTabs
{
	static const FName StartPageTabId;
	static const FName SessionInfoTabId;
	static const FName TimingProfilerTabId;
	static const FName LoadingProfilerTabId;
	static const FName NetworkingProfilerTabId;
	static const FName MemoryProfilerTabId;
	static const FName InsightsMessageLogTabId;
	static const FName AutomationWindowTabId;
};

class MEMREPORTPARSER_API SMemreportStartPageWindow : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SMemreportStartPageWindow){}
    SLATE_END_ARGS()
    
public:
    SMemreportStartPageWindow();
    
    virtual ~SMemreportStartPageWindow() override;
    
    static TSharedPtr<SMemreportStartPageWindow> GetMemreportStartPageWindow();
    
    void Construct(const FArguments& InArgs);
    
    void SetMemreportViewModels(TArray<FStatMemory> StatMemories);
    
    TSharedRef<ITableRow> MemreportList_OnGenerateRow(TSharedPtr<FStatMemory> InMemreportView, const TSharedRef<STableViewBase>& OwnerTable);
    
    void MemreportList_OnMouseButtonDoubleClick(TSharedPtr<FStatMemory> StatMemory);

    TSharedPtr<SWidget> MemreportList_GetContextMenu();

    void MemreportList_OnSelectionChanged(TSharedPtr<FStatMemory, ESPMode::NotThreadSafe> StatMemory, ESelectInfo::Type Arg);
    
    TSharedRef<SWidget> ConstructFilesPanel();
    
    static TSharedPtr<SMemreportStartPageWindow> MemreportStartPageWindow;

private:
    static TSharedPtr<SListView<TSharedPtr<FStatMemory>>> MemreportListView;

    TArray<TSharedPtr<FStatMemory>> MemreportViewModels;

    TMap<FString, TSharedPtr<SSingleMemreportFileWindow>> MemreportFileWindows;
};

class SMemreportListRow : public SMultiColumnTableRow<TSharedPtr<FStatMemory>>
{
    SLATE_BEGIN_ARGS(SMemreportListRow){}
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs, TSharedPtr<FStatMemory> InTrace, TSharedRef<SMemreportStartPageWindow> InParentWidget, const TSharedRef<STableViewBase>& InOwnerTableView);

    virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

    FText GetFileName() const;

    FText GetBootTime() const;

    FText GetPlatform() const;

    FText GetPhysicalMemoryUsed() const;

    FText GetVirtualMemoryUsed() const;

    FText GetSmallPoolAllocations() const;

    FText GetSmallPoolOSAllocated() const;

    FText GetLargePoolRequestedAllocations() const;

    FText GetLargePoolOSAllocated() const;

private:
    TWeakPtr<FStatMemory> WeakTrace;

    TWeakPtr<SMemreportStartPageWindow> WeakParentWidget;
};
