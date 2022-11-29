// Copyright (C) 2007-2022, MuChenhen


#include "STextureListRow.h"

#include "SlateOptMacros.h"
#include "TextureValues.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "STextureListRow"

void STextureListRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
    TextureMemory = InArgs._TextureMemory;

    SMultiColumnTableRow<TSharedPtr<FTextureMemory>>::Construct(FSuperRowType::FArguments(), InOwnerTableView);
}

FText STextureListRow::GetCookedWidth() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->CookedWidth);
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetCookedSize() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->CookedSize + " KB");
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetInMemWidth() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->InMemWidth);
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetInMemSize() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->InMemSize + " KB");
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetFormat() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->Format);
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetLODGroup() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->LODGroup);
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetName() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->Name);
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetStreaming() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->Streaming);
    }
    return FText::GetEmpty();
}

FText STextureListRow::GetUsageCount() const
{
    if (TextureMemory.IsValid())
    {
        return FText::FromString(TextureMemory->UsageCount);
    }
    return FText::GetEmpty();
}

TSharedRef<SWidget> STextureListRow::GetOperate()
{
    return
    SNew(SBox)
    .HAlign(HAlign_Center)
    .VAlign(VAlign_Center)
    [
        SNew(SButton)
        .Text(LOCTEXT("Operate", "Operate"))
        .ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
        .DesiredSizeScale(FVector2D(0.5f, 0.5f))
        .ContentPadding(FMargin(2.0, 2.0))
        .OnClicked(this, &STextureListRow::OnOperateClicked)
        .Visibility(this, &STextureListRow::GetOperateVisibility)
        .VAlign(VAlign_Center)
        .HAlign(HAlign_Center)
        [
            SNew(SImage)
            .Image(FEditorStyle::GetBrush("SystemWideCommands.FindInContentBrowser"))
        ]
    ];

}

FReply STextureListRow::OnOperateClicked() const
{
    const FString TextureName = TextureMemory->Name;
    UObject* Texture = LoadObject<UObject>(nullptr, *TextureName);
    if (Texture)
    {
        TArray<UObject*> Textures;
        Textures.Add(Texture);
        GEditor->SyncBrowserToObjects(Textures);
    }

    return FReply::Handled();
}

EVisibility STextureListRow::GetOperateVisibility() const
{
    const FString TextureName = TextureMemory->Name;
    const UObject* Texture = LoadObject<UObject>(nullptr, *TextureName);
    if (Texture)
    {
        return EVisibility::Visible;
    }
    return EVisibility::Hidden;
}

TSharedRef<SWidget> STextureListRow::GenerateWidgetForColumn(const FName& ColumnName)
{
    if (ColumnName == ColNameCookedWidth)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetCookedWidth)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameCookedSize)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetCookedSize)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameInMemWidth)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetInMemWidth)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameInMemSize)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetInMemSize)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameFormat)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetFormat)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameLODGroup)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetLODGroup)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameName)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetName)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameStreaming)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetStreaming)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameUsageCount)
    {
        return SNew(SBox)
               .Padding(FMargin(4.0, 0.0))
               [
                   SNew(STextBlock)
                   .Text(this, &STextureListRow::GetUsageCount)
                   // .ToolTip(SMemreportListRow::GetTraceTooltip())
               ];
    }
    if (ColumnName == ColNameOperate)
    {
        return GetOperate();
    }
    return SNew(STextBlock).Text(LOCTEXT("UnknownColumn", "Unknown Column"));
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
