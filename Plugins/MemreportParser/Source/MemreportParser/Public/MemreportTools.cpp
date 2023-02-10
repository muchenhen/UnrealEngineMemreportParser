#include "MemreportTools.h"

FSlateColor MemreportTools::StringToSlateColor(const FString& ColorString)
{
    uint32 ColorHex = 0;

    if (ColorString.Len() == 6)
    {
        ColorHex = FParse::HexNumber(*ColorString) | 0xFF000000;
    }
    else if (ColorString.Len() == 8)
    {
        ColorHex = FParse::HexNumber(*ColorString);
    }

    return FSlateColor(FLinearColor(
        ((0xFF000000 & ColorHex) >> 24) / 255.0f,
        ((0x00FF0000 & ColorHex) >> 16) / 255.0f,
        ((0x0000FF00 & ColorHex) >> 8) / 255.0f,
        (0x000000FF & ColorHex) / 255.0f
    ));
}
