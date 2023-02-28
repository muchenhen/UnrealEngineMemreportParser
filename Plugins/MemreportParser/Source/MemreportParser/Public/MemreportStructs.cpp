#include "MemreportStructs.h"

bool FMemreportFile::IsValid() const
{
    if (StatMemory.Platform == "")
    {
        UE_LOG(LogTemp, Error, TEXT("StatMemory.Platform is empty, %s"), *StatMemory.Platform);
        return false;
    }
    return true;
}

FString FMemreportFile::GetConfigCacheMemNum() const
{
    int32 Total = 0;
    for (const FConfigCache& ConfigCache : ConfigCaches)
    {
        Total += FCString::Atoi(*ConfigCache.NumBytes);
    }
    // bytes to kb
    Total /= 1024;
    return FString::FromInt(Total);
}
