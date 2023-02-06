// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MEMREPORTPARSER_API SExportTab : public SDockTab
{
public:
    SLATE_BEGIN_ARGS(SExportTab)
        {
        }

    SLATE_END_ARGS()
    
    void Construct(const FArguments& InArgs);

    // 导出Obj List到csv文件
    FReply ExportObjectsCSV();
    
    // 导出SpawnActors List到csv
    FReply ExportActorsCSV();
    
    // 导出ConfigCache信息到csv
    FReply ExportConfigCacheCSV();

    // 导出Textures到csv
    FReply ExportTexturesCSV();
    
};
