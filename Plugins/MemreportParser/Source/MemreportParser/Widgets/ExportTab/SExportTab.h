// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MemreportStructs.h"

/**
 * 
 */
class MEMREPORTPARSER_API SExportTab : public SDockTab
{
public:
    SLATE_BEGIN_ARGS(SExportTab){}
    SLATE_ARGUMENT(FMemreportFile, MemreportFile)
    SLATE_END_ARGS()
    
    void Construct(const FArguments& InArgs);

    // 第一列VerticalBox Class列
    TSharedRef<SWidget> ConstructClassColumn(const FMemreportFile& InMemreportFile);

    // 第二列VerticalBox Count列 数量
    TSharedRef<SWidget> ConstructCountColumn(const FMemreportFile& InMemreportFile);

    // 第三列VerticalBox Num列 NumKB 内存大小
    TSharedRef<SWidget> ConstructNumColumn(const FMemreportFile& InMemreportFile);

    // 第四列VerticalBox Max列 MaxKB
    TSharedRef<SWidget> ConstructMaxColumn(const FMemreportFile& InMemreportFile);

    // 第五列VerticalBox ResExc列 ResExcKB
    TSharedRef<SWidget> ConstructResExcColumn(const FMemreportFile& InMemreportFile);

    // 第六列VerticalBox Export列
    TSharedRef<SWidget> ConstructExportColumn(const FMemreportFile& InMemreportFile);

public:
    // 导出Obj List到csv文件
    FReply ExportObjectsCSV();
    
    // 导出SpawnActors List到csv
    FReply ExportActorsCSV();
    
    // 导出ConfigCache信息到csv
    FReply ExportConfigCacheCSV();

    // 导出Textures到csv
    FReply ExportTexturesCSV();

private:
    FMemreportFile MemreportFile;
    
};
