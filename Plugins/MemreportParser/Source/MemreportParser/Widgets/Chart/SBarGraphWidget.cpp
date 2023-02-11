// Fill out your copyright notice in the Description page of Project Settings.


#include "SBarGraphWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SBarGraphWidget::Construct(const FArguments& InArgs)
{
    /*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

int32 SBarGraphWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // Calculate the size of the bar
    const FVector2D BarSize = FVector2D(Value * AllottedGeometry.Size.X, AllottedGeometry.Size.Y);

    // Draw the bar
    FSlateBrush Brush;
    Brush.TintColor = FLinearColor::Green;
    Brush.ImageSize = BarSize;
    Brush.DrawAs = ESlateBrushDrawType::Image;
    FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(FVector2D::ZeroVector, BarSize), &Brush, ESlateDrawEffect::None, InWidgetStyle.GetColorAndOpacityTint());
    return LayerId;
}

FVector2D SBarGraphWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
    return FVector2D(100.0f, 100.0f);
}

FChildren* SBarGraphWidget::GetChildren()
{
    return nullptr;
}

void SBarGraphWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
    
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
