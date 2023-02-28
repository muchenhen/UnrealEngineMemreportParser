// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppMemberFunctionMayBeStatic
#include "SExportTab.h"

#include "MemreportParserManager.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "SExportTab"

static FTextBlockStyle TitleTextBlockStyle = FTextBlockStyle()
                                             .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 40))
                                             .SetColorAndOpacity(FLinearColor::White);

static FTextBlockStyle NormalTextBlockStyle = FTextBlockStyle()
                                              .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 14))
                                              .SetColorAndOpacity(FLinearColor::White);

FReply SExportTab::ExportObjectsCSV()
{
    UMemreportParserManager::SaveObjListToCSV();
    return FReply::Handled();
}

FReply SExportTab::ExportActorsCSV()
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
                    ConstructNumColumn(MemreportFile)
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
    const FText ObjectsCount = FText::FromString(FString::FromInt(InMemreportFile.ObjectList.Num()));
    const FText SpawnedActorsCount = FText::FromString(FString::FromInt(InMemreportFile.SpawnedActors.Num()));
    const FText ConfigCacheCount = FText::FromString(FString::FromInt(InMemreportFile.ConfigCaches.Num()));
    const FText TexturesCount = FText::FromString(InMemreportFile.TextureTotalStat.Count);
    const FText ParticleSystemsCount = FText::FromString(FString::FromInt(InMemreportFile.ParticleSystems.Num()));
    const FText SkeletalMeshCount = FText::FromString(FString::FromInt(InMemreportFile.SkeletalMeshObjects.Num()));
    const FText StaticMeshCount = FText::FromString(FString::FromInt(InMemreportFile.StaticMeshObjects.Num()));
    const FText StaticMeshComponentCount = FText::FromString(FString::FromInt(InMemreportFile.StaticMeshComponentObjects.Num()));

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

TSharedRef<SWidget> SExportTab::ConstructNumColumn(const FMemreportFile& InMemreportFile)
{
    const FText ObjectsNum = FText::FromString(InMemreportFile.ObjectsStat.Num);
    // SpawnedActors并没有内存信息
    const FText SpawnedActorsNum = FText();
    const FText ConfigCacheNum = FText::FromString(InMemreportFile.GetConfigCacheMemNum());
    const FText TexturesNum = FText::FromString(InMemreportFile.TextureTotalStat.TotalSizeInMem);
    const FText ParticleSystemsNum = FText::FromString(InMemreportFile.ParticleSystemsTotal.Size);
    const FText SkeletalMeshNum = FText::FromString(InMemreportFile.SkeletalMeshObjectsStat.Num);
    const FText StaticMeshNum = FText::FromString(InMemreportFile.StaticMeshObjectsStat.Num);
    const FText StaticMeshComponentNum = FText::FromString(InMemreportFile.StaticMeshComponentObjectsStat.Num);

    auto NumColumn =
        SNew(SVerticalBox)

        // Num
        + SVerticalBox::Slot()
          .FillHeight(1)
          .VAlign(VAlign_Fill)
          .HAlign(HAlign_Left)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("Num_Loc", "Num"))
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

    return NumColumn;
}

TSharedRef<SWidget> SExportTab::ConstructMaxColumn(const FMemreportFile& InMemreportFile)
{
    return SNew(SVerticalBox);
}

TSharedRef<SWidget> SExportTab::ConstructResExcColumn(const FMemreportFile& InMemreportFile)
{
    return SNew(SVerticalBox);
}

TSharedRef<SWidget> SExportTab::ConstructExportColumn(const FMemreportFile& InMemreportFile)
{
    return SNew(SVerticalBox);
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
