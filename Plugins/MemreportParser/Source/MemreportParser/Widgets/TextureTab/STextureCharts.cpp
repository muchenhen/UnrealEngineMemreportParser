// Copyright (C) 2007-2022, MuChenhen


#include "STextureCharts.h"

#include "SlateOptMacros.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "STextureCharts"

void STextureCharts::Construct(const FArguments& InArgs)
{
    ChartSwitchWidget = SNew(SWidgetSwitcher);

    FormatChart = MakeFormatChart();

    GroupChart = MakeGroupChart();

    ChartSwitchWidget->AddSlot()
    [
        FormatChart.ToSharedRef()
    ];
    
    ChartSwitchWidget->AddSlot()
    [
        GroupChart.ToSharedRef()
    ];
    
    ChildSlot
    [

        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .FillHeight(0.5f)
            [
                SNew(SButton)
                .OnClicked(this, &STextureCharts::OnButtonFormatChartClicked)
                .Text(LOCTEXT("STextureChart_ButtonFormatChart", "Format"))
            ]
            + SVerticalBox::Slot()
            .FillHeight(0.5f)
            [
                SNew(SButton)
                .OnClicked(this,  &STextureCharts::OnButtonGroupChartClicked)
                .Text(LOCTEXT("STextureChart_ButtonGroupChart", "Group"))
            ]
        ]

        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            ChartSwitchWidget.ToSharedRef()
        ]
    ];
}

TSharedRef<SWidget> STextureCharts::MakeFormatChart()
{
    auto AFormatChart =
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .VAlign(VAlign_Center)
        [
            SNew(STextBlock)
             .Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
             .Text(LOCTEXT("UnlockedNotice1", "Unlocked1"))
        ]

        + SVerticalBox::Slot()
        .VAlign(VAlign_Center)
        [
            SNew(STextBlock)
            .Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
            .Text(LOCTEXT("UnlockedNotice2", "Unlocked2"))
        ];
    return AFormatChart;
}

TSharedRef<SWidget> STextureCharts::MakeGroupChart()
{
    auto AGroupChart =
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .VAlign(VAlign_Center)
        [
            SNew(SImage)
            .Image(FEditorStyle::GetBrush("GenericUnlock"))
        ]

        + SVerticalBox::Slot()
        .VAlign(VAlign_Center)
        [
            SNew(SImage)
            .Image(FEditorStyle::GetBrush("GenericUnlock"))
        ];
    return AGroupChart;
}

FReply STextureCharts::OnButtonFormatChartClicked() const
{
    ChartSwitchWidget->SetActiveWidget(FormatChart.ToSharedRef());
    return FReply::Handled();
}

FReply STextureCharts::OnButtonGroupChartClicked() const
{
    ChartSwitchWidget->SetActiveWidget(GroupChart.ToSharedRef());
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
