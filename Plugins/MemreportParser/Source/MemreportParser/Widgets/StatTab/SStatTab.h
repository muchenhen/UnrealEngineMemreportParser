// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MemreportParser/Public/MemreportStructs.h"
#include "Widgets/Docking/SDockableTab.h"

/**
 * 
 */
class MEMREPORTPARSER_API SStatTab : public SDockTab
{
public:
    SLATE_BEGIN_ARGS(SStatTab){}
    SLATE_ARGUMENT(FStatMemory, StatMemory)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    // 获取Stat的TextBlockStyle
    static FTextBlockStyle GetStatTextBlockStyle();

    // STAT相关的显示区域
    TSharedRef<SWidget> ConstructStatPanel();

    // 各类Memory的显示区域
    TSharedRef<SWidget> ConstructMemoryPanel();

    // FMallocBinned2 Mem report 的显示区域
    TSharedRef<SWidget> ConstructFMallocBinned2Panel();
    
};
