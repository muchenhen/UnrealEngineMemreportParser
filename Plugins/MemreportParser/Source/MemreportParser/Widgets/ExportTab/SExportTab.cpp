// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppMemberFunctionMayBeStatic
#include "SExportTab.h"

#include "MemreportParserManager.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateBoxBrush.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "SExportTab"
#define BOX_CORE_BRUSH( RelativePath, ... ) FSlateBoxBrush( FPaths::EngineContentDir() / "Editor/Slate" / RelativePath + TEXT(".png"), __VA_ARGS__ )


static FTextBlockStyle TitleTextBlockStyle =
    FTextBlockStyle()
    .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 40))
    .SetColorAndOpacity(FLinearColor::White);

static FTextBlockStyle NormalTextBlockStyle =
    FTextBlockStyle()
    .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 14))
    .SetColorAndOpacity(FLinearColor::White);

static FButtonStyle ExportButtonStyle =
    FButtonStyle()
    .SetNormal(BOX_CORE_BRUSH("Common/Button", FVector2D(32, 32), 8.0f / 32.0f))
    .SetHovered(BOX_CORE_BRUSH("Common/Button_Hovered", FVector2D(32, 32), 8.0f / 32.0f))
    .SetPressed(BOX_CORE_BRUSH("Common/Button_Pressed", FVector2D(32, 32), 8.0f / 32.0f))
    .SetNormalPadding(FMargin(30,20))
    .SetPressedPadding(FMargin(30,20));

FReply SExportTab::ExportObjectsCSV()
{
    UMemreportParserManager::SaveObjListToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportSpawnedActorsCSV()
{
    UMemreportParserManager::SaveSpawnedActorsToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportConfigCacheCSV()
{
    UMemreportParserManager::SaveConfigCacheMemoryToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportTexturesCSV()
{
    UMemreportParserManager::SaveTexturesToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportParticleSystemsCSV()
{
    UMemreportParserManager::SaveParticleSystemsToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportSkeletalMeshesCSV()
{
    UMemreportParserManager::SaveSkeletalMeshesToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportStaticMeshesCSV()
{
    UMemreportParserManager::SaveStaticMeshesToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportStaticMeshComponentsCSV()
{
    UMemreportParserManager::SaveStaticMeshComponentsToCSV();
    return FReply::Handled();
}

void SExportTab::Construct(const FArguments& InArgs)
{
    MemreportFile = InArgs._MemreportFile;
    SDockTab::Construct(SDockTab::FArguments()
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SBox)
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                  .FillWidth(1.0f)
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)
                [
                    ConstructClassColumn(MemreportFile)
                ]

                + SHorizontalBox::Slot()
                  .FillWidth(1.0f)
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)
                [
                    ConstructCountColumn(MemreportFile)
                ]

                + SHorizontalBox::Slot()
                  .FillWidth(1.0f)
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)
                [
                    ConstructTotalColumn(MemreportFile)
                ]

                + SHorizontalBox::Slot()
                  .FillWidth(1.0f)
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)
                [
                    ConstructMaxColumn(MemreportFile)
                ]

                + SHorizontalBox::Slot()
                  .FillWidth(1.0f)
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)
                [
                    ConstructResExcColumn(MemreportFile)
                ]

                + SHorizontalBox::Slot()
                  .FillWidth(1.0f)
                  .HAlign(HAlign_Fill)
                  .VAlign(VAlign_Fill)
                [
                    ConstructExportColumn(MemreportFile)
                ]
            ]
        ]);
}

TSharedRef<SWidget> SExportTab::ConstructClassColumn(const FMemreportFile& InMemreportFile)
{
    auto ClassColumn =
        SNew(SVerticalBox)

        // Class
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Class_Loc", "Class"))
            .TextStyle(&TitleTextBlockStyle)
        ]

        // Objects
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Objects_Loc", "Objects"))
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SpawnedActors
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("SpawnedActors_Loc", "SpawnedActors"))
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ConfigCache
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("ConfigCache_Loc", "ConfigCache"))
            .TextStyle(&NormalTextBlockStyle)
        ]

        // Textures
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Textures_Loc", "Textures"))
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ParticleSystems
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("ParticleSystems_Loc", "ParticleSystems"))
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SkeletalMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("SkeletalMesh_Loc", "SkeletalMesh"))
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("StaticMesh_Loc", "StaticMesh"))
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMeshComponent
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("StaticMeshComponent_Loc", "StaticMeshComponent"))
            .TextStyle(&NormalTextBlockStyle)
        ];

    return ClassColumn;
}

