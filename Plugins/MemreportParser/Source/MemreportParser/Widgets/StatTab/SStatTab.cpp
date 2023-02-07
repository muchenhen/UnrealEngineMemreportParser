// Fill out your copyright notice in the Description page of Project Settings.


#include "SStatTab.h"

#include "SlateOptMacros.h"
#include "Engine/AssetManager.h"
#include "Framework/Styling/TextBlockWidgetStyle.h"
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
                    .HeightOverride(300)
                    .Padding(FMargin(2.0f))
                    .Clipping(EWidgetClipping::ClipToBounds)
                    [
                        ConstructMemoryPanel(StatMemory)
                    ]
                ]
            ]

            + SOverlay::Slot()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Top)
            .Padding(650,10,0,0)
            [
                SNew(SBox)
                .WidthOverride(600)
                .HeightOverride(420)
                .Clipping(EWidgetClipping::ClipToBounds)
                [
                    ConstructFMallocBinned2Panel(StatMemory)
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
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(FText::FromString(InFStatMemory.PhysicalMemoryUsed))
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
            [
                SNew(STextBlock)
                .Clipping(EWidgetClipping::Inherit)
                .Text(LOCTEXT("Peak_LOC", "Peak(MB)"))
                .TextStyle(&HeaderTextBlockStyle)
            ]
        ]
    ;
    
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
                .Text(LOCTEXT("Constants.BinnedPageSizeValue_LOC", "65536"))
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
                .Text(LOCTEXT("Constants.BinnedAllocationGranularityValue_LOC", "65536"))
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
                .Text(LOCTEXT("Small Pool AllocationsValue_LOC", "0"))
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
                .Text(LOCTEXT("Small Pool OS AllocatedValue_LOC", "0"))
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
                .Text(LOCTEXT("Large Pool Requested AllocationsValue_LOC", "0"))
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
                .Text(LOCTEXT("Large Pool OS AllocatedValue_LOC", "0"))
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
                .Text(LOCTEXT("Requested AllocationsValue_LOC", "0"))
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
                .Text(LOCTEXT("OS AllocatedValue_LOC", "0"))
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
                .Text(LOCTEXT("PoolInfoValue_LOC", "0"))
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
                .Text(LOCTEXT("HashValue_LOC", "0"))
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
                .Text(LOCTEXT("TLSValue_LOC", "0"))
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
                .Text(LOCTEXT("Total allocated from OSValue_LOC", "0"))
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
                .Text(LOCTEXT("Cached free OS pagesValue_LOC", "0"))
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
