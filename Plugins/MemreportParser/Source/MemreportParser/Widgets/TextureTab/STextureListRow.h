// Copyright (C) 2007-2022, MuChenhen

#pragma once

#include "CoreMinimal.h"
#include "MemreportStructs.h"

class STexturesTab;

typedef TSharedPtr<FTextureMemory> FTextureMemoryPtr;
/**
 * 
 */
class MEMREPORTPARSER_API STextureListRow : public SMultiColumnTableRow<TSharedPtr<FTextureMemory>>
{
    SLATE_BEGIN_ARGS(STextureListRow)
        :_TextureMemory(nullptr)
    {}
    SLATE_ARGUMENT(FTextureMemoryPtr, TextureMemory)
    SLATE_END_ARGS()
    
public:
    void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);

    FText GetCookedWidth() const;

    FText GetCookedSize() const;

    FText GetInMemWidth() const;

    FText GetInMemSize() const;

    FText GetFormat() const;

    FText GetLODGroup() const;

    FText GetName() const;

    FText GetStreaming() const;

    FText GetUsageCount() const;

    TSharedRef<SWidget> GetOperate();
    FReply OnOperateClicked() const;
    EVisibility GetOperateVisibility() const;
    
    virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

    FTextureMemoryPtr TextureMemory;

};