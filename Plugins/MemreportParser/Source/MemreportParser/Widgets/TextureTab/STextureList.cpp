#include "STextureList.h"

#include "STextureListRow.h"


#define LOCTEXT_NAMESPACE "STextureList"


void STextureList::Construct(const FArguments& InArgs)
{
    TextureMemories = InArgs._TextureMemories;
    SListView::Construct
    (
        SListView::FArguments()
        .ListItemsSource(&TextureMemories)
        .SelectionMode(ESelectionMode::Single)
        .OnGenerateRow(this, &STextureList::OnGenerateRowSTextureList)
        .HeaderRow(MakeHeaderRow())
    );
}

TSharedRef<SHeaderRow> STextureList::MakeHeaderRow()
{
    return SNew(SHeaderRow)
           + SHeaderRow::Column(ColNameInMemWidth)
             .FillWidth(1.0f)
             .DefaultLabel(LOCTEXT("InMemWidthColumn", "Mem Width"))

           + SHeaderRow::Column(ColNameInMemSize)
             .FillWidth(1.0f)
             .DefaultLabel(LOCTEXT("InMemSizeColumn", "Mem Size"))
             .SortPriority(this, &STextureList::GetColumnSortPriority, ColNameInMemSize)
             .SortMode(this, &STextureList::GetColumnSortMode, ColNameInMemSize)
             .OnSort(this, &STextureList::OnColumnSortModeChanged)

           + SHeaderRow::Column(ColNameCookedWidth)
             .FillWidth(1.0f)
             .DefaultLabel(LOCTEXT("CookedWidthColumn", "Disk Width"))

           + SHeaderRow::Column(ColNameCookedSize)
             .FillWidth(1.0f)
             .DefaultLabel(LOCTEXT("CookedSizeColumn", "Disk Size"))

           + SHeaderRow::Column(ColNameFormat)
             .FillWidth(1.5f)
             .DefaultLabel(LOCTEXT("FormatColumn", "Format"))

           + SHeaderRow::Column(ColNameLODGroup)
             .FillWidth(2.0f)
             .DefaultLabel(LOCTEXT("LODGroupColumn", "LODGroup"))

           + SHeaderRow::Column(ColNameName)
             .FillWidth(6.0f)
             .DefaultLabel(LOCTEXT("NameColumn", "Name"))

           + SHeaderRow::Column(ColNameStreaming)
             .FillWidth(0.8f)
             .DefaultLabel(LOCTEXT("StreamingColumn", "Streaming"))

           + SHeaderRow::Column(ColNameUsageCount)
             .FillWidth(0.8f)
             .DefaultLabel(LOCTEXT("UsageCountColumn", "UsageCount"))

            + SHeaderRow::Column(ColNameOperate)
             .FillWidth(0.5f)
             .DefaultLabel(LOCTEXT("OperateColumn", "Operate"));
}

TSharedRef<ITableRow> STextureList::OnGenerateRowSTextureList(FTextureMemoryPtr Item, const TSharedRef<STableViewBase>& OwnerTable) const
{
    TSharedRef<STextureListRow> ReturnRow = SNew(STextureListRow, OwnerTable)
        .TextureMemory(Item);
    return ReturnRow;
}

void STextureList::OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode)
{
    if (SortPriority == EColumnSortPriority::Primary)
    {
        PrimarySortedColumn = ColumnId;
        PrimarySortMode = InSortMode;

        if (ColumnId == SecondarySortedColumn) // Cannot be primary and secondary at the same time.
        {
            SecondarySortedColumn = FName();
            SecondarySortMode = EColumnSortMode::None;
        }
    }
    else if (SortPriority == EColumnSortPriority::Secondary)
    {
        SecondarySortedColumn = ColumnId;
        SecondarySortMode = InSortMode;
    }
    SortTextureList();
    RequestListRefresh();
}

void STextureList::SortTextureList()
{
    auto Compare = [](const TSharedPtr<FTextureMemory>& A, const TSharedPtr<FTextureMemory>& B, const FName& ColName, EColumnSortMode::Type SortMode) {
        if (ColName == ColNameInMemSize)
        {
            return SortMode == EColumnSortMode::Ascending ? A->InMemSize < B->InMemSize : A->InMemSize > B->InMemSize;
        }
        else
        {
            return SortMode == EColumnSortMode::Ascending ? A->InMemSize < B->InMemSize : A->InMemSize > B->InMemSize;
        }
    };

    TextureMemories.Sort([&](const FTextureMemoryPtr& A, const FTextureMemoryPtr& B) {
        if (Compare(A, B, PrimarySortedColumn, PrimarySortMode))
        {
            return true;
        }
        else if (Compare(B, A, PrimarySortedColumn, PrimarySortMode))
        {
            return false;
        }
        else
        {
            return SecondarySortedColumn.IsNone() ? false : Compare(A, B, SecondarySortedColumn, SecondarySortMode);
        }
    });
}

EColumnSortMode::Type STextureList::GetColumnSortMode(const FName ColumnId) const
{
    if (ColumnId == PrimarySortedColumn)
    {
        return PrimarySortMode;
    }
    else if (ColumnId == SecondarySortedColumn)
    {
        return SecondarySortMode;
    }

    return EColumnSortMode::None;
}

EColumnSortPriority::Type STextureList::GetColumnSortPriority(const FName ColumnId) const
{
    if (ColumnId == PrimarySortedColumn)
    {
        return EColumnSortPriority::Primary;
    }
    else if (ColumnId == SecondarySortedColumn)
    {
        return EColumnSortPriority::Secondary;
    }
    return EColumnSortPriority::Max;
}

#undef LOCTEXT_NAMESPACE
