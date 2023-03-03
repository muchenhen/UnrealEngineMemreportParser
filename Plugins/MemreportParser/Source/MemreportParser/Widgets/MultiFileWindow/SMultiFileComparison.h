// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MemreportStructs.h"

/**
 * 
 */
class MEMREPORTPARSER_API SMultiFileComparison : public SWindow
{
public:
    SLATE_BEGIN_ARGS(SMultiFileComparison){}
    SLATE_ARGUMENT(TArray<FMemreportFile>, MemreportFiles)
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

private:
    TArray<FMemreportFile> MemreportFiles;
};
