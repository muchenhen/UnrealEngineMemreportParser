#pragma once
DECLARE_LOG_CATEGORY_EXTERN(LogMemreportParser, Display, Display);

struct FStatMemory
{
    // 文件名
    FString FileName;
    // 启动至今的时间
    FString SinceBootTime;
    // 平台
    FString Platform;
    // 进程的物理内存使用情况
    FString ProcessPhysicalMemoryUsed;
    // 进程的物理内存峰值
    FString ProcessPhysicalMemoryPeak;
    // 进程的虚拟内存使用情况
    FString ProcessVirtualMemoryUsed;
    // 进程的虚拟内存峰值
    FString ProcessVirtualMemoryPeak;
    // 设备的已使用物理内存
    FString PhysicalMemoryUsed;
    // 设备的可用物理内存
    FString PhysicalMemoryFree;
    // 设备的总物理内存
    FString PhysicalMemoryTotal;
    // 设备的已使用虚拟内存
    FString VirtualMemoryUsed;
    // 设备的可用虚拟内存
    FString VirtualMemoryFree;
    // 设备的总虚拟内存
    FString VirtualMemoryTotal;

    /*---FMallocBinned2 Mem report---*/
    // 内存分配器使用的页面大小
    FString ConstantsBinnedPageSize;
    // 内存分配粒度
    FString ConstantsBinnedAllocationGranularity;
    // 小内存池中的内存分配情况
    FString SmallPoolAllocations;
    // 从操作系统分配的小内存池大小
    FString SmallPoolOSAllocated;
    // 请求从大内存池分配的内存大小
    FString LargePoolRequestedAllocations;
    // 从操作系统分配的大内存池大小
    FString LargePoolOSAllocated;
    // 请求分配的内存总的大小
    FString RequestedAllocations;
    // 从操作系统分配的内存总的大小
    FString OSAllocated;
    // 内存池信息的大小
    FString PoolInfo;
    // 哈希表的大小
    FString Hash;
    // 线程本地存储的大小
    FString TLS;
    // 从操作系统分配的内存总的大小
    FString TotalAllocatedFromOS;
    // 缓存的空闲操作系统页面的大小
    FString CachedFreeOSPages;
    // --Memory Stats
    // 当前game thread内存栈的大小
    FString GameThreadCurrentSize;
    // 线程的页面分配器的分配大小，已使用
    FString ThreadPageAllocatorUsed;
    // 线程的页面分配器的分配大小，未使用
    FString ThreadPageAllocatorUnused;
    // 命名表内存使用情况
    FString NametableMemoryUsage;
    // 资产注册表内存使用情况
    FString AssetRegistryMemoryUsage;
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

    static TArray<FString> GetHeader();

    TArray<FString> GetDataArray() const;
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

    static TArray<FString> GetHeader();

    TArray<FString> GetDataArray() const;
};

struct FObjectsStat
{
    // 表示该类型的对象的数量
    FString Count;
    // 表示该类型的对象的总大小，单位是MB
    FString Total;
    FString Max;
    // 表示该类型的对象在保留时，额外占用的内存大小，单位是MB
    FString Res;
    FString ResDedSys;
    FString ResShrSys;
    FString ResDedVid;
    FString ResShrVid;
    FString ResUnknown;

    void Print() const;

    void Empty();
};

struct FSpawnedActor
{
    FString TimeUnseen;
    FString TimeAlive;
    FString Distance;
    FString Class;
    FString Name;
    FString Owner;

    static TArray<FString> GetHeader();

    TArray<FString> GetDataArray() const;
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

    static TArray<FString> GetHeader();

    TArray<FString> GetDataArray() const;
};

struct FConfigCache
{
    FString FileName;
    FString NumBytes;
    FString MaxBytes;

    static TArray<FString> GetHeader();

    TArray<FString> GetDataArray() const;
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

    static TArray<FString> GetHeader();

    TArray<FString> GetDataArray() const;
};

struct FPFFormatStat
{
    FString Name;
    FString InMem;
    FString OnDisk;
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
    // 表示纹理的数量，不包括LODGroup为TEXTUREGROUP_World的纹理。
    FString CountApplicableToMin;
    // 每一种PF格式的统计信息
    TArray<FPFFormatStat> PFFormatStats;
    // 每一个TextureGroup的统计信息
    TArray<FTextureGroupStat> TextureGroupStats;
};

enum ECSVFileType
{
    Obj,
    SpawnedActors,
    ConfigCache,
    Texture,
    ParticleSystems,
    SkeletalMeshes,
    StaticMeshes,
    StaticMeshComponents
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

    static TArray<FString> GetHeader();
    TArray<FString> GetDataArray() const;
};

struct FParticleSystemsTotal
{
    FString Name;
    // 内存占用大小
    FString Size;
    // TODO: 这个是什么
    FString PSysSize;
    // TODO: 这个是什么
    FString ModuleSize;
    // 组件的大小
    FString ComponentSize;
    // 组件的数量
    FString ComponentCount;
    // 组件的资源大小
    FString ComponentResourceSize;
    // 组件的真实资源大小，运行时的大小
    FString ComponentTrueResourceSize;

public:
    FString GetSizeInKB() const;
};

/**
 * @brief 单个文件的所有内容都在这里了
 */
struct FMemreportFile
{
    FStatMemory StatMemory;
    TArray<FObj> ObjectList;
    FObjectsStat ObjectsStat;
    TArray<FSpawnedActor> SpawnedActors;
    TArray<FParticleDynamicMemory> ParticleDynamicMemories;
    TArray<FConfigCache> ConfigCaches;
    TArray<FTextureMemory> TextureMemories;
    FTextureTotalStat TextureTotalStat;
    TArray<FObjClass> SkeletalMeshObjects;
    FObjectsStat SkeletalMeshObjectsStat;
    TArray<FObjClass> StaticMeshObjects;
    FObjectsStat StaticMeshObjectsStat;
    TArray<FObjClass> LevelObjects;
    TArray<FObjClass> StaticMeshComponentObjects;
    FObjectsStat StaticMeshComponentObjectsStat;
    TArray<FLevels> Levels;
    TArray<FRHIResourceMemory> RHIResourceMemories;
    TArray<FParticleSystem> ParticleSystems;
    FParticleSystemsTotal ParticleSystemsTotal;

    bool IsValid() const;

    FString GetConfigCacheMemNumInKB() const;
    FString GetConfigCacheMaxMemNumInKB() const;
    // FString GetParticleSystems
};

struct FMemreportViewModel
{
    FString FileName;
    FString Platform;
    FString Time;
};
