// Fill out your copyright notice in the Description page of Project Settings.

#include "MemreportParserManager.h"

#include "HAL/FileManager.h"
#include "Internationalization/Regex.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(LogMemreportParser);

#define LOCTEXT_NAMESPACE "MemreportParser"

FString UMemreportParserManager::FileFolder;
TArray<FString> UMemreportParserManager::FileNames;
TMap<FString, FString> UMemreportParserManager::FileContents;
TMap<FString, FMemreportFile> UMemreportParserManager::FilesData;
FString UMemreportParserManager::CurrentFileName;

FObjectsStat UMemreportParserManager::AlphasortObjectsStat;
FObjectsStat UMemreportParserManager::LevelObjectsStat;
FObjectsStat UMemreportParserManager::StaticMeshComponentObjectsStat;
FObjectsStat UMemreportParserManager::StaticMeshObjectsStat;
FObjectsStat UMemreportParserManager::SkeletalMeshObjectsStat;


void UMemreportParserManager::SetFileFolder(const FString& InFileFolder)
{
    FileFolder = InFileFolder;
}

void UMemreportParserManager::LoadFiles()
{
    if (!FileFolder.IsEmpty())
    {
        IFileManager& FileManager = IFileManager::Get();

        if (FileManager.DirectoryExists(*FileFolder))
        {
            TArray<FString> FoundFiles;
            FileContents.Empty();
            FileManager.FindFiles(FoundFiles, *FileFolder, TEXT(".memreport"));
            for (int i = 0; i < FoundFiles.Num(); i++)
            {
                FString FileName = FoundFiles[i].Replace(TEXT(".memreport"), TEXT(""));
                FileNames.Add(FileName);
            }
            for (auto& FileName : FoundFiles)
            {
                FString Content;
                FString Path = FileFolder / FileName;
                FFileHelper::LoadFileToString(Content, *Path);
                FileContents.Add(FileName.Replace(TEXT(".memreport"), TEXT("")), Content);
                FFileStatData FileStatData = FileManager.GetStatData(*Path);
                //TODO: 文件的一些属性
                FileStatData.bIsValid;
                FileStatData.CreationTime;
                FileStatData.FileSize;
            }
            
            for (auto& FileContent : FileContents)
            {
                FString FileName = FileContent.Key.Replace(TEXT(".memreport"), TEXT(""));
                FilesData.Add(FileName) = FileParser(FileContent.Value);
            }
        }
    }
}

