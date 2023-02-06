// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MemreportStructs.h"
#include "MemreportParserManager.generated.h"

/**
 * 解析方法，解析结果数据和导出csv的方法
 */
UCLASS()
class MEMREPORTPARSER_API UMemreportParserManager : public UObject
{
    GENERATED_BODY()
private:
    // 文件夹路径
    static FString FileFolder;

    // 文件夹下所有文件名
    static TArray<FString> FileNames;

    // 文件名 -文件内容
    static TMap<FString, FString> FileContents;

    // 文件名 - 解析后的数据
    static TMap<FString, FMemreportFile> FilesData;

    // 当前的文件名
    // TODO：测试时用的，需要修改具体的使用，可以考虑作为当前单选的文件的文件名，用于打开单独的窗口
    static FString CurrentFileName;

public:
    // 设置当前的文件夹路径
    static void SetFileFolder(const FString& InFileFolder);

    // 加载文件内容
    static void LoadFiles();

    // 获取文件头信息
    static TArray<FStatMemory> GetStats();

    static FMemreportFile GetFileData(const FString& InFileName);

    // 卸载全部文件
    static void OnCloseParser();
private:
    // Object的stat信息 该变量用于Objects -alphasort
    static FObjectsStat AlphasortObjectsStat;

    // Object的stat信息 该变量用于Obj List: class=Level -alphasort
    static FObjectsStat LevelObjectsStat;

    // Object的stat信息 该变量用于Obj List: class=StaticMeshComponent -alphasort
    static FObjectsStat StaticMeshComponentObjectsStat;

    // Object的stat信息 该变量用于Obj List: class=StaticMesh -alphasort
    static FObjectsStat StaticMeshObjectsStat;

    // Object的stat信息 该变量用于Obj List: class=SkeletalMesh -alphasort
    static FObjectsStat SkeletalMeshObjectsStat;

public:
    // 解析单个文件
    static FMemreportFile FileParser(const FString& FileContent);

    // 解析文件头
    static FStatMemory StatParser(const TArray<FString>& StringArray, const int& StartStat, const int& EndStat);

    // 解析RHI相关
    static TArray<FRHIResourceMemory> RHIResourceMemoryParser(const TArray<FString>& StringArray, const int& StartRHI, const int& EndRHI);

    // 解析Levels相关
    static TArray<FLevels> LevelsParser(const TArray<FString>& StringArray, const int& StartLevels, const int& EndLevels);

    // 解析SpawnedActor
    static TArray<FSpawnedActor> SpawnedActorsParser(const TArray<FString>& StringArray, const int& StartActors, const int& EndActors);

    // TODO：解析粒子相关 
    static TArray<FParticleDynamicMemory> ParticleDynamicMemoryParser(const TArray<FString>& StringArray, const int& StartParticleDynamicMemory, const int& EndParticleDynamicMemory);

    // 解析config相关
    static TArray<FConfigCache> ConfigCacheMemoryParser(const TArray<FString>& StringArray, const int& StartConfigCache, const int& EndConfigCache);

    // TODO：解析Pooled Render Targets相关

    // 解析Texture相关 TextureList
    static TArray<FTextureMemory> TextureMemoryParser(const TArray<FString>& StringArray, const int& StartTextures, const int& EndTextures);
    // Texture信息单行处理
    static FTextureMemory TextureMemoryRowSplit(const FString& String);
    // Texture信息最后的Total处理
    static FTextureTotalStat TextureTotalStatParser(const TArray<FString>& StringArray, const int& StartTextureTotal, const int& EndTextureTotal);

    // TODO：解析Sounds相关

    // TODO：解析ParticleSystems
    static void ParticleSystemsParser(const TArray<FString>& StringArray, const int& StartParticleSystems, const int& EndParticleSystems, FMemreportFile& MemreportFile);

    // 解析Object List相关
    static TArray<FObj> ObjParser(const TArray<FString>& StringArray, const int& StartObj, const int& EndObj);

    // TODO：解析Objects SoundWave类型相关

    // 解析Objects SkeletalMesh类型相关
    static TArray<FObjClass> SkeletalMeshParser(const TArray<FString>& StringArray, const int& StartSkeletalMesh, const int& EndSkeletalMesh);

    // 解析Objects StaticMesh类型相关
    static TArray<FObjClass> StaticMeshParser(const TArray<FString>& StringArray, const int& StartStaticMesh, const int& EndStaticMesh);

    // 解析Objects Level类型相关
    static TArray<FObjClass> LevelObjectParser(const TArray<FString>& StringArray, const int& StartLevel, const int& EndLevel);

    // 解析Objects StaticMeshComponent类型相关
    static TArray<FObjClass> StaticMeshComponentParser(const TArray<FString>& StringArray, const int& StartStaticMeshComponent, const int& EndStaticMeshComponent);

    // 解析Objects的总结行 是一个出现较多的东西
    static FObjectsStat ObjectsStatParser(const FString& String);

public: // 以下是一些导出文件相关的函数
    
    // 把字符串数组转成一个字符串
    static FString TransFStringArrayToFString(const TArray<FString>& FStringArray);

    // 生成CSV文件名
    static FString GetCSVFileName(const FString& OriFileName, ECSVFileType CSVFileType);

    static bool CheckCurrentFile();

    // 输出所有Object的信息到CSV文件
    static void SaveObjListToCSV();

    // 输出所有的SpawnedActor的信息到CSV文件
    static void SaveSpawnedActorsToCSV();

    // 输出所有的Config信息到CSV文件
    static void SaveConfigCacheMemoryToCSV();

    // 输出所有的Texture信息到CSV文件
    static void SaveTexturesToCSV();
};
