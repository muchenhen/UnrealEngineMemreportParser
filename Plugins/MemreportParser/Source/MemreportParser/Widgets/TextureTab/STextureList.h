#pragma once

#include "MemreportStructs.h"
#include "TextureValues.h"
typedef TSharedPtr<FTextureMemory> FTextureMemoryPtr;

class STextureList final : public SListView<FTextureMemoryPtr>
{
public:
    SLATE_BEGIN_ARGS(STextureList){}

    SLATE_ARGUMENT(TArray<FTextureMemoryPtr>, TextureMemories);
    
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    TSharedRef<SHeaderRow> MakeHeaderRow();

    TSharedRef<ITableRow> OnGenerateRowSTextureList(FTextureMemoryPtr Item, const TSharedRef<STableViewBase>& OwnerTable) const;

    void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnId, const EColumnSortMode::Type InSortMode);

    void SortTextureList();

    EColumnSortMode::Type GetColumnSortMode(const FName ColumnId) const;

    EColumnSortPriority::Type GetColumnSortPriority(const FName ColumnId) const;

private:
    TArray<FTextureMemoryPtr> TextureMemories;
    
    // Sorting.
    EColumnSortMode::Type PrimarySortMode = EColumnSortMode::Descending;

    EColumnSortMode::Type SecondarySortMode = EColumnSortMode::None;

    FName PrimarySortedColumn;

    FName SecondarySortedColumn;
};
