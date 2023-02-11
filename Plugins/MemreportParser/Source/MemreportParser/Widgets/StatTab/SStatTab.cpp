// Fill out your copyright notice in the Description page of Project Settings.


#include "SStatTab.h"

#include "MemreportTools.h"
#include "SlateOptMacros.h"
#include "Engine/AssetManager.h"
#include "Framework/Styling/TextBlockWidgetStyle.h"
#include "MemreportParser/Widgets/Chart/SBarGraphWidget.h"
#include "Widgets/SCanvas.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "SStatTab"

void SStatTab::Construct(const FArguments& InArgs)
{
    StatMemory = InArgs._StatMemory;
    
    SDockTab::Construct(SDockTab::FArguments()
        .TabRole(ETabRole::NomadTab)
        
        [
		    SNew(SOverlay)
		    .Visibility(EVisibility::SelfHitTestInvisible)

		    /*
		     * 左上角 两行大字 系统启动时间和平台
		     */
		    + SOverlay::Slot()
		    .HAlign(HAlign_Left)
		    .VAlign(VAlign_Top)
		    .Padding(10,10,0,0)
            [
                SNew(SBox)
                .WidthOverride(600)
                .HeightOverride(100)
                .Clipping(EWidgetClipping::ClipToBounds)
                [
                    ConstructStatPanel(StatMemory)
                ]
            ]

            /*
             * 接着左上角的
             * Process Physical Memory: xxx MB used, xxx MB peak
             * Process Virtual Memory: xxx MB used, xxx MB peak
             */
            + SOverlay::Slot()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Top)
            .Padding(10,120,0,0)
            [
                SNew(SBorder)
                .BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
                .BorderBackgroundColor(FSlateColor(FLinearColor::Blue))
                .Content()
                [
                    SNew(SBox)
                    .WidthOverride(600)
                    .HeightOverride(150)
                    .Padding(FMargin(2.0f))
                    .Clipping(EWidgetClipping::ClipToBounds)
                    [
                        ConstructProcessMemoryPanel(StatMemory)
                    ]
                ]
            ]

            /*
             * 左边再往下
             * Physical Memory: 4938.33 MB used,  2335.36 MB free, 7273.69 MB total
             * Virtual Memory: 2436.47 MB used,  1659.53 MB free, 4096.00 MB total
             */
            + SOverlay::Slot()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Top)
            .Padding(10,280,0,0)
            [
                SNew(SBorder)
                .BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
                .BorderBackgroundColor(FSlateColor(FLinearColor(243, 156, 18)))
                .Content()
                [
                    SNew(SBox)
                    .WidthOverride(600)
                    .HeightOverride(150)
                    .Padding(FMargin(2.0f))
                    .Clipping(EWidgetClipping::ClipToBounds)
                    [
                        ConstructMemoryPanel(StatMemory)
                    ]
                ]
            ]

            /*
             * FMallocBinned2 Mem report的一堆东西 右上角部分
             */
            + SOverlay::Slot()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Top)
            .Padding(650,10,5,0)
            [
                SNew(SBorder)
                .BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
                .BorderBackgroundColor(MemreportTools::StringToSlateColor("ffaf65"))
                .Content()
                [
                    SNew(SBox)
                    .WidthOverride(600)
                    .HeightOverride(420)
                    .Padding(FMargin(2.0f))
                    .Clipping(EWidgetClipping::ClipToBounds)
                    [
                        ConstructFMallocBinned2Panel(StatMemory)
                    ]
                ]
            ]

            /*
             * 最下面的部分
             */
            + SOverlay::Slot()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Top)
            .Padding(650,440,5,0)
            [
                SNew(SBorder)
                .BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
                .BorderBackgroundColor(MemreportTools::StringToSlateColor("111111"))
                .Content()
                [
                    SNew(SBox)
                    .WidthOverride(600)
                    .HeightOverride(420)
                    .Padding(FMargin(2.0f))
                    .Clipping(EWidgetClipping::ClipToBounds)
                    [
                        SNew(SBarGraphWidget)
                    ]
                ]
            ]
	    ]);
}

