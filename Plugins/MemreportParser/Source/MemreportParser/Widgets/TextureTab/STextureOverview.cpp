// Copyright (C) 2007-2022, MuChenhen


#include "STextureOverview.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "STextureOverview"

void STextureOverview::Construct(const FArguments& InArgs)
{
    TextureTotalStat = InArgs._TextureTotalStat;
    ChildSlot
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .FillWidth(0.3f)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .VAlign(EVerticalAlignment::VAlign_Center)
            .FillHeight(25.f)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                  .HAlign(EHorizontalAlignment::HAlign_Left)
                  .AutoWidth()
                [
                    SNew(STextBlock)
                   .Text(LOCTEXT("STextureOverview_TotalNum", "Total Num: "))
                   .Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
                   .ColorAndOpacity(FLinearColor::White)
                ]

                + SHorizontalBox::Slot()
                  .AutoWidth()
                [
                    SNew(STextBlock)
                   .Text(GetTextureTotalNum())
                   .Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
                   .ColorAndOpacity(FLinearColor::White)
                ]
            ]

            + SVerticalBox::Slot()
            .FillHeight(25.f)
            .VAlign(EVerticalAlignment::VAlign_Center)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                  .HAlign(EHorizontalAlignment::HAlign_Left)
                  .AutoWidth()
                [
                    SNew(STextBlock)
                   .Text(LOCTEXT("STextureOverview_TotalSizeInMem", "Total Size In Mem: "))
                   .Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
                   .ColorAndOpacity(FLinearColor::White)
                ]

                + SHorizontalBox::Slot()
                  .AutoWidth()
                [
                    SNew(STextBlock)
                   .Text(GetTextureTotalSizeInMemory())
                   .Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
                   .ColorAndOpacity(FLinearColor::White)
                ]
            ]

            + SVerticalBox::Slot()
            .FillHeight(25.f)
            .VAlign(EVerticalAlignment::VAlign_Center)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                  .HAlign(EHorizontalAlignment::HAlign_Left)
                  .AutoWidth()
                [
                    SNew(STextBlock)
                   .Text(LOCTEXT("STextureOverview_TotalSizeOnDisk", "Total Size On Disk: "))
                   .Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
                   .ColorAndOpacity(FLinearColor::White)
                ]

                + SHorizontalBox::Slot()
                  .AutoWidth()
                [
                    SNew(STextBlock)
                    .Text(GetTextureTotalSizeOnDisk())
                    .Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
                    .ColorAndOpacity(FLinearColor::White)
                ]
            ]
        ]
    ];
	
}

FText STextureOverview::GetTextureTotalNum() const
{
    return FText::FromString(TextureTotalStat.Get()->Count);
}

FText STextureOverview::GetTextureTotalSizeInMemory() const
{
    return FText::FromString(TextureTotalStat.Get()->TotalSizeInMem + " MB");
}

FText STextureOverview::GetTextureTotalSizeOnDisk() const
{
    return FText::FromString(TextureTotalStat.Get()->TotalSizeOnDisk + " MB");
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
