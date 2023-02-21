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
    .SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20))
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
        ];

    return ClassColumn;
}

TSharedRef<SWidget> SExportTab::ConstructCountColumn(const FMemreportFile& InMemreportFile)
{
    return SNew(SVerticalBox);
}

TSharedRef<SWidget> SExportTab::ConstructNumColumn(const FMemreportFile& InMemreportFile)
{
    return SNew(SVerticalBox);
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