TSharedRef<SWidget> SStatTab::ConstructStatPanel(const FStatMemory& InFStatMemory)
{
    const FTextBlockStyle TextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 24))
        .SetFontSize(24)
        .SetColorAndOpacity(FLinearColor::White);

    // 一个垂直框
    return SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)
    
            // SinceBootTime
            + SVerticalBox::Slot()
            .FillHeight(1)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SHorizontalBox)
                .Clipping(EWidgetClipping::Inherit)
                + SHorizontalBox::Slot()
                .FillWidth(1)
                .HAlign(HAlign_Left)
                .VAlign(VAlign_Fill)
                .Padding(FMargin(0,0,20,0))
                [
                    SNew(STextBlock)
                    .Clipping(EWidgetClipping::Inherit)
                    .Text(FText::FromString(TEXT("Time Since Boot(Seconds):")))
                    .TextStyle(&TextBlockStyle)
                ]
                + SHorizontalBox::Slot()
                .FillWidth(1)
                .HAlign(HAlign_Right)
                .VAlign(VAlign_Fill)
                [
                    SNew(STextBlock)
                    .Clipping(EWidgetClipping::Inherit)
                    .Text(FText::FromString(InFStatMemory.SinceBootTime))
                    .TextStyle(&TextBlockStyle)
                ]
            ]

            // Platform
            + SVerticalBox::Slot()
            .FillHeight(1)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SHorizontalBox)
                .Clipping(EWidgetClipping::Inherit)
                
                + SHorizontalBox::Slot()
                .FillWidth(1)
                .HAlign(HAlign_Left)
                .VAlign(VAlign_Fill)
                .Padding(FMargin(0,0,20,0))
                [
                    SNew(STextBlock)
                    .Clipping(EWidgetClipping::Inherit)
                    .Text(FText::FromString(TEXT("Platform:")))
                    .TextStyle(&TextBlockStyle)
                ]
                + SHorizontalBox::Slot()
                .FillWidth(1)
                .HAlign(HAlign_Right)
                .VAlign(VAlign_Fill)
                [
                    SNew(STextBlock)
                    .Clipping(EWidgetClipping::Inherit)
                    .Text(FText::FromString(InFStatMemory.Platform))
                    .TextStyle(&TextBlockStyle)
                ]
            ];
}

TSharedRef<SWidget> SStatTab::ConstructProcessMemoryPanel(const FStatMemory& InFStatMemory)
{
    const FTextBlockStyle HeaderTextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 18))
        .SetFontSize(18)
        .SetColorAndOpacity(FLinearColor::White);

    const FTextBlockStyle TextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 14))
        .SetFontSize(14)
        .SetColorAndOpacity(FLinearColor::White);

    auto MemoryPanel =
        SNew(SHorizontalBox)
        .Clipping(EWidgetClipping::Inherit)
        
        + SHorizontalBox::Slot()
        .FillWidth(0.6)
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(0,0,20,0))
        [
            // 垂直框
            SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)

            // 内存类型
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Memory_LOC", "Memory"))
                .TextStyle(&HeaderTextBlockStyle)
            ]

            // Process Physical Memory
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("ProcessPhysicalMemory_LOC", "Process Physical Memory:"))
                .TextStyle(&TextBlockStyle)
            ]

            // Process Virtual Memory
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("ProcessVirtualMemory_LOC", "Process Virtual Memory:"))
                .TextStyle(&TextBlockStyle)
            ]
        ]
    
        + SHorizontalBox::Slot()
        .FillWidth(0.2)
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(0,0,20,0))
        [
            // 垂直框
            SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)

            // 已使用的内存大小
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Used_LOC", "Used(MB)"))
                .TextStyle(&HeaderTextBlockStyle)
            ]

            // Process Physical Memory Used Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.ProcessPhysicalMemoryUsed))
                .TextStyle(&TextBlockStyle)
            ]

            // Process Virtual Memory Used Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.ProcessVirtualMemoryUsed))
                .TextStyle(&TextBlockStyle)
            ]
        ]

        + SHorizontalBox::Slot()
        .FillWidth(0.2)
        .HAlign(HAlign_Right)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(0,0,20,0))
        [
            // 垂直框
            SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)

            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Peak_LOC", "Peak(MB)"))
                .TextStyle(&HeaderTextBlockStyle)
            ]

            // Process Physical Memory Peak Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.ProcessPhysicalMemoryPeak))
                .TextStyle(&TextBlockStyle)
            ]

            // Process Virtual Memory Peak Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.ProcessVirtualMemoryPeak))
                .TextStyle(&TextBlockStyle)
            ]
        ]
    ;
    
    return MemoryPanel;
}