TSharedRef<SWidget> SExportTab::ConstructCountColumn(const FMemreportFile& InMemreportFile)
{
    // 数量信息

    // Objects的数量
    const FText ObjectsCount = FText::FromString(InMemreportFile.ObjectsStat.Count);
    // SpawnedActors的数量
    const FText SpawnedActorsCount = FText::FromString(FString::FromInt(InMemreportFile.SpawnedActors.Num()));
    // ConfigCache的数量
    const FText ConfigCacheCount = FText::FromString(FString::FromInt(InMemreportFile.ConfigCaches.Num()));
    // Textures的数量
    const FText TexturesCount = FText::FromString(InMemreportFile.TextureTotalStat.Count);
    // ParticleSystems的数量
    const FText ParticleSystemsCount = FText::FromString(FString::FromInt(InMemreportFile.ParticleSystems.Num()));
    // SkeletalMesh的数量
    const FText SkeletalMeshCount = FText::FromString(InMemreportFile.SkeletalMeshObjectsStat.Count);
    // StaticMesh的数量
    const FText StaticMeshCount = FText::FromString(InMemreportFile.StaticMeshObjectsStat.Count);
    // StaticMeshComponent的数量
    const FText StaticMeshComponentCount = FText::FromString(InMemreportFile.StaticMeshComponentObjectsStat.Count);

    auto CountColumn =
        SNew(SVerticalBox)

        // Count
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Count_Loc", "Count"))
            .TextStyle(&TitleTextBlockStyle)
        ]

        // Objects
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ObjectsCount)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SpawnedActors
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SpawnedActorsCount)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ConfigCache
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ConfigCacheCount)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // Textures
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(TexturesCount)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ParticleSystems
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ParticleSystemsCount)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SkeletalMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SkeletalMeshCount)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshCount)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMeshComponent
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshComponentCount)
            .TextStyle(&NormalTextBlockStyle)
        ];

    return CountColumn;
}

TSharedRef<SWidget> SExportTab::ConstructTotalColumn(const FMemreportFile& InMemreportFile)
{
    // 这里表示的是某类型的内存占用总的大小

    // 所有Objects的内存大小 原始单位是MB
    const FText ObjectsNum = FText::FromString(InMemreportFile.ObjectsStat.Total + " MB");
    // SpawnedActors并没有内存信息
    const FText SpawnedActorsNum = FText();
    // ConfigCache的内存大小 原始单位是byte
    const FText ConfigCacheNum = FText::FromString(InMemreportFile.GetConfigCacheMemNumInKB() + " KB");
    // Textures的内存大小 原始单位是MB
    const FText TexturesNum = FText::FromString(InMemreportFile.TextureTotalStat.TotalSizeInMem + " MB");
    // ParticleSystems的内存大小 原始单位是byte
    const FText ParticleSystemsNum = FText::FromString(InMemreportFile.ParticleSystemsTotal.GetSizeInKB() + " KB");
    // SkeletalMesh的内存大小 原始单位是MB
    const FText SkeletalMeshNum = FText::FromString(InMemreportFile.SkeletalMeshObjectsStat.Total + " MB");
    // StaticMesh的内存大小 原始单位是MB
    const FText StaticMeshNum = FText::FromString(InMemreportFile.StaticMeshObjectsStat.Total + " MB");
    // StaticMeshComponent的内存大小 原始单位是MB
    const FText StaticMeshComponentNum = FText::FromString(InMemreportFile.StaticMeshComponentObjectsStat.Total + " MB");

    auto TotalColumn =
        SNew(SVerticalBox)

        // Count
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Total_Loc", "Total"))
            .TextStyle(&TitleTextBlockStyle)
        ]

        // Objects
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ObjectsNum)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SpawnedActors
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SpawnedActorsNum)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ConfigCache
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ConfigCacheNum)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // Textures
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(TexturesNum)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ParticleSystems
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ParticleSystemsNum)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SkeletalMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SkeletalMeshNum)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshNum)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMeshComponent
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshComponentNum)
            .TextStyle(&NormalTextBlockStyle)
        ];

    return TotalColumn;
}

TSharedRef<SWidget> SExportTab::ConstructMaxColumn(const FMemreportFile& InMemreportFile)
{
    // 以下是某类型的内存占用的峰值

    // Objects的内存峰值 原始单位是MB
    const FText ObjectsMax = FText::FromString(InMemreportFile.ObjectsStat.Max);
    // Spawned Actors并没有内存信息
    const FText SpawnedActorsMax = FText();
    // ConfigCache的内存峰值 原始单位是byte
    const FText ConfigCacheMax = FText::FromString(InMemreportFile.GetConfigCacheMaxMemNumInKB());
    // TODO: 这里的Max应该不是TotalSizeOnDisk，毕竟也许并没有全部加载进来，而这里展示的信息应该是内存实际占用的峰值
    const FText TexturesMax = FText::FromString(InMemreportFile.TextureTotalStat.TotalSizeOnDisk);
    // TODO: 这个应该也不对
    // const FText ParticleSystemsMax = FText::FromString(InMemreportFile.ParticleSystemsTotal.Size);
    // 由于不清楚ParticleSystems的Max是什么，所以暂时不显示
    const FText ParticleSystemsMax = FText();
    // SkeletalMesh的内存峰值 原始单位是MB
    const FText SkeletalMeshMax = FText::FromString(InMemreportFile.SkeletalMeshObjectsStat.Max);
    // StaticMesh的内存峰值 原始单位是MB
    const FText StaticMeshMax = FText::FromString(InMemreportFile.StaticMeshObjectsStat.Max);
    // StaticMeshComponent的内存峰值 原始单位是MB
    const FText StaticMeshComponentMax = FText::FromString(InMemreportFile.StaticMeshComponentObjectsStat.Max);

    auto MaxColumn =
        SNew(SVerticalBox)

        // Max
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Max_Loc", "Max"))
            .TextStyle(&TitleTextBlockStyle)
        ]

        // Objects
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ObjectsMax)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SpawnedActors
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SpawnedActorsMax)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ConfigCache
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ConfigCacheMax)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // Textures
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(TexturesMax)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ParticleSystems
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ParticleSystemsMax)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SkeletalMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SkeletalMeshMax)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshMax)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMeshComponent
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshComponentMax)
            .TextStyle(&NormalTextBlockStyle)
        ];

    return MaxColumn;
}