FMemreportFile UMemreportParserManager::FileParser(const FString& FileContent)
{
    FMemreportFile FileData;

    TArray<FString> StringArray;
    FileContent.ParseIntoArrayLines(StringArray, false);

    int StartStat = 0;
    int EndStat = 0;

    int StartObj = 0;
    int EndObj = 0;

    int StartRHI = 0;
    int EndRHI = 0;

    int StartLevels = 0;
    int EndLevels = 0;

    int StartActors = 0;
    int EndActors = 0;

    int StartParticleDynamicMemory = 0;
    int EndParticleDynamicMemory = 0;

    int StartConfigCache = 0;
    int EndConfigCache = 0;

    int StartPooledRenderTargets = 0;
    int EndPooledRenderTargets = 0;

    int StartTextures = 0;
    int EndTextures = 0;

    int StartTextureTotal = 0;
    int EndTextureTotal = 0;

    int StartSounds = 0;
    int EndSounds = 0;

    int StartParticleSystems = 0;
    int EndParticleSystems = 0;

    int StartSkeletalMesh = 0;
    int EndSkeletalMesh = 0;

    int StartStaticMesh = 0;
    int EndStaticMesh = 0;

    int StartLevel = 0;
    int EndLevel = 0;

    int StartStaticMeshComponent = 0;
    int EndStaticMeshComponent = 0;

    int i = 0;
    for (auto& String : StringArray)
    {
        // Stats
        if (String.Contains(TEXT("Platform Memory Stats")) && StartStat == 0)
        {
            StartStat = i;
        }
        else if (String.Contains(TEXT("Memory Stats:")) && EndStat == 0 && StartStat != 0)
        {
            EndStat = i;
        }

        // Obj List
        else if (String.Contains(TEXT("Obj List: -alphasort")) && EndObj == 0 && StartObj == 0)
        {
            StartObj = i + 4;
        }
        else if (String.Contains(TEXT("Objects (Total")) && EndObj == 0 && StartObj != 0)
        {
            EndObj = i;
        }

        // RHI resource memory
        else if (String.Contains(TEXT("RHI resource memory")) && StartRHI == 0)
        {
            StartRHI = i + 1;
        }
        else if (String.Contains(TEXT("MB total")) && EndRHI == 0 && StartRHI != 0)
        {
            EndRHI = i;
        }

        // Levels
        else if (String.Contains(TEXT("Levels:")) && StartLevels == 0)
        {
            StartLevels = i + 1;
        }
        else if (String.IsEmpty() && StringArray[i - 1].Contains(TEXT("/Game")) && EndLevels == 0 && StartLevels != 0)
        {
            EndLevels = i - 1;
        }

        // Spawned actors
        else if (String.Contains(TEXT("Listing spawned actors")) && StartActors == 0)
        {
            StartActors = i;
        }
        else if (String.IsEmpty() && StartActors != 0 && EndActors == 0)
        {
            EndActors = i;
        }

        // Particle Dynamic Memory Stats
        else if (String.Contains(TEXT("Particle Dynamic Memory Stats")) && StartParticleDynamicMemory == 0)
        {
            StartParticleDynamicMemory = i + 2;
        }
        else if (String.IsEmpty() && EndParticleDynamicMemory == 0 && StartParticleDynamicMemory != 0)
        {
            EndParticleDynamicMemory = i;
        }

        // Config cache memory
        else if (String.Contains(TEXT("Config cache memory usage:")) && StartConfigCache == 0)
        {
            StartConfigCache = i;
        }
        else if (String.IsEmpty() && EndConfigCache == 0 && StartConfigCache != 0)
        {
            EndConfigCache = i;
        }

        // Pooled Render Targets
        else if (String.Contains(TEXT("Pooled Render Targets")) && StartPooledRenderTargets == 0)
        {
            StartPooledRenderTargets = i;
        }
        else if (String.IsEmpty() && EndPooledRenderTargets == 0)
        {
            EndPooledRenderTargets = i;
        }

        // All textures
        else if (String.Contains(TEXT("Listing all textures")) && StartTextures == 0)
        {
            StartTextures = i;
        }
        // Textures total
        else if (String.Contains(TEXT("Total size:")) && EndTextures == 0 && StartTextures != 0)
        {
            EndTextures = i - 1;
            StartTextureTotal = i;
        }
        else if (String.IsEmpty() && EndTextureTotal == 0 && StartTextureTotal != 0)
        {
            EndTextureTotal = i;
        }

        // TODO: Sounds 缺少资料
        else if (String.Contains(TEXT("Listing all sounds")) && StartSounds == 0)
        {
            StartSounds = i;
        }

        // ParticleSystems
        else if (String.Contains(TEXT("ParticleSystems:")) && StartParticleSystems == 0)
        {
            StartParticleSystems = i + 2;
        }
        else if (String.Contains(TEXT("Total,")) && EndParticleSystems == 0 && StartParticleSystems != 0)
        {
            EndParticleSystems = i;
        }

        // TODO SoundWave 缺少资料
        // TODO 注意最后一行是 Total

        // SkeletalMesh
        else if (String.Contains(TEXT("Obj List: class=SkeletalMesh")) && StartSkeletalMesh == 0)
        {
            StartSkeletalMesh = i + 4;
        }
        else if (String.Contains(TEXT("Objects (Total")) && EndSkeletalMesh == 0 && StartSkeletalMesh != 0)
        {
            EndSkeletalMesh = i;
        }

        // StaticMesh
        else if (String.Contains(TEXT("Obj List: class=StaticMesh")) && StartStaticMesh == 0)
        {
            StartStaticMesh = i + 4;
        }
        else if (String.Contains(TEXT("Objects (Total")) && EndStaticMesh == 0 && StartStaticMesh != 0)
        {
            EndStaticMesh = i;
        }

        // Level
        else if (String.Contains(TEXT("Obj List: class=Level")) && StartLevel == 0)
        {
            StartLevel = i + 4;
        }
        else if (String.Contains(TEXT("Objects (Total")) && EndLevel == 0 && StartLevel != 0)
        {
            EndLevel = i;
        }

        // StaticMeshComponent
        else if (String.Contains(TEXT("Obj List: class=StaticMeshComponent")) && StartStaticMeshComponent == 0)
        {
            StartStaticMeshComponent = i + 4;
        }
        else if (String.Contains(TEXT("Objects (Total")) && EndStaticMeshComponent == 0 && StartStaticMeshComponent != 0)
        {
            EndStaticMeshComponent = i;
        }

        i++;
    }

    if (StartStat != 0 && EndStat != 0)
    {
        const FStatMemory StatMemory = StatParser(StringArray, StartStat, EndStat);
        FileData.StatMemory = StatMemory;
    }

    if (StartObj != 0 && EndObj != 0)
    {
        const TArray<FObj> ObjectList = ObjParser(StringArray, StartObj, EndObj);
        FileData.ObjectList = ObjectList;
    }

    if (StartRHI != 0 && EndRHI != 0)
    {
        const TArray<FRHIResourceMemory> RHIResourceMemories = RHIResourceMemoryParser(StringArray, StartRHI, EndRHI);
        FileData.RHIResourceMemories = RHIResourceMemories;
    }
    if (StartLevels != 0 && EndLevels != 0)
    {
        const TArray<FLevels> Levels = LevelsParser(StringArray, StartLevels, EndLevels);
        FileData.Levels = Levels;
    }

    if (StartActors != 0 && EndActors != 0)
    {
        const TArray<FSpawnedActor> SpawnedActors = SpawnedActorsParser(StringArray, StartActors, EndActors);
        FileData.SpawnedActors = SpawnedActors;
    }

    if (StartParticleDynamicMemory != 0 && EndParticleDynamicMemory != 0)
    {
        const TArray<FParticleDynamicMemory> ParticleDynamicMemoryMemories = ParticleDynamicMemoryParser(StringArray, StartParticleDynamicMemory, EndParticleDynamicMemory);
        FileData.ParticleDynamicMemories = ParticleDynamicMemoryMemories;
    }

    if (StartConfigCache != 0 && EndConfigCache != 0)
    {
        const TArray<FConfigCache> ConfigCaches = ConfigCacheMemoryParser(StringArray, StartConfigCache, EndConfigCache);
        FileData.ConfigCaches = ConfigCaches;
    }

    if (StartTextures != 0 && EndTextures != 0)
    {
        const TArray<FTextureMemory> TextureMemories = TextureMemoryParser(StringArray, StartTextures, EndTextures);
        FileData.TextureMemories = TextureMemories;
    }

    if (StartTextureTotal != 0 && EndTextureTotal != 0)
    {
        const FTextureTotalStat TextureTotalStat = TextureTotalStatParser(StringArray, StartTextureTotal, EndTextureTotal);
        FileData.TextureTotalStat = TextureTotalStat;
    }

    if (StartParticleSystems != 0 && EndParticleSystems != 0)
    {
        ParticleSystemsParser(StringArray, StartParticleSystems, EndParticleSystems, FileData);
    }

    if (StartSkeletalMesh != 0 && EndSkeletalMesh != 0)
    {
        const TArray<FObjClass> SkeletalMeshObjects = SkeletalMeshParser(StringArray, StartSkeletalMesh, EndSkeletalMesh);
        FileData.SkeletalMeshObjects = SkeletalMeshObjects;
    }

    if (StartStaticMesh != 0 && EndStaticMesh != 0)
    {
        const TArray<FObjClass> StaticMeshObjects = StaticMeshParser(StringArray, StartStaticMesh, EndStaticMesh);
        FileData.StaticMeshObjects = StaticMeshObjects;
    }

    if (StartLevel != 0 && EndLevel != 0)
    {
        const TArray<FObjClass> LevelObjects = LevelObjectParser(StringArray, StartLevel, EndLevel);
        FileData.LevelObjects = LevelObjects;
    }

    if (StartStaticMeshComponent != 0 && EndStaticMeshComponent != 0)
    {
        const TArray<FObjClass> StaticMeshComponentObjects = StaticMeshComponentParser(StringArray, StartStaticMeshComponent, EndStaticMeshComponent);
        FileData.StaticMeshComponentObjects = StaticMeshComponentObjects;
    }

    return FileData;
}

