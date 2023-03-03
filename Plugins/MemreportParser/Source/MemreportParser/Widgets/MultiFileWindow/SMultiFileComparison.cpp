// Fill out your copyright notice in the Description page of Project Settings.


#include "SMultiFileComparison.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMultiFileComparison::Construct(const FArguments& InArgs)
{
    MemreportFiles = InArgs._MemreportFiles;
    SWindow::Construct(SWindow::FArguments()
        // .Title(LOCTEXT("AddContentDialogTitle", "Add Content to the Project"))
        .SizingRule(ESizingRule::UserSized)
        .ClientSize(FVector2D(1280, 720))
        .SupportsMinimize(false)
        .SupportsMaximize(false)
        [
            SNew(SBorder)
            [
                SNew(SBox)
            ]
        ]
    );
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
