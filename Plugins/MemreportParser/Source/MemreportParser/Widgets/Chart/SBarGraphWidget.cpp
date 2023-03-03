//Copyright MuChenhen All Rights Reserved.


#include "SBarGraphWidget.h"

#include "SlateOptMacros.h"
#include "Fonts/FontMeasure.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SBarGraphWidget::Construct(const FArguments& InArgs)
{
    Value = InArgs._Value;
    Width = InArgs._Width;
    Color = InArgs._Color;
    TopText = InArgs._TopText;
}

int32 SBarGraphWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // 如果有文字 则先画文字
    if (!TopText.IsEmpty())
    {
        // 计算文字的位置
        FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 10);
        FVector2D TextSize = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(TopText, FontInfo);
        FVector2D TextPos = FVector2D(AllottedGeometry.Size.X / 2 - TextSize.X / 2, -TextSize.Y);

        // 画文字
        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId +1,
            AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, TextSize),
            TopText,
            FontInfo,
            ESlateDrawEffect::None,
            FLinearColor::White
        );

        // 画柱状图
        const FVector2D BarSize = FVector2D(Width, Value);
        FSlateBrush Brush;
        Brush.TintColor = Color;
        Brush.ImageSize = BarSize;
        Brush.DrawAs = ESlateBrushDrawType::Box;
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(FVector2D(0,-20), BarSize),
            &Brush,
            ESlateDrawEffect::None,
            Color);

        return LayerId +1;
    }
    else
    {
        // 画柱状图
        const FVector2D BarSize = FVector2D(Width, Value);
        FSlateBrush Brush;
        Brush.TintColor = Color;
        Brush.ImageSize = BarSize;
        Brush.DrawAs = ESlateBrushDrawType::Box;
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, BarSize),
            &Brush,
            ESlateDrawEffect::None,
            Color);

        return LayerId;
    }

    
    // Calculate the size of the bar
    // const FVector2D BarSize = FVector2D(Width, Value);

    // // Draw the bar
    // FSlateBrush Brush;
    // Brush.TintColor = Color;
    // Brush.ImageSize = BarSize;
    // Brush.DrawAs = ESlateBrushDrawType::Box;
    // FSlateDrawElement::MakeBox(
    //     OutDrawElements,
    //     LayerId,
    //     AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, BarSize),
    //     &Brush,
    //     ESlateDrawEffect::None,
    //     Color);
    //
    // // Draw the text above the bar
    // FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 10);
    // FVector2D TextSize = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(TopText, FontInfo);
    // FVector2D TextPos = FVector2D(AllottedGeometry.Size.X / 2 - TextSize.X / 2, -TextSize.Y);
    //
    // FSlateDrawElement::MakeText(
    //     OutDrawElements,
    //     LayerId + 1,
    //     AllottedGeometry.ToOffsetPaintGeometry(TextPos),
    //     TopText,
    //     FontInfo,
    //     ESlateDrawEffect::None,
    //     FLinearColor::White);

    return LayerId + 1;
}

FVector2D SBarGraphWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
    // 如果有文字，就把文字的高度也算进去
    if (!TopText.IsEmpty())
    {
        return FVector2D(Width, Value + 20);
    }
    return FVector2D(Width, Value);
}

FChildren* SBarGraphWidget::GetChildren()
{
    return nullptr;
}

void SBarGraphWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
    
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