TArray<FStatMemory> UMemreportParserManager::GetStats()
{
    TArray<FStatMemory> StatMemories;
    if (FileContents.Num() > 0)
    {
        for (int i = 0; i < FileContents.Num(); i++)
        {
            CurrentFileName = FileNames[i];
            const FString FileContent = FileContents[CurrentFileName];
            FMemreportFile FileData;

            TArray<FString> StringArray;
            FileContent.ParseIntoArrayLines(StringArray, false);

            int StartStat = 0;
            int EndStat = 0;

            int index = 0;
            for (auto& String : StringArray)
            {
                // Stats
                if (String.Contains(TEXT("CommandLine Options")) && StartStat == 0)
                {
                    StartStat = index + 1;
                }
                else if (String.Contains(TEXT("Memory Stats:")) && EndStat == 0 && StartStat != 0)
                {
                    EndStat = index;
                }
                if (StartStat != 0 && EndStat != 0)
                {
                    break;
                }
                index++;
            }
            FStatMemory StatMemory = StatParser(StringArray, StartStat, EndStat);
            StatMemory.FileName = CurrentFileName;
            StatMemory.FileName = StatMemory.FileName.Replace(TEXT(".memreport"), TEXT(""));
            StatMemories.Add(StatMemory);
        }
    }
    return StatMemories;
}

FMemreportFile UMemreportParserManager::GetFileData(const FString& InFileName)
{
    if (FilesData.Find(InFileName))
    {
        return FilesData[InFileName];
    }
    else
    {
        return FMemreportFile();
    }
}

void UMemreportParserManager::OnCloseParser()
{
    FileFolder.Empty();
    FileNames.Empty();
    FileContents.Empty();
    FilesData.Empty();
    CurrentFileName.Empty();
    AlphasortObjectsStat.Empty();
    LevelObjectsStat.Empty();
    StaticMeshComponentObjectsStat.Empty();
    StaticMeshObjectsStat.Empty();
    SkeletalMeshObjectsStat.Empty();
}

