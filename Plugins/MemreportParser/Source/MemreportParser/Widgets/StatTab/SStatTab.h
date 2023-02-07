// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Docking/SDockableTab.h"

/**
 * 
 */
class MEMREPORTPARSER_API SStatTab : public SDockTab
{
public:
    SLATE_BEGIN_ARGS(SStatTab)
        {
        }

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    
};
