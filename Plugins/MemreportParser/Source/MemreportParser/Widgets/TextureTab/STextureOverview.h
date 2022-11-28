// Copyright (C) 2007-2022, MuChenhen

#pragma once

#include "CoreMinimal.h"
#include "MemreportStructs.h"
#include "Widgets/SCompoundWidget.h"

typedef TSharedPtr<FTextureTotalStat> FTextureTotalStatPtr;

/**
 * 
 */
class MEMREPORTPARSER_API STextureOverview : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STextureOverview)
        {
        }
    SLATE_ARGUMENT(FTextureTotalStatPtr, TextureTotalStat)
    SLATE_END_ARGS()

private:

    FTextureTotalStatPtr TextureTotalStat;
    
public:
    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

    FText GetTextureTotalNum() const;

    FText GetTextureTotalSizeInMemory() const;

    FText GetTextureTotalSizeOnDisk() const;


};