FStatMemory UMemreportParserManager::StatParser(const TArray<FString>& StringArray, const int& StartStat, const int& EndStat)
{
    FStatMemory StatMemory;
    for (int i = StartStat; i <= EndStat; i++)
    {
        const FString String = StringArray[i];

        // Time Since Boot:
        if (String.Contains("Time Since Boot:"))
        {
            FString TempString = String.Replace(TEXT("Seconds"), TEXT(""));
            TempString = TempString.Replace(TEXT(" "), TEXT(""));
            TArray<FString> OutArray;
            TempString.ParseIntoArray(OutArray, TEXT(":"), true);
            StatMemory.SinceBootTime = OutArray[1];
        }

        // Platform
        if (String.Contains("Platform"))
        {
            TArray<FString> OutArray;
            String.ParseIntoArray(OutArray, TEXT(" "), true);
            StatMemory.Platform = OutArray[OutArray.Num() - 1];
        }

        auto GetMB = [String](FString SubStr1, FString SubStr2) {
            int32 StartIndex = String.Find(SubStr1, ESearchCase::IgnoreCase, ESearchDir::FromStart, INDEX_NONE);
            StartIndex += GetNum(SubStr1);
            const int32 EndIndex = String.Find(SubStr2, ESearchCase::IgnoreCase, ESearchDir::FromStart, INDEX_NONE);
            FString AimString;
            for (int j = StartIndex; j <= EndIndex; j++)
            {
                AimString.AppendChar(String[j]);
            }
            const FString EndString = AimString.TrimStartAndEnd();
            return EndString;
        };

        // Process Physical Memory
        if (String.Contains("Process Physical Memory"))
        {
            const FString SubStr1 = "Memory: ";
            const FString SubStr2 = "MB";
            StatMemory.PhysicalMemoryUsed = GetMB(SubStr1, SubStr2);
            StatMemory.PhysicalMemoryUsed = StatMemory.PhysicalMemoryUsed.Replace(TEXT(" "), TEXT(""));
            StatMemory.PhysicalMemoryUsed = StatMemory.PhysicalMemoryUsed.Replace(TEXT("M"), TEXT(""));
            continue;
        }

        // Process Virtual Memory
        if (String.Contains("Process Virtual Memory"))
        {
            const FString SubStr1 = "Memory: ";
            const FString SubStr2 = "MB";
            StatMemory.VirtualMemoryUsed = GetMB(SubStr1, SubStr2);
            StatMemory.VirtualMemoryUsed = StatMemory.VirtualMemoryUsed.Replace(TEXT(" "), TEXT(""));
            StatMemory.VirtualMemoryUsed = StatMemory.VirtualMemoryUsed.Replace(TEXT("M"), TEXT(""));
            continue;
        }

        // Small Pool Allocations
        if (String.Contains("Small Pool Allocations"))
        {
            const FString SubStr1 = "Allocations: ";
            const FString SubStr2 = "mb";
            StatMemory.SmallPoolAllocations = GetMB(SubStr1, SubStr2);
            StatMemory.SmallPoolAllocations = StatMemory.SmallPoolAllocations.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // Small Pool OS Allocated
        if (String.Contains("Small Pool OS Allocated"))
        {
            const FString SubStr1 = "Allocated: ";
            const FString SubStr2 = "mb";
            StatMemory.SmallPoolOSAllocated = GetMB(SubStr1, SubStr2);
            StatMemory.SmallPoolOSAllocated = StatMemory.SmallPoolOSAllocated.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // Large Pool Requested Allocations
        if (String.Contains("Large Pool Requested Allocations"))
        {
            const FString SubStr1 = "Allocations: ";
            const FString SubStr2 = "mb";
            StatMemory.LargePoolRequestedAllocations = GetMB(SubStr1, SubStr2);
            StatMemory.LargePoolRequestedAllocations = StatMemory.LargePoolRequestedAllocations.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // Large Pool OS Allocated
        if (String.Contains("Large Pool OS Allocated"))
        {
            const FString SubStr1 = "Allocated: ";
            const FString SubStr2 = "mb";
            StatMemory.LargePoolOSAllocated = GetMB(SubStr1, SubStr2);
            StatMemory.LargePoolOSAllocated = StatMemory.LargePoolOSAllocated.Replace(TEXT("m"), TEXT(""));
            continue;
        }
    }
    return StatMemory;
}

TArray<FRHIResourceMemory> UMemreportParserManager::RHIResourceMemoryParser(const TArray<FString>& StringArray, const int& StartRHI, const int& EndRHI)
{
    TArray<FRHIResourceMemory> RHIResourceMemories;
    float TotalSize = 0;
    for (int i = StartRHI; i <= EndRHI; i++)
    {
        FString String = StringArray[i];
        if (i == EndRHI)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
        else
        {
            TArray<FString> Outs;
            String.ParseIntoArray(Outs, TEXT(" - "), false);
            FRHIResourceMemory RHIResourceMemory;
            RHIResourceMemory.MemorySize = Outs[0];
            RHIResourceMemory.MemorySize = RHIResourceMemory.MemorySize.Replace(TEXT(" "), TEXT(""));
            float MemorySize = FCString::Atof(*RHIResourceMemory.MemorySize);
            MemorySize /= 1024;
            MemorySize /= 1024;
            TotalSize += MemorySize;
            RHIResourceMemory.MemorySize = FString::SanitizeFloat(MemorySize);
            RHIResourceMemory.Description = Outs[1];
            RHIResourceMemory.ShortName = Outs[2];
            RHIResourceMemory.GroupName = Outs[3];
            RHIResourceMemory.Category = Outs[4];
            RHIResourceMemories.Add(RHIResourceMemory);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("RHIResourceMemories num: %d, total size %f MB"), RHIResourceMemories.Num(), TotalSize);
    return RHIResourceMemories;
}

TArray<FLevels> UMemreportParserManager::LevelsParser(const TArray<FString>& StringArray, const int& StartLevels, const int& EndLevels)
{
    TArray<FLevels> Levels;

    for (int i = StartLevels; i <= EndLevels; i++)
    {
        FLevels OneLevels;
        FString MapName;
        FString String = StringArray[i];
        // 匹配MapName
        const FRegexPattern MapNamePattern(FString(TEXT("/[/A-Za-z0-9]+")));
        FRegexMatcher MapNameMatcher(MapNamePattern, String);
        if (MapNameMatcher.FindNext())
        {
            OneLevels.MapName = MapNameMatcher.GetCaptureGroup(0);
        }
        // 匹配时间
        const FRegexPattern SecPattern(FString(TEXT("[0-9.]+\\b sec\\b")));
        FRegexMatcher SecMatcher(SecPattern, String);
        if (SecMatcher.FindNext())
        {
            OneLevels.LoadTime = SecMatcher.GetCaptureGroup(0);
            OneLevels.LoadTime = OneLevels.LoadTime.Replace(TEXT("sec"), TEXT(""));
            OneLevels.LoadTime = OneLevels.LoadTime.Replace(TEXT(" "), TEXT(""));
        }
        // 加载状态
        if (String.Contains(TEXT("red loaded and visible")))
        {
            OneLevels.StreamingStatus = TEXT("LEVEL_Visible");
        }
        else if (String.Contains(TEXT("orange, in process of being made visible")))
        {
            OneLevels.StreamingStatus = TEXT("LEVEL_MakingVisible");
        }
        else if (String.Contains(TEXT("yellow loaded but not visible")))
        {
            OneLevels.StreamingStatus = TEXT("LEVEL_Loaded");
        }
        else if (String.Contains(TEXT("blue  (GC needs to occur to remove this)")))
        {
            OneLevels.StreamingStatus = TEXT("LEVEL_UnloadedButStillAround");
        }
        else if (String.Contains(TEXT("green Unloaded")))
        {
            OneLevels.StreamingStatus = TEXT("LEVEL_Unloaded");
        }
        else if (String.Contains(TEXT("purple (preloading)")))
        {
            OneLevels.StreamingStatus = TEXT("LEVEL_Preloading");
        }
        Levels.Add(OneLevels);
    }
    UE_LOG(LogMemreportParser, Display, TEXT("Levels Num: %d"), Levels.Num());
    return Levels;
}

TArray<FObj> UMemreportParserManager::ObjParser(const TArray<FString>& StringArray, const int& StartObj, const int& EndObj)
{
    bool bStart = false;
    TArray<FObj> ObjectList;
    for (int i = StartObj; i <= EndObj; i++)
    {
        const FString String = StringArray[i];
        if (String.Contains(TEXT("Class")))
        {
            bStart = true;
            continue;
        }
        if (bStart && !String.IsEmpty())
        {
            TArray<FString> OutArray;
            String.ParseIntoArray(OutArray, TEXT(" "), true);
            FObj Obj;
            Obj.Class = OutArray[0];
            Obj.Count = OutArray[1];
            Obj.NumKB = OutArray[2];
            Obj.MaxKB = OutArray[3];
            Obj.ResExcKB = OutArray[4];
            Obj.ResExcDedSysKB = OutArray[5];
            Obj.ResExcShrSysKB = OutArray[6];
            Obj.ResExcDedVidKB = OutArray[7];
            Obj.ResExcShrVidKB = OutArray[8];
            Obj.ResExcUnkKB = OutArray[9];
            ObjectList.Add(Obj);
        }
        if (String.IsEmpty())
        {
            bStart = false;
        }
        if (!bStart && String.Contains(TEXT("Total: ")))
        {
            AlphasortObjectsStat = ObjectsStatParser(String);
            AlphasortObjectsStat.Print();
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("Objects Num: %d"), ObjectList.Num());
    return ObjectList;
}

TArray<FObjClass> UMemreportParserManager::SkeletalMeshParser(const TArray<FString>& StringArray, const int& StartSkeletalMesh, const int& EndSkeletalMesh)
{
    TArray<FObjClass> SkeletalMeshObjects;
    for (int i = StartSkeletalMesh; i <= EndSkeletalMesh; i++)
    {
        FString String = StringArray[i];
        if (!String.IsEmpty() && EndSkeletalMesh - i >= 5)
        {
            TArray<FString> OutArray;
            String = String.Replace(TEXT("SkeletalMesh "), TEXT(""));
            String.ParseIntoArray(OutArray, TEXT(" "), true);
            if (OutArray.Num() == 9)
            {
                FObjClass Obj;
                Obj.Object = OutArray[0];
                Obj.NumKB = OutArray[1];
                Obj.MaxKB = OutArray[2];
                Obj.ResExcKB = OutArray[3];
                Obj.ResExcDedSysKB = OutArray[4];
                Obj.ResExcShrSysKB = OutArray[5];
                Obj.ResExcDedVidKB = OutArray[6];
                Obj.ResExcShrVidKB = OutArray[7];
                Obj.ResExcUnkKB = OutArray[8];
                SkeletalMeshObjects.Add(Obj);
            }
        }
        else if (String.Contains(TEXT("Total: ")))
        {
            SkeletalMeshObjectsStat = ObjectsStatParser(String);
            SkeletalMeshObjectsStat.Print();
        }
        else if (!String.IsEmpty() && EndSkeletalMesh - i < 5)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("SkeletalMesh Objects Num: %d"), SkeletalMeshObjects.Num());
    return SkeletalMeshObjects;
}

TArray<FObjClass> UMemreportParserManager::StaticMeshParser(const TArray<FString>& StringArray, const int& StartStaticMesh, const int& EndStaticMesh)
{
    TArray<FObjClass> StaticMeshList;
    for (int i = StartStaticMesh; i <= EndStaticMesh; i++)
    {
        FString String = StringArray[i];
        if (!String.IsEmpty() && EndStaticMesh - i >= 5)
        {
            TArray<FString> OutArray;
            String = String.Replace(TEXT("StaticMesh "), TEXT(""));
            String.ParseIntoArray(OutArray, TEXT(" "), true);
            if (OutArray.Num() == 9)
            {
                FObjClass Obj;
                Obj.Object = OutArray[0];
                Obj.NumKB = OutArray[1];
                Obj.MaxKB = OutArray[2];
                Obj.ResExcKB = OutArray[3];
                Obj.ResExcDedSysKB = OutArray[4];
                Obj.ResExcShrSysKB = OutArray[5];
                Obj.ResExcDedVidKB = OutArray[6];
                Obj.ResExcShrVidKB = OutArray[7];
                Obj.ResExcUnkKB = OutArray[8];
                StaticMeshList.Add(Obj);
            }
        }
        else if (String.Contains(TEXT("Total: ")))
        {
            StaticMeshObjectsStat = ObjectsStatParser(String);
            StaticMeshObjectsStat.Print();
        }
        else if (!String.IsEmpty() && EndStaticMesh - i < 5)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("StaticMesh Objects Num: %d"), StaticMeshList.Num());
    return StaticMeshList;
}

TArray<FObjClass> UMemreportParserManager::LevelObjectParser(const TArray<FString>& StringArray, const int& StartLevel, const int& EndLevel)
{
    TArray<FObjClass> LevelList;
    for (int i = StartLevel; i <= EndLevel; i++)
    {
        FString String = StringArray[i];
        if (!String.IsEmpty() && EndLevel - i >= 5)
        {
            TArray<FString> OutArray;
            String = String.Replace(TEXT("Level "), TEXT(""));
            String.ParseIntoArray(OutArray, TEXT(" "), true);
            if (OutArray.Num() == 9)
            {
                FObjClass Obj;
                Obj.Object = OutArray[0];
                Obj.NumKB = OutArray[1];
                Obj.MaxKB = OutArray[2];
                Obj.ResExcKB = OutArray[3];
                Obj.ResExcDedSysKB = OutArray[4];
                Obj.ResExcShrSysKB = OutArray[5];
                Obj.ResExcDedVidKB = OutArray[6];
                Obj.ResExcShrVidKB = OutArray[7];
                Obj.ResExcUnkKB = OutArray[8];
                LevelList.Add(Obj);
            }
        }
        else if (String.Contains(TEXT("Total: ")))
        {
            LevelObjectsStat = ObjectsStatParser(String);
            LevelObjectsStat.Print();
        }
        else if (!String.IsEmpty() && EndLevel - i < 5)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("Level Objects Num: %d"), LevelList.Num());
    return LevelList;
}

TArray<FObjClass> UMemreportParserManager::StaticMeshComponentParser(const TArray<FString>& StringArray, const int& StartStaticMeshComponent, const int& EndStaticMeshComponent)
{
    TArray<FObjClass> StaticMeshComponentList;
    for (int i = StartStaticMeshComponent; i <= EndStaticMeshComponent; i++)
    {
        FString String = StringArray[i];
        if (!String.IsEmpty() && EndStaticMeshComponent - i >= 5)
        {
            TArray<FString> OutArray;
            String = String.Replace(TEXT("StaticMeshComponent "), TEXT(""));
            String.ParseIntoArray(OutArray, TEXT(" "), true);
            if (OutArray.Num() == 9)
            {
                FObjClass Obj;
                Obj.Object = OutArray[0];
                Obj.NumKB = OutArray[1];
                Obj.MaxKB = OutArray[2];
                Obj.ResExcKB = OutArray[3];
                Obj.ResExcDedSysKB = OutArray[4];
                Obj.ResExcShrSysKB = OutArray[5];
                Obj.ResExcDedVidKB = OutArray[6];
                Obj.ResExcShrVidKB = OutArray[7];
                Obj.ResExcUnkKB = OutArray[8];
                StaticMeshComponentList.Add(Obj);
            }
        }
        else if (String.Contains(TEXT("Total: ")))
        {
            StaticMeshComponentObjectsStat = ObjectsStatParser(String);
            StaticMeshComponentObjectsStat.Print();
        }
        else if (!String.IsEmpty() && EndStaticMeshComponent - i < 5)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("StaticMeshComponent Objects Num: %d"), StaticMeshComponentList.Num());
    return StaticMeshComponentList;
}

TArray<FSpawnedActor> UMemreportParserManager::SpawnedActorsParser(const TArray<FString>& StringArray, const int& StartActors, const int& EndActors)
{
    bool bStart = false;
    TArray<FSpawnedActor> SpawnedActors;
    for (int i = StartActors; i <= EndActors; i++)
    {
        const FString String = StringArray[i];
        if (String.Contains(TEXT("TimeUnseen")))
        {
            bStart = true;
            continue;
        }
        if (bStart && !String.IsEmpty())
        {
            TArray<FString> OutArray;
            String.ParseIntoArray(OutArray, TEXT(","), true);
            FSpawnedActor SpawnedActor;
            SpawnedActor.TimeUnseen = OutArray[0];
            SpawnedActor.TimeAlive = OutArray[1];
            SpawnedActor.Distance = OutArray[2];
            SpawnedActor.Class = OutArray[3];
            SpawnedActor.Name = OutArray[4];
            SpawnedActor.Owner = OutArray[5];
            SpawnedActors.Add(SpawnedActor);
        }
    }
    return SpawnedActors;
}

TArray<FParticleDynamicMemory> UMemreportParserManager::ParticleDynamicMemoryParser(const TArray<FString>& StringArray, const int& StartParticleDynamicMemory, const int& EndParticleDynamicMemory)
{
    TArray<FParticleDynamicMemory> ParticleDynamicMemories;
    bool bStat = false;
    for (int i = StartParticleDynamicMemory; i <= EndParticleDynamicMemory; i++)
    {
        const FString String = StringArray[i];
        if (String.Contains("ParticleData,Total(Bytes),FMath::Max(Bytes)"))
        {
            bStat = true;
        }
        if (!bStat && !String.IsEmpty())
        {
            TArray<FString> OutArray;
            String.ParseIntoArray(OutArray, TEXT(","), true);
            FParticleDynamicMemory ParticleDynamicMemory;
            ParticleDynamicMemory.Type = OutArray[0];
            ParticleDynamicMemory.Count = OutArray[1];
            ParticleDynamicMemory.MaxCount = OutArray[2];
            ParticleDynamicMemory.Mem = OutArray[3];
            ParticleDynamicMemory.MaxMem = OutArray[4];
            ParticleDynamicMemory.GTMem = OutArray[5];
            ParticleDynamicMemory.GTMemMax = OutArray[6];
            ParticleDynamicMemories.Add(ParticleDynamicMemory);
        }
        else if (bStat)
        {
        }
    }
    return ParticleDynamicMemories;
}

TArray<FConfigCache> UMemreportParserManager::ConfigCacheMemoryParser(const TArray<FString>& StringArray, const int& StartConfigCache, const int& EndConfigCache)
{
    TArray<FConfigCache> ConfigCaches;
    bool bStart = false;
    for (int i = StartConfigCache; i <= EndConfigCache; i++)
    {
        const FString String = StringArray[i];
        if (String.Contains(TEXT("FileName")))
        {
            bStart = true;
            continue;
        }
        if (String.IsEmpty())
        {
            bStart = false;
        }
        if (bStart && !String.IsEmpty())
        {
            TArray<FString> OutArray;
            String.ParseIntoArray(OutArray, TEXT(" "), true);
            FConfigCache ConfigCache;
            ConfigCache.FileName = OutArray[0];
            ConfigCache.NumBytes = OutArray[1];
            ConfigCache.MaxBytes = OutArray[2];
            ConfigCaches.Add(ConfigCache);
        }
    }
    return ConfigCaches;
}

TArray<FTextureMemory> UMemreportParserManager::TextureMemoryParser(const TArray<FString>& StringArray, const int& StartTextures, const int& EndTextures)
{
    TArray<FTextureMemory> TextureMemories;
    bool bStart = false;
    for (int i = StartTextures; i <= EndTextures; i++)
    {
        const FString String = StringArray[i];
        if (String.Contains(TEXT("Cooked/OnDisk:")))
        {
            bStart = true;
            continue;
        }
        if (String.Contains(TEXT("Total size: InMem")))
        {
            bStart = false;
        }
        if (bStart && !String.IsEmpty())
        {
            // TArray<FString> OutArray;
            // String.ParseIntoArray(OutArray, TEXT(" "), true);
            FTextureMemory TextureMemory = TextureMemoryRowSplit(String);

            TextureMemories.Add(TextureMemory);
        }
    }
    return TextureMemories;
}

FTextureMemory UMemreportParserManager::TextureMemoryRowSplit(const FString& String)
{
    FTextureMemory TextureMemory;
    TArray<FString> OutStrings;
    FString TempString;
    int StartIndex = 0;
    // Cooked/OnDisk: Width x Height (Size in KB, Authored Bias)
    int LeftBrackets = String.Find(TEXT("("), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
    int RightBrackets = String.Find(TEXT(")"), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
    for (int i = LeftBrackets + 1; i < RightBrackets; i++)
    {
        TempString += String[i];
    }
    TempString = TempString.Replace(TEXT("KB"), TEXT(""));
    TempString.TrimStartAndEndInline();
    TempString.ParseIntoArray(OutStrings, TEXT(","), true);
    if (OutStrings.Num() == 2)
    {
        TextureMemory.CookedSize = OutStrings[0];
        TextureMemory.AuthoredBias = OutStrings[1];
    }
    OutStrings.Empty();
    TempString.Empty();
    for (int i = 0; i < LeftBrackets; i++)
    {
        TempString += String[i];
    }
    TextureMemory.CookedWidth = TempString;

    // Current/InMem: Width x Height (Size in KB)
    StartIndex += RightBrackets;
    StartIndex++;
    LeftBrackets = String.Find(TEXT("("), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
    RightBrackets = String.Find(TEXT(")"), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
    for (int i = LeftBrackets + 1; i < RightBrackets; i++)
    {
        TempString += String[i];
    }
    TempString = TempString.Replace(TEXT(","), TEXT(""));
    TempString = TempString.Replace(TEXT("KB"), TEXT(""));
    TempString.ParseIntoArray(OutStrings, TEXT(" "), true);
    if (OutStrings.Num() == 2)
    {
        TextureMemory.InMemWidth = OutStrings[0];
        TextureMemory.InMemSize = OutStrings[1];
    }

    StartIndex = RightBrackets + 1;
    TempString.Empty();
    OutStrings.Empty();
    for (int i = StartIndex; i < String.Len(); i++)
    {
        TempString += String[i];
    }
    TempString.ParseIntoArray(OutStrings, TEXT(","), true);
    if (OutStrings.Num() == 5)
    {
        TextureMemory.Format = OutStrings[0];
        TextureMemory.LODGroup = OutStrings[1];
        // OutStrings[2]去掉所有空格
        TextureMemory.Name = OutStrings[2].Replace(TEXT(" "), TEXT(""));
        TextureMemory.Streaming = OutStrings[3];
        TextureMemory.UsageCount = OutStrings[4];
    }

    return TextureMemory;
}

FTextureTotalStat UMemreportParserManager::TextureTotalStatParser(const TArray<FString>& StringArray, const int& StartTextureTotal, const int& EndTextureTotal)
{
    FTextureTotalStat TextureTotalStat;
    for (int i = StartTextureTotal; i < EndTextureTotal; i++)
    {
        FString String = StringArray[i];
        String = String.Replace(TEXT("MB"), TEXT(""));
        String = String.Replace(TEXT(" "), TEXT(""));
        String.TrimStartAndEndInline();
        FRegexPattern RegexPattern(TEXT("[A-Za-z]+=[0-9]+.[0-9]+"));

        TArray<FString> Results;
        TArray<FString> OutStrings;

        if (i == StartTextureTotal)
        {
            FRegexMatcher RegexMatcher(RegexPattern, String);
            while (RegexMatcher.FindNext())
            {
                Results.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Results.Num() == 2)
            {
                RegexPattern = FRegexPattern(TEXT("[A-Za-z]+=[0-9]+"));
                FRegexMatcher Matcher(RegexPattern, String);
                int skip = 0;

                while (Matcher.FindNext())
                {
                    if (skip>=2)
                    {
                        Results.Add(Matcher.GetCaptureGroup(0));
                    }
                    skip++;
                }
            }

            if (Results.Num() == 4)
            {
                Results[0].ParseIntoArray(OutStrings, TEXT("="), true);
                TextureTotalStat.TotalSizeInMem = OutStrings[1];
                OutStrings.Empty();
                Results[1].ParseIntoArray(OutStrings, TEXT("="), true);
                TextureTotalStat.TotalSizeOnDisk = OutStrings[1];
                OutStrings.Empty();
                Results[2].ParseIntoArray(OutStrings, TEXT("="), true);
                TextureTotalStat.Count = OutStrings[1];
                OutStrings.Empty();
                Results[3].ParseIntoArray(OutStrings, TEXT("="), true);
                TextureTotalStat.CountApplicableToMin = OutStrings[1];
                OutStrings.Empty();
                Results.Empty();
            }
            else if (Results.Num() == 3)
            {
                Results[0].ParseIntoArray(OutStrings, TEXT("="), true);
                TextureTotalStat.TotalSizeInMem = OutStrings[1];
                OutStrings.Empty();
                Results[1].ParseIntoArray(OutStrings, TEXT("="), true);
                TextureTotalStat.TotalSizeOnDisk = OutStrings[1];
                OutStrings.Empty();
                Results[2].ParseIntoArray(OutStrings, TEXT("="), true);
                TextureTotalStat.Count = OutStrings[1];
                OutStrings.Empty();
            }
            else
            {
                UE_LOG(LogMemreportParser, Error, TEXT("TextureTotalStatParser Failed!"));
            }
        }
        else
        {
            FTextureGroupStat TextureGroupStat;
            String = String.Replace(TEXT("Total"), TEXT(""));
            String = String.Replace(TEXT("size"), TEXT(""));
            String.ParseIntoArray(OutStrings, TEXT(":"), true);
            TextureGroupStat.Name = OutStrings[0];
            FRegexMatcher RegexMatcher(RegexPattern, OutStrings[1]);
            TArray<FString> TempStrings;
            while (RegexMatcher.FindNext())
            {
                Results.Add(RegexMatcher.GetCaptureGroup(0));
            }
            Results[0].ParseIntoArray(TempStrings, TEXT("="));
            TextureGroupStat.InMem = TempStrings[1];
            TempStrings.Empty();
            Results[1].ParseIntoArray(TempStrings, TEXT("="));
            TextureGroupStat.OnDisk = TempStrings[1];
            TextureTotalStat.TextureGroupStats.Add(TextureGroupStat);
            TempStrings.Empty();
        }
    }
    return TextureTotalStat;
}

void UMemreportParserManager::ParticleSystemsParser(const TArray<FString>& StringArray, const int& StartParticleSystems, const int& EndParticleSystems, FMemreportFile& MemreportFile)
{
    for (int i = StartParticleSystems; i <= EndParticleSystems; i++)
    {
        FString String = StringArray[i];
        TArray<FString> Outs;
        String.ParseIntoArray(Outs, TEXT(","), false);
        if (Outs.Num() == 8)
        {
            if (i != EndParticleSystems)
            {
                FParticleSystem ParticleSystem;
                ParticleSystem.Size = Outs[0];
                ParticleSystem.Name = Outs[1];
                ParticleSystem.PSysSize = Outs[2];
                ParticleSystem.ModuleSize = Outs[3];
                ParticleSystem.ComponentSize = Outs[4];
                ParticleSystem.ComponentCount = Outs[5];
                ParticleSystem.CompResSize = Outs[6];
                ParticleSystem.CompTrueResSize = Outs[7];
                MemreportFile.ParticleSystems.Add(ParticleSystem);
            }
            else
            {
                FParticleSystemsTotal ParticleSystemsTotal;
                ParticleSystemsTotal.Size = Outs[0];
                ParticleSystemsTotal.Name = Outs[1];
                ParticleSystemsTotal.PSysSize = Outs[2];
                ParticleSystemsTotal.ModuleSize = Outs[3];
                ParticleSystemsTotal.ComponentSize = Outs[4];
                ParticleSystemsTotal.ComponentCount = Outs[5];
                ParticleSystemsTotal.ComponentResourceSize = Outs[6];
                ParticleSystemsTotal.ComponentTrueResourceSize = Outs[7];
                MemreportFile.ParticleSystemsTotal = ParticleSystemsTotal;
            }
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("MemreportFile.ParticleSystems Parsed"));
    UE_LOG(LogMemreportParser, Display, TEXT("MemreportFile.ParticleSystemsTotal Parsed"));
}

FObjectsStat UMemreportParserManager::ObjectsStatParser(const FString& String)
{
    FObjectsStat ObjectsStat;
    const FRegexPattern RegexPattern(FString(TEXT("[A-Za-z0-9.: ]+")));
    FRegexMatcher RegexMatcher(RegexPattern, String);
    TArray<FString> Results;
    while (RegexMatcher.FindNext())
    {
        Results.Add(RegexMatcher.GetCaptureGroup(0));
    }
    if (Results.Num() == 9)
    {
        TArray<FString> OutStrings;
        Results[0].ParseIntoArray(OutStrings, TEXT(" "));
        ObjectsStat.Num = OutStrings[0];
        OutStrings.Empty();
        for (int i = 1; i < 9; i++)
        {
            FString Temp = Results[i];
            Temp.ParseIntoArray(OutStrings, TEXT(":"), true);
            OutStrings[0].TrimStartAndEndInline();
            OutStrings[1].TrimStartAndEndInline();
            if (OutStrings[0] == TEXT("Total"))
            {
                ObjectsStat.Total = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }
            else if (OutStrings[0] == TEXT("Max"))
            {
                ObjectsStat.Max = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }
            else if (OutStrings[0] == TEXT("Res"))
            {
                ObjectsStat.Res = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }
            else if (OutStrings[0] == TEXT("ResDedSys"))
            {
                ObjectsStat.ResDedSys = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }
            else if (OutStrings[0] == TEXT("ResShrSys"))
            {
                ObjectsStat.ResShrSys = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }
            else if (OutStrings[0] == TEXT("ResDedVid"))
            {
                ObjectsStat.ResDedVid = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }
            else if (OutStrings[0] == TEXT("ResShrVid"))
            {
                ObjectsStat.ResShrVid = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }
            else if (OutStrings[0] == TEXT("ResUnknown"))
            {
                ObjectsStat.ResUnknown = OutStrings[1].Replace(TEXT("M"), TEXT(""));
            }

            OutStrings.Empty();
        }
    }
    return ObjectsStat;
}

FString UMemreportParserManager::TransFStringArrayToFString(const TArray<FString>& FStringArray)
{
    if (FStringArray.Num() == 0)
    {
        return FString();
    }
    FString OutString;
    int i = 0;
    for (auto& String : FStringArray)
    {
        OutString += String;
        if (i < FStringArray.Num() - 1)
        {
            OutString += ',';
        }
        i++;
    }
    return OutString;
}

FString UMemreportParserManager::GetCSVFileName(const FString& OriFileName, ECSVFileType CSVFileType)
{
    FString FileName = FileFolder / TEXT("CSVs") / OriFileName;

    switch (CSVFileType)
    {
    case Obj:
        FileName += TEXT("-Obj");
        break;
    case SpawnedActors:
        FileName += TEXT("-SpawnedActors");
        break;
    case ConfigCache:
        FileName += TEXT("-ConfigCache");
        break;
    case Texture:
        FileName += TEXT("-Texture");
        break;
    default:
        break;
    }
    
    FileName += TEXT(".csv");
    return FileName;
}

bool UMemreportParserManager::CheckCurrentFile()
{
    if (FilesData.Num() <= 0 || !FilesData.Contains(CurrentFileName))
    {
        if (FilesData.Num() <= 0)
        {
            UE_LOG(LogMemreportParser, Error, TEXT("FilesData.Num() <= 0"));
            return false;
        }
        if (!FilesData.Contains(CurrentFileName))
        {
            UE_LOG(LogMemreportParser, Error, TEXT("FilesData Not find : %s"), *CurrentFileName);
            for (TTuple<FString, FMemreportFile> Data : FilesData)
            {
                UE_LOG(LogMemreportParser, Error, TEXT("FilesData : %s"), *Data.Key);
            }
            return false;
        }
    }
    return true;
}

void UMemreportParserManager::SaveObjListToCSV()
{
    if (!CheckCurrentFile())
        return;

    TArray<FObj> ObjectList = FilesData[CurrentFileName].ObjectList;
    const FString FileName = GetCSVFileName(FileNames[0], Obj);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FObj::GetHeader()));
    for (auto& Obj : ObjectList)
    {
        OutStrings.Add(TransFStringArrayToFString(Obj.GetDataArray()));
    }

    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save ObjList to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save ObjList to CSV Failed"));
    }
}

void UMemreportParserManager::SaveSpawnedActorsToCSV()
{
    if (!CheckCurrentFile())
        return;

    TArray<FSpawnedActor> SpawnedActors = FilesData[CurrentFileName].SpawnedActors;
    const FString FileName = GetCSVFileName(FileNames[0], ECSVFileType::SpawnedActors);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FSpawnedActor::GetHeader()));
    for (auto& SpawnedActor : SpawnedActors)
    {
        OutStrings.Add(TransFStringArrayToFString(SpawnedActor.GetDataArray()));
    }
    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save SpawnedActors to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save SpawnedActors to CSV Failed"));
    }
}

void UMemreportParserManager::SaveConfigCacheMemoryToCSV()
{
    if (!CheckCurrentFile())
        return;
    
    TArray<FConfigCache> ConfigCaches = FilesData[CurrentFileName].ConfigCaches;
    const FString FileName = GetCSVFileName(FileNames[0], ConfigCache);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FConfigCache::GetHeader()));
    for (auto& ConfigCache : ConfigCaches)
    {
        OutStrings.Add(TransFStringArrayToFString(ConfigCache.GetDataArray()));
    }
    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save ConfigCache to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save ConfigCache to CSV Failed"));
    }
}

void UMemreportParserManager::SaveTexturesToCSV()
{
    if (!CheckCurrentFile())
        return;
    
    TArray<FTextureMemory> Textures = FilesData[CurrentFileName].TextureMemories;
    const FString FileName = GetCSVFileName(FileNames[0], Texture);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FTextureMemory::GetHeader()));
    for (auto& Texture : Textures)
    {
        OutStrings.Add(TransFStringArrayToFString(Texture.GetDataArray()));
    }
    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save Texture to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save Texture to CSV Failed"));
    }
}

#undef LOCTEXT_NAMESPACE
