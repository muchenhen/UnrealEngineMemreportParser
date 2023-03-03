//Copyright MuChenhen All Rights Reserved.


#include "SHistogramWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHistogramWidget::Construct(const FArguments& InArgs)
{
    DataInput = InArgs._DataInput;
    Width = InArgs._Width;
    Height = InArgs._Height;
}

int32 SHistogramWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // DataInput.Get().Num() 为柱状图的个数 是0的话就不画了
    if (DataInput.Get().Num() == 0)
    {
        return LayerId;
    }

    // 边缘
    const FMargin Border = FMargin(10.0f, 10.0f, 10.0f, 10.0f);
    
    // 下方所有计算需要考虑边缘的影响
    // 实际的绘制高度和宽度    
    const float DrawHeight = Height.Get() - Border.Top - Border.Bottom;
    const float DrawWidth = Width.Get() - Border.Left - Border.Right;
    // 计算位置时的偏移量
    const float OffsetX = Border.Left;
    const float OffsetY = Border.Top;
    // 实际的00点位置
    const FVector2D Origin = FVector2D(OffsetX, OffsetY + DrawHeight);
    // Y轴正方向
    const FVector2D YAxis = FVector2D(0.0f, -1.0f);
    // X轴正方向
    const FVector2D XAxis = FVector2D(1.0f, 0.0f);
    
    // X轴和Y轴的起始和结束位置
    const FVector2D XAxisStart = Origin;
    const FVector2D XAxisEnd = Origin + XAxis * DrawWidth;
    const FVector2D YAxisStart = Origin;
    const FVector2D YAxisEnd = Origin + YAxis * DrawHeight;
    
    TArray<FVector2D> XPoints;
    XPoints.Add(XAxisStart);
    XPoints.Add(XAxisEnd);
    TArray<FVector2D> YPoints;
    YPoints.Add(YAxisStart);
    YPoints.Add(YAxisEnd);

    // 画X轴
    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        XPoints,
        ESlateDrawEffect::None,
        AxisColor,
        true,
        AxisThickness
    );
    
    // 画Y轴
    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        YPoints,
        ESlateDrawEffect::None,
        AxisColor,
        true,
        AxisThickness
    );
    

    // 根据数据之中的最大值和最小值以及Y轴的高度 计算出Y轴的比例
    // 计算考虑边缘
    float MaxValue = DataInput.Get()[0];
    float MinValue = DataInput.Get()[0];
    for (int i = 0; i < DataInput.Get().Num(); i++)
    {
        if (DataInput.Get()[i] > MaxValue)
        {
            MaxValue = DataInput.Get()[i];
        }
        if (DataInput.Get()[i] < MinValue)
        {
            MinValue = DataInput.Get()[i];
        }
    }
    // Y轴的比例
    const float YScale = DrawHeight / MaxValue;
    // Y轴分为5个区间
    const float YInterval = MaxValue / 5;
    // 计算出Y轴的刻度值 一共5个刻度值 不从0开始
    TArray<float> YScalePoints;
    for (int i = 0; i < 5; i++)
    {
        YScalePoints.Add(MinValue + YInterval * (i + 1));
    }
    // 计算出Y轴的刻度的位置 一共5个刻度的位置 注意计算边缘 从下到上 不从0开始
    TArray<FVector2D> YScalePointsPosition;
    for (int i = 1; i <= 5; i++)
    {
        FVector2D Point = Origin + YAxis * i * YInterval * YScale;
        YScalePointsPosition.Add(Point);
    }
    // 画Y轴的刻度 注意计算边缘
    for (int i = 0; i < 5; i++)
    {
        TArray<FVector2D> Points;
        FVector2D IndexPointStart = YScalePointsPosition[i];
        FVector2D IndexPointEnd = IndexPointStart + XAxis * 5;
        Points.Add(IndexPointStart);
        Points.Add(IndexPointEnd);
        FSlateDrawElement::MakeLines(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(),
            Points,
            ESlateDrawEffect::None,
            AxisTickColor,
            true,
            AxisTickThickness
        );
        
        FText Text = FText::FromString(FString::SanitizeFloat(YScalePoints[i]));
        // 计算出Y轴的刻度的值的位置 注意计算边缘
        FVector2D TextPosition = IndexPointEnd + XAxis * 5;
        // 画Y轴的刻度的值
        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(TextPosition, FVector2D(100, 100)),
            Text,
            FCoreStyle::Get().GetFontStyle("NormalFont"),
            ESlateDrawEffect::None,
            FLinearColor::White
        );
    }

    // 柱子间隔固定为10
    constexpr float BarInterval = 10;

    // 第一个柱子要和Y轴保持距离
    constexpr float FirstBarOffset = 35;

    // 柱子的宽度
    const float BarWidth = (DrawWidth - FirstBarOffset - BarInterval * (DataInput.Get().Num() - 1)) / DataInput.Get().Num();
    
    // 画柱状图 MakeBox
    for (int i = 0; i < DataInput.Get().Num(); i++)
    {
        // 计算出柱子的位置
        FVector2D BarPosition = Origin + XAxis * (FirstBarOffset + i * (BarWidth + BarInterval));
        // 由于UE的坐标系是左上角，真正的位置需要向上移动一个柱子的高度
        BarPosition += YAxis * DataInput.Get()[i] * YScale;
        // 计算出柱子的大小
        FVector2D BarSize = FVector2D(BarWidth, DataInput.Get()[i] * YScale);
       
        // 画柱子
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(BarPosition, BarSize),
            FEditorStyle::GetBrush("WhiteBrush"),
            ESlateDrawEffect::None,
            FLinearColor::Red);
        // 画柱子的值
        FText Text = FText::FromString(FString::SanitizeFloat(DataInput.Get()[i]));
        // 柱子值的位置 要在柱子的上方 对于柱子居中
        FVector2D TextPosition = BarPosition + FVector2D(BarWidth / 2, -10);
        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(TextPosition, FVector2D(100, 100)),
            Text,
            FCoreStyle::Get().GetFontStyle("NormalFont"),
            ESlateDrawEffect::None,
            FLinearColor::White
        );
    }
    
    return LayerId;
}

FVector2D SHistogramWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
    return FVector2D(Width.Get(), Height.Get());
}

FChildren* SHistogramWidget::GetChildren()
{
    return nullptr;
}

void SHistogramWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
