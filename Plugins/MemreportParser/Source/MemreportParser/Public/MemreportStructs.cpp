#include "MemreportStructs.h"

TArray<FString> FObj::GetHeader()
{
    TArray<FString> Header;
    Header.Add("Class");
    Header.Add("Count");
    Header.Add("NumKB");
    Header.Add("MaxKB");
    Header.Add("ResExcKB");
    Header.Add("ResExcDedSysKB");
    Header.Add("ResExcShrSysKB");
    Header.Add("ResExcDedVidKB");
    Header.Add("ResExcShrVidKB");
    Header.Add("ResExcUnkKB");
    return Header;
}

TArray<FString> FObj::GetDataArray() const
{
    TArray<FString> Data;
    Data.Add(Class);
    Data.Add(Count);
    Data.Add(NumKB);
    Data.Add(MaxKB);
    Data.Add(ResExcKB);
    Data.Add(ResExcDedSysKB);
    Data.Add(ResExcShrSysKB);
    Data.Add(ResExcDedVidKB);
    Data.Add(ResExcShrVidKB);
    Data.Add(ResExcUnkKB);
    return Data;
}

TArray<FString> FObjClass::GetHeader()
{
    TArray<FString> Header;
    Header.Add("Object");
    Header.Add("NumKB");
    Header.Add("MaxKB");
    Header.Add("ResExcKB");
    Header.Add("ResExcDedSysKB");
    Header.Add("ResExcShrSysKB");
    Header.Add("ResExcDedVidKB");
    Header.Add("ResExcShrVidKB");
    Header.Add("ResExcUnkKB");
    return Header;
}

TArray<FString> FObjClass::GetDataArray() const
{
    TArray<FString> Data;
    Data.Add(Object);
    Data.Add(NumKB);
    Data.Add(MaxKB);
    Data.Add(ResExcKB);
    Data.Add(ResExcDedSysKB);
    Data.Add(ResExcShrSysKB);
    Data.Add(ResExcDedVidKB);
    Data.Add(ResExcShrVidKB);
    Data.Add(ResExcUnkKB);
    return Data;
}

void FObjectsStat::Print() const
{
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat               Count: %s"), *Count);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat             Total: %s"), *Total);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat               Max: %s"), *Max);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat               Res: %s"), *Res);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResDedSys: %s"), *ResDedSys);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResShrSys: %s"), *ResShrSys);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResDedVid: %s"), *ResDedVid);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResShrVid: %s"), *ResShrVid);
    UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat        ResUnknown: %s"), *ResUnknown);
}
void FObjectsStat::Empty()
{
    Count.Empty();
    Total.Empty();
    Max.Empty();
    Res.Empty();
    ResDedSys.Empty();
    ResShrSys.Empty();
    ResDedVid.Empty();
    ResShrVid.Empty();
    ResUnknown.Empty();
}

TArray<FString> FSpawnedActor::GetHeader()
{
    TArray<FString> Header;
    Header.Add("TimeUnseen");
    Header.Add("TimeAlive");
    Header.Add("Distance");
    Header.Add("Class");
    Header.Add("Name");
    Header.Add("Owner");
    return Header;
}

TArray<FString> FSpawnedActor::GetDataArray() const
{
    TArray<FString> Data;
    Data.Add(TimeUnseen);
    Data.Add(TimeAlive);
    Data.Add(Distance);
    Data.Add(Class);
    Data.Add(Name);
    Data.Add(Owner);
    return Data;
}

TArray<FString> FParticleDynamicMemory::GetHeader()
{
    TArray<FString> Header;
    Header.Add("Type");
    Header.Add("Count");
    Header.Add("MaxCount");
    Header.Add("Mem");
    Header.Add("MaxMem");
    Header.Add("GTMem");
    Header.Add("GTMemMax");
    return Header;
}

TArray<FString> FParticleDynamicMemory::GetDataArray() const
{
    TArray<FString> Data;
    Data.Add(Type);
    Data.Add(Count);
    Data.Add(MaxCount);
    Data.Add(Mem);
    Data.Add(MaxMem);
    Data.Add(GTMem);
    Data.Add(GTMemMax);
    return Data;
}

TArray<FString> FConfigCache::GetHeader()
{
    TArray<FString> Header;
    Header.Add("FileName");
    Header.Add("NumBytes");
    Header.Add("MaxBytes");
    return Header;
}

TArray<FString> FConfigCache::GetDataArray() const
{
    TArray<FString> Data;
    Data.Add(FileName);
    Data.Add(NumBytes);
    Data.Add(MaxBytes);
    return Data;
}

TArray<FString> FTextureMemory::GetHeader()
{
    TArray<FString> Header;
    Header.Add("CookedWidth");
    Header.Add("AuthoredBias");
    Header.Add("CookedSize");
    Header.Add("InMemWidth");
    Header.Add("InMemSize");
    Header.Add("Format");
    Header.Add("LODGroup");
    Header.Add("Name");
    Header.Add("Streaming");
    Header.Add("UsageCount");
    return Header;
}

TArray<FString> FTextureMemory::GetDataArray() const
{
    TArray<FString> Data;
    Data.Add(CookedWidth);
    Data.Add(AuthoredBias);
    Data.Add(CookedSize);
    Data.Add(InMemWidth);
    Data.Add(InMemSize);
    Data.Add(Format);
    Data.Add(LODGroup);
    Data.Add(Name);
    Data.Add(Streaming);
    Data.Add(UsageCount);
    return Data;
}

TArray<FString> FParticleSystem::GetHeader()
{
    TArray<FString> Headers;
    Headers.Add("Size");
    Headers.Add("Name");
    Headers.Add("PSysSize");
    Headers.Add("ModuleSize");
    Headers.Add("ComponentSize");
    Headers.Add("ComponentCount");
    Headers.Add("CompResSize");
    Headers.Add("CompTrueResSize");
    return Headers;
}

TArray<FString> FParticleSystem::GetDataArray() const
{
    TArray<FString> Data;
    Data.Add(Size);
    Data.Add(Name);
    Data.Add(PSysSize);
    Data.Add(ModuleSize);
    Data.Add(ComponentSize);
    Data.Add(ComponentCount);
    Data.Add(CompResSize);
    Data.Add(CompTrueResSize);
    return Data;
}

FString FParticleSystemsTotal::GetSizeInKB() const
{
    const int Bytes =  FCString::Atoi(*Size);
    const int KB = Bytes / 1024;
    return FString::FromInt(KB);
}

bool FMemreportFile::IsValid() const
{
    if (StatMemory.Platform == "")
    {
        UE_LOG(LogTemp, Error, TEXT("StatMemory.Platform is empty, %s"), *StatMemory.Platform);
        return false;
    }
    return true;
}

FString FMemreportFile::GetConfigCacheMemNumInKB() const
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

FString FMemreportFile::GetConfigCacheMaxMemNumInKB() const
{
    int32 Total = 0;
    for (const FConfigCache& ConfigCache : ConfigCaches)
    {
        Total += FCString::Atoi(*ConfigCache.MaxBytes);
    }
    // bytes to kb
    Total /= 1024;
    return FString::FromInt(Total);
}
