// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MemreportParser/Public/MemreportStructs.h"
#include "CoreMinimal.h"
#include "STextureList.h"
#include "Widgets/SCompoundWidget.h"

typedef TSharedPtr<FTextureMemory> FTextureMemoryPtr;
typedef TSharedPtr<FTextureTotalStat> FTextureTotalStatPtr;
typedef TSharedPtr<SListView<FTextureMemoryPtr>> STextureListViewPtr;
/**
 * 
 */
class MEMREPORTPARSER_API STexturesTab : public SDockTab
{
public:
    SLATE_BEGIN_ARGS(STexturesTab){}
    SLATE_ARGUMENT(TArray<FTextureMemoryPtr>, TextureMemories)
    SLATE_ARGUMENT(FTextureTotalStatPtr, TextureTotalStat)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs);

    void SetTextureMemoryLists(TArray<FTextureMemory> InTextureMemories) const;

    void SetTextureTotalStat(FTextureTotalStat InTextureTotalStat);
    
private:
    STextureListViewPtr TextureList;
    
    TArray<FTextureMemoryPtr> TextureMemories;

    FTextureTotalStatPtr TextureTotalStat;
};
