#pragma once
DECLARE_LOG_CATEGORY_EXTERN(LogMemreportParser, Display, Display);

struct FStatMemory
{
    FString FileName;
    FString BootTime;
    FString Platform;
    FString PhysicalMemoryUsed;
    FString VirtualMemoryUsed;
    FString SmallPoolAllocations;
    FString SmallPoolOSAllocated;
    FString LargePoolRequestedAllocations;
    FString LargePoolOSAllocated;
};

struct FObj
{
    FString Class;
    FString Count;
    FString NumKB;
    FString MaxKB;
    FString ResExcKB;
    FString ResExcDedSysKB;
    FString ResExcShrSysKB;
    FString ResExcDedVidKB;
    FString ResExcShrVidKB;
    FString ResExcUnkKB;

    static TArray<FString> GetHeader()
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

    TArray<FString> GetDataArray() const
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
};

struct FObjClass
{
    FString Object;
    FString NumKB;
    FString MaxKB;
    FString ResExcKB;
    FString ResExcDedSysKB;
    FString ResExcShrSysKB;
    FString ResExcDedVidKB;
    FString ResExcShrVidKB;
    FString ResExcUnkKB;

    static TArray<FString> GetHeader()
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

    TArray<FString> GetDataArray() const
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
};

struct FObjectsStat
{
    FString Num;
    FString Total;
    FString Max;
    FString Res;
    FString ResDedSys;
    FString ResShrSys;
    FString ResDedVid;
    FString ResShrVid;
    FString ResUnknown;

    void Print() const
    {
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat               Num: %s"), *Num);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat             Total: %s"), *Total);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat               Max: %s"), *Max);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat               Res: %s"), *Res);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResDedSys: %s"), *ResDedSys);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResShrSys: %s"), *ResShrSys);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResDedVid: %s"), *ResDedVid);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat         ResShrVid: %s"), *ResShrVid);
        UE_LOG(LogMemreportParser, Display, TEXT("FObjectsStat        ResUnknown: %s"), *ResUnknown);
    }

    void Empty()
    {
        Num.Empty();
        Total.Empty();
        Max.Empty();
        Res.Empty();
        ResDedSys.Empty();
        ResShrSys.Empty();
        ResDedVid.Empty();
        ResShrVid.Empty();
        ResUnknown.Empty();
    }
};

struct FSpawnedActor
{
    FString TimeUnseen;
    FString TimeAlive;
    FString Distance;
    FString Class;
    FString Name;
    FString Owner;

    static TArray<FString> GetHeader()
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

    TArray<FString> GetDataArray() const
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
};

struct FParticleDynamicMemory
{
    FString Type;
    FString Count;
    FString MaxCount;
    FString Mem;
    FString MaxMem;
    FString GTMem;
    FString GTMemMax;

    static TArray<FString> GetHeader()
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

    TArray<FString> GetDataArray() const
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
};

struct FConfigCache
{
    FString FileName;
    FString NumBytes;
    FString MaxBytes;

    static TArray<FString> GetHeader()
    {
        TArray<FString> Header;
        Header.Add("FileName");
        Header.Add("NumBytes");
        Header.Add("MaxBytes");
        return Header;
    }

    TArray<FString> GetDataArray() const
    {
        TArray<FString> Data;
        Data.Add(FileName);
        Data.Add(NumBytes);
        Data.Add(MaxBytes);
        return Data;
    }
};

/**
 * @brief Texture相关的内存信息每一行的数据解析
 * @example 2048x2048 (2632 KB, ?), 2048x2048 (2632 KB), PF_ASTC_6x6, TEXTUREGROUP_Skybox, /Engine/EngineSky/T_Sky_Blue.T_Sky_Blue, YES, 3
 */
struct FTextureMemory
{
    // 储存里的纹理的宽度和高度
    FString CookedWidth;
    // LOD偏移
    FString AuthoredBias;
    // 储存里的纹理的大小，单位KB
    FString CookedSize;
    // 内存中的纹理的宽度和高度
    FString InMemWidth;
    // 内存中的纹理的大小，单位KB
    FString InMemSize;
    // 纹理的格式
    FString Format;
    // LOD组名
    FString LODGroup;
    // 纹理的路径
    FString Name;
    // 是否是流式加载
    FString Streaming;
    // 使用次数
    FString UsageCount;
};

struct FTextureGroupStat
{
    // 组名 
    FString Name;
    // 组中纹理的大小，内存中
    FString InMem;
    // 组中纹理的大小，磁盘中
    FString OnDisk;
};

struct FTextureTotalStat
{
    // 纹理的总大小，单位KB，内存中
    FString TotalSizeInMem;
    // 纹理的总大小，单位KB，磁盘中
    FString TotalSizeOnDisk;
    // 纹理的总数量
    FString Count;
    // TODO：这是啥？
    FString CountApplicableToMin;
    // 每一个TextureGroup的统计信息
    TArray<FTextureGroupStat> TextureGroupStats;
};

enum ECSVFileType
{
    Obj,
    SpawnedActors,
    ConfigCache
};

struct FLevels
{
    FString MapName;
    // the time it took the last time this package was fully loaded, 0 otherwise.
    FString LoadTime;
    FString StreamingStatus;
};

struct FRHIResourceMemory
{
    // 单位是MB
    FString MemorySize;
    FString Description;
    FString ShortName;
    FString GroupName;
    FString Category;
};

struct FParticleSystem
{
    FString Size;
    FString Name;
    FString PSysSize;
    FString ModuleSize;
    FString ComponentSize;
    FString ComponentCount;
    FString CompResSize;
    FString CompTrueResSize;
};

struct FParticleSystemsTotal
{
    FString Name;
    FString Size;
    FString PSysSize;
    FString ModuleSize;
    FString ComponentSize;
    FString ComponentCount;
    FString ComponentResourceSize;
    FString ComponentTrueResourceSize;
};

/**
 * @brief 单个文件的所有内容都在这里了
 */
struct FMemreportFile
{
    FStatMemory StatMemory;
    TArray<FObj> ObjectList;
    TArray<FSpawnedActor> SpawnedActors;
    TArray<FParticleDynamicMemory> ParticleDynamicMemories;
    TArray<FConfigCache> ConfigCaches;
    TArray<FTextureMemory> TextureMemories;
    FTextureTotalStat TextureTotalStat;
    TArray<FObjClass> SkeletalMeshObjects;
    TArray<FObjClass> StaticMeshObjects;
    TArray<FObjClass> LevelObjects;
    TArray<FObjClass> StaticMeshComponentObjects;
    TArray<FLevels> Levels;
    TArray<FRHIResourceMemory> RHIResourceMemories;
    TArray<FParticleSystem> ParticleSystems;
    FParticleSystemsTotal ParticleSystemsTotal;

    bool IsValid() const
    {
        if (StatMemory.Platform.IsEmpty())
        {
            return false;
        }
        return true;
    }
};

struct FMemreportViewModel
{
    FString FileName;
    FString Platform;
    FString Time;
};
