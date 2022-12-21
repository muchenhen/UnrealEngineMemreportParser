// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppMemberFunctionMayBeStatic
#include "SExportTab.h"

#include "MemreportParserManager.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "SExportTab"

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

void SExportTab::Construct(const FArguments& InArgs)
{
    SDockTab::Construct(SDockTab::FArguments()
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SBox)
            .Padding(FMargin(300.f))
            [
                SNew(SHorizontalBox)
                
                + SHorizontalBox::Slot()
                .AutoWidth()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Padding(FMargin(15.0f))
                [
                    SNew(SButton)
                    .Text(LOCTEXT("ExportObjectsCSV", "Export Objects CSV"))
                    .OnClicked(this, &SExportTab::ExportObjectsCSV)
                ]

                + SHorizontalBox::Slot()
                .AutoWidth()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Padding(FMargin(15.0f))
                [
                    SNew(SButton)
                    .Text(LOCTEXT("ExportActorsCSV", "Export Actors CSV"))
                    .OnClicked(this, &SExportTab::ExportActorsCSV)
                ]

                + SHorizontalBox::Slot()
                .AutoWidth()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                .Padding(FMargin(15.0f))
                [
                    SNew(SButton)
                    .Text(LOCTEXT("ExportConfigCacheCSV", "Export ConfigCache CSV"))
                    .OnClicked(this, &SExportTab::ExportConfigCacheCSV)
                ]
            ]
        ]);
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
