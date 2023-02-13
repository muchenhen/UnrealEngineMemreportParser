// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateOptMacros.h"
#include "Styling/SlateTypes.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SLeafWidget.h"
/**
 * 
 */
class MEMREPORTPARSER_API SHistogramWidget : public SLeafWidget
{
public:
    SLATE_BEGIN_ARGS(SHistogramWidget)
    : _DataInput()
    , _Width(100)
    , _Height(100)
    { }

    SLATE_ATTRIBUTE(TArray<float>, DataInput)
    SLATE_ATTRIBUTE(float, Width)
    SLATE_ATTRIBUTE(float, Height)

    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

    virtual FChildren* GetChildren() override;

    virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

private:
    TAttribute<TArray<float>> DataInput;
    TAttribute<float> Width;
    TAttribute<float> Height;

    // 坐标轴的颜色
    FLinearColor AxisColor = FLinearColor::White;
    // 坐标轴粗细
    float AxisThickness = 2.0f;
    // 坐标轴刻度的颜色
    FLinearColor AxisTickColor = FLinearColor::White;
    // 坐标轴刻度粗细
    float AxisTickThickness = 1.0f;
    // 坐标轴刻度长度
    float AxisTickLength = 5.0f;
    // 坐标轴刻度文字的颜色
    FLinearColor AxisTickTextColor = FLinearColor::White;
    // 坐标轴刻度文字的大小
    float AxisTickTextSize = 10.0f;
};