TSharedRef<SWidget> SStatTab::ConstructMemoryPanel(const FStatMemory& InFStatMemory)
{
    const FTextBlockStyle HeaderTextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 18))
        .SetFontSize(18)
        .SetColorAndOpacity(FLinearColor::White);

    const FTextBlockStyle TextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 14))
        .SetFontSize(14)
        .SetColorAndOpacity(FLinearColor::White);

    auto MemoryPanel =
        SNew(SHorizontalBox)
        .Clipping(EWidgetClipping::Inherit)
    
        + SHorizontalBox::Slot()
        .FillWidth(0.4)
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(0,0,20,0))
        [
            // 垂直框
            SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)

            // 内存类型
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Memory_LOC", "Memory"))
                .TextStyle(&HeaderTextBlockStyle)
            ]

            // Physical Memory
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("PhysicalMemory_LOC", "Physical Memory:"))
                .TextStyle(&TextBlockStyle)
            ]

            // Virtual Memory
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("VirtualMemory_LOC", "Virtual Memory:"))
                .TextStyle(&TextBlockStyle)
            ]
        ]

        + SHorizontalBox::Slot()
        .FillWidth(0.2)
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(10,0,20,0))
        [
            // 垂直框
            SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)

            // 已使用的内存大小
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Used_LOC", "Used(MB)"))
                .TextStyle(&HeaderTextBlockStyle)
            ]

            // Physical Memory Used Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.PhysicalMemoryUsed))
                .TextStyle(&TextBlockStyle)
            ]

            // Virtual Memory Used Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.VirtualMemoryUsed))
                .TextStyle(&TextBlockStyle)
            ]
        ]

        + SHorizontalBox::Slot()
        .FillWidth(0.2)
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        .Padding(FMargin(10,0,20,0))
        [
            SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)

            // free
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Free_LOC", "Free(MB)"))
                .TextStyle(&HeaderTextBlockStyle)
            ]

            // Physical Memory Free Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.PhysicalMemoryFree))
                .TextStyle(&TextBlockStyle)
            ]

            // Virtual Memory Free Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.VirtualMemoryFree))
                .TextStyle(&TextBlockStyle)
            ]
        ]

        + SHorizontalBox::Slot()
        .FillWidth(0.2)
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        .Padding( FMargin(10,0,20,0))
        [
            SNew(SVerticalBox)
            .Clipping(EWidgetClipping::Inherit)

            // total
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Total_LOC", "Total(MB)"))
                .TextStyle(&HeaderTextBlockStyle)
            ]

            // Physical Memory Total Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.PhysicalMemoryTotal))
                .TextStyle(&TextBlockStyle)
            ]

            // Virtual Memory Total Size
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            .Padding(FMargin(0,5))
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.VirtualMemoryTotal))
                .TextStyle(&TextBlockStyle)
            ]
        ];

    return MemoryPanel;
}

TSharedRef<SWidget> SStatTab::ConstructFMallocBinned2Panel(const FStatMemory& InFStatMemory)
{
    // 内存类型的字体样式
    const FTextBlockStyle HeaderTextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16))
        .SetColorAndOpacity(FLinearColor::White);

    // 内存具体大小的字体样式
    const FTextBlockStyle TextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Light.ttf"), 16))
        .SetColorAndOpacity(FLinearColor::White);

    // 垂直框
    auto MallocBinned2Panel =
        SNew(SVerticalBox)
        .Clipping(EWidgetClipping::Inherit)

        // Constants.BinnedPageSize
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Constants.BinnedPageSize_LOC", "Constants.BinnedPageSize:"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.ConstantsBinnedPageSize))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Constants.BinnedAllocationGranularity
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Constants.BinnedAllocationGranularity_LOC", "Constants.BinnedAllocationGranularity:"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.ConstantsBinnedAllocationGranularity))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Small Pool Allocations(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Small Pool Allocations_LOC", "Small Pool Allocations(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.SmallPoolAllocations))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Small Pool OS Allocated(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Small Pool OS Allocated_LOC", "Small Pool OS Allocated(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.SmallPoolOSAllocated))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        //Large Pool Requested Allocations(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Large Pool Requested Allocations_LOC", "Large Pool Requested Allocations(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.LargePoolRequestedAllocations))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Large Pool OS Allocated(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Large Pool OS Allocated_LOC", "Large Pool OS Allocated(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.LargePoolOSAllocated))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Requested Allocations(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Requested Allocations_LOC", "Requested Allocations(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.RequestedAllocations))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // OS Allocated(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("OS Allocated_LOC", "OS Allocated(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.OSAllocated))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // PoolInfo(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("PoolInfo_LOC", "PoolInfo(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.PoolInfo))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Hash(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Hash_LOC", "Hash(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.Hash))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // TLS(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("TLS_LOC", "TLS(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.TLS))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Total allocated from OS(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Total allocated from OS_LOC", "Total allocated from OS(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.TotalAllocatedFromOS))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]

        // Cached free OS pages(MB)
        + SVerticalBox::Slot()
        .FillHeight(1)
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(FMargin(0,2))
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Cached free OS pages_LOC", "Cached free OS pages(MB):"))
                .TextStyle(&TextBlockStyle)
            ]

            + SHorizontalBox::Slot()
            .FillWidth(1)
            .HAlign(HAlign_Right)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.CachedFreeOSPages))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]
    ;
    return MallocBinned2Panel;
}

FTextBlockStyle SStatTab::GetStatTextBlockStyle()
{
    FTextBlockStyle TextBlockStyle =
        FTextBlockStyle()
        .SetFont(FSlateFontInfo("Roboto", 20))
        .SetTypefaceFontName("Bold")
        .SetFontSize(24)
        .SetColorAndOpacity(FLinearColor::White);
    
    return TextBlockStyle;
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
