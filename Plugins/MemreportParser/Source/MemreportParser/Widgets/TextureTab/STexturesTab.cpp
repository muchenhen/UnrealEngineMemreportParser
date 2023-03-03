// Fill out your copyright notice in the Description page of Project Settings.


#include "STexturesTab.h"
#include "STextureOverview.h"
#include "SlateOptMacros.h"
#include "STextureCharts.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "STexturesTab"

void STexturesTab::Construct(const FArguments& InArgs)
{
    TextureMemories = InArgs._TextureMemories;
    TextureTotalStat = InArgs._TextureTotalStat;
    SDockTab::Construct(SDockTab::FArguments()
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SBox)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .FillHeight(30.f)
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .FillWidth(30.f)
                    [
                        SNew(STextureOverview)
                        .TextureTotalStat(TextureTotalStat)
                    ]

                    // + SHorizontalBox::Slot()
                    // .FillWidth(70.f)
                    // [
                    //     SNew(STextureCharts)
                    // ]
                ]

                + SVerticalBox::Slot()
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)
                  .FillHeight(70.f)
                  .Padding(0.0f, 1.0f, 0.0f, 1.0f)
                [
                    SAssignNew(TextureList, STextureList)
                    .TextureMemories(TextureMemories)
                ]
            ]
        ]);
}

void STexturesTab::SetTextureMemoryLists(TArray<FTextureMemory> InTextureMemories) const
{
    for (auto& InTextureMemory : InTextureMemories)
    {
        const FTextureMemoryPtr TextureMemoryPtr = MakeShareable(new FTextureMemory);
        TextureMemoryPtr.Get()->CookedSize = InTextureMemory.CookedSize;
        TextureMemoryPtr.Get()->CookedWidth = InTextureMemory.CookedWidth;
        TextureMemoryPtr.Get()->AuthoredBias = InTextureMemory.AuthoredBias;
        TextureMemoryPtr.Get()->InMemSize = InTextureMemory.InMemSize;
        TextureMemoryPtr.Get()->InMemWidth = InTextureMemory.InMemWidth;
        TextureMemoryPtr.Get()->Format = InTextureMemory.Format;
        TextureMemoryPtr.Get()->LODGroup = InTextureMemory.LODGroup;
        TextureMemoryPtr.Get()->Name = InTextureMemory.Name;
        TextureMemoryPtr.Get()->Streaming = InTextureMemory.Streaming;
        TextureMemoryPtr.Get()->UsageCount = InTextureMemory.UsageCount;
        InTextureMemories.Add(*TextureMemoryPtr);
    }
    if (TextureList.IsValid())
    {
        TextureList->RequestListRefresh();
    }
}

void STexturesTab::SetTextureTotalStat(FTextureTotalStat InTextureTotalStat)
{
    const FTextureTotalStatPtr InTextureTotalStatPtr = MakeShareable(new FTextureTotalStat(InTextureTotalStat));
    TextureTotalStat = InTextureTotalStatPtr;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