TSharedRef<SWidget> SExportTab::ConstructResExcColumn(const FMemreportFile& InMemreportFile)
{
    const FText ObjectsResExc = FText::FromString(InMemreportFile.ObjectsStat.Res);
    // Spawned Actors并没有内存信息
    const FText SpawnedActorsResExc = FText();
    // ConfigCache没有此内存信息
    const FText ConfigCacheResExc = FText();
    // Textures没有此内存信息
    const FText TexturesResExc = FText();
    // ParticleSystems没有此内存信息
    const FText ParticleSystemsResExc = FText();
    const FText SkeletalMeshResExc = FText::FromString(InMemreportFile.SkeletalMeshObjectsStat.Res);
    const FText StaticMeshResExc = FText::FromString(InMemreportFile.StaticMeshObjectsStat.Res);
    const FText StaticMeshComponentResExc = FText::FromString(InMemreportFile.StaticMeshComponentObjectsStat.Res);

    auto ResExcColumn =
        SNew(SVerticalBox)

        // Res
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("ResExc_Loc", "Res"))
            .TextStyle(&TitleTextBlockStyle)
        ]

        // Objects
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ObjectsResExc)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SpawnedActors
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SpawnedActorsResExc)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ConfigCache
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ConfigCacheResExc)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // Textures
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(TexturesResExc)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // ParticleSystems
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(ParticleSystemsResExc)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // SkeletalMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(SkeletalMeshResExc)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMesh
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshResExc)
            .TextStyle(&NormalTextBlockStyle)
        ]

        // StaticMeshComponent
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(StaticMeshComponentResExc)
            .TextStyle(&NormalTextBlockStyle)
        ];

    return ResExcColumn;
}

TSharedRef<SWidget> SExportTab::ConstructExportColumn(const FMemreportFile& InMemreportFile)
{
    auto ExportButtonColumn =
        SNew(SVerticalBox)

        // Export
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Export_Loc", "Export"))
            .TextStyle(&TitleTextBlockStyle)
        ]

        // Objects Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .OnClicked(this, &SExportTab::ExportObjectsCSV)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ExportObjects_Loc", "Export Objects CSV"))
            ]
        ]

        // SpawnedActors Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .Text(LOCTEXT("ExportSpawnedActors_Loc", "Export SpawnedActors CSV"))
            .OnClicked(this, &SExportTab::ExportSpawnedActorsCSV)
        ]

        // ConfigCache Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .Text(LOCTEXT("ExportConfigCache_Loc", "Export ConfigCache CSV"))
            .OnClicked(this, &SExportTab::ExportConfigCacheCSV)
        ]

        // Textures Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .Text(LOCTEXT("ExportTextures_Loc", "Export Textures CSV"))
            .OnClicked(this, &SExportTab::ExportTexturesCSV)
        ]

        // ParticleSystems Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .Text(LOCTEXT("ExportParticleSystems_Loc", "Export ParticleSystems CSV"))
            .OnClicked(this, &SExportTab::ExportParticleSystemsCSV)
        ]

        // SkeletalMesh Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .Text(LOCTEXT("ExportSkeletalMesh_Loc", "Export SkeletalMesh CSV"))
            .OnClicked(this, &SExportTab::ExportSkeletalMeshesCSV)
        ]

        // StaticMesh Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .Text(LOCTEXT("ExportStaticMesh_Loc", "Export StaticMesh CSV"))
            .OnClicked(this, &SExportTab::ExportStaticMeshesCSV)
        ]

        // StaticMeshComponent Export Button
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Fill)
          .Padding(10, 10)
        [
            SNew(SButton)
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .ButtonStyle(&ExportButtonStyle)
            .Text(LOCTEXT("ExportStaticMeshComponent_Loc", "Export StaticMeshComponent CSV"))
            .OnClicked(this, &SExportTab::ExportStaticMeshComponentsCSV)
        ];

    return ExportButtonColumn;
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
