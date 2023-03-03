// Copyright (C) 2007-2022, MuChenhen

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "MemreportStructs.h"

typedef TSharedPtr<FTextureTotalStat> FTextureTotalStatPtr;

/**
 * 
 */
class MEMREPORTPARSER_API STextureCharts : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STextureCharts){}
    SLATE_ARGUMENT(FTextureTotalStatPtr, TextureTotalStat)
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

    TSharedRef<SWidget> MakeFormatChart();

    TSharedRef<SWidget> MakeGroupChart();

    
    FReply OnButtonFormatChartClicked() const;

    FReply OnButtonGroupChartClicked() const;

    TSharedPtr<SWidgetSwitcher> ChartSwitchWidget;
    
    TSharedPtr<SWidget> FormatChart;

    TSharedPtr<SWidget> GroupChart;

private:
    FTextureTotalStatPtr TextureTotalStat;
};
