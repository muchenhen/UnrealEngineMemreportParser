// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

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

    FReply ExportObjectsCSV();
    FReply ExportActorsCSV();
    FReply ExportConfigCacheCSV();
    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);
};
