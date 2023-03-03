//Copyright MuChenhen All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class MEMREPORTPARSER_API SBarGraphWidget : public SLeafWidget
{
public:
    SLATE_BEGIN_ARGS(SBarGraphWidget){}
    SLATE_ARGUMENT(float, Value)
    SLATE_ARGUMENT(float, Width)
    SLATE_ARGUMENT(FLinearColor, Color)
    SLATE_ARGUMENT(FString, TopText)
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

    virtual FChildren* GetChildren() override;

    virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

public:
    // 柱状图高度
    float Value = 10.0f;
    // 柱状图宽度
    float Width = 10.0f;
    // 柱状图颜色
    FLinearColor Color = FLinearColor::Red;
    // 柱子顶部的文字
    FString TopText;
};
