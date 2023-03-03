//Copyright MuChenhen All Rights Reserved.

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
                FMemreportFile ParsedFileData = FileParser(FileContent.Value);
                ParsedFileData.StatMemory.FileName = FileName;
                FilesData.Add(FileName) = ParsedFileData;
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
    // int EndSounds = 0;

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
        if (String.Contains(TEXT("CommandLine Options")) && StartStat == 0)
        {
            StartStat = 1;
        }
        else if (String.Contains(TEXT("AssetRegistry memory usage")) && EndStat == 0 && StartStat != 0)
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
        ObjParser(StringArray, StartObj, EndObj, FileData);
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
        SkeletalMeshParser(StringArray, StartSkeletalMesh, EndSkeletalMesh, FileData);
    }

    if (StartStaticMesh != 0 && EndStaticMesh != 0)
    {
        StaticMeshParser(StringArray, StartStaticMesh, EndStaticMesh, FileData);
    }

    if (StartLevel != 0 && EndLevel != 0)
    {
        const TArray<FObjClass> LevelObjects = LevelObjectParser(StringArray, StartLevel, EndLevel);
        FileData.LevelObjects = LevelObjects;
    }

    if (StartStaticMeshComponent != 0 && EndStaticMeshComponent != 0)
    {
        StaticMeshComponentParser(StringArray, StartStaticMeshComponent, EndStaticMeshComponent, FileData);
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
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 2)
            {
                StatMemory.ProcessPhysicalMemoryUsed = Strings[0];
                StatMemory.ProcessPhysicalMemoryPeak = Strings[1];
            }
            continue;
        }

        // Process Virtual Memory
        if (String.Contains("Process Virtual Memory"))
        {
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 2)
            {
                StatMemory.ProcessVirtualMemoryUsed = Strings[0];
                StatMemory.ProcessVirtualMemoryPeak = Strings[1];
            }
            continue;
        }

        // Physical Memory
        if (String.Contains("Physical Memory:"))
        {
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 3)
            {
                StatMemory.PhysicalMemoryUsed = Strings[0];
                StatMemory.PhysicalMemoryFree = Strings[1];
                StatMemory.PhysicalMemoryTotal = Strings[2];
            }
            continue;
        }

        // Virtual Memory
        if (String.Contains("Virtual Memory:"))
        {
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 3)
            {
                StatMemory.VirtualMemoryUsed = Strings[0];
                StatMemory.VirtualMemoryFree = Strings[1];
                StatMemory.VirtualMemoryTotal = Strings[2];
            }
            continue;
        }

        // Constants.BinnedPageSize
        if (String.Contains("Constants.BinnedPageSize"))
        {
            TArray<FString> Output;
            String.ParseIntoArray(Output, TEXT("="), true);
            if (Output.Num() >= 2)
            {
                StatMemory.ConstantsBinnedPageSize = Output[1];
            }
            continue;
        }

        // Constants.BinnedAllocationGranularity
        if (String.Contains("Constants.BinnedAllocationGranularity"))
        {
            TArray<FString> Output;
            String.ParseIntoArray(Output, TEXT("="), true);
            if (Output.Num() >= 2)
            {
                StatMemory.ConstantsBinnedAllocationGranularity = Output[1];
            }
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

        // Requested Allocations
        if (String.Contains("Requested Allocations"))
        {
            const FString SubStr1 = "Allocations: ";
            const FString SubStr2 = "mb";
            StatMemory.RequestedAllocations = GetMB(SubStr1, SubStr2);
            StatMemory.RequestedAllocations = StatMemory.RequestedAllocations.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // OS Allocated
        if (String.Contains("OS Allocated"))
        {
            const FString SubStr1 = "Allocated: ";
            const FString SubStr2 = "mb";
            StatMemory.OSAllocated = GetMB(SubStr1, SubStr2);
            StatMemory.OSAllocated = StatMemory.OSAllocated.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // PoolInfo
        if (String.Contains("PoolInfo"))
        {
            const FString SubStr1 = "PoolInfo: ";
            const FString SubStr2 = "mb";
            StatMemory.PoolInfo = GetMB(SubStr1, SubStr2);
            StatMemory.PoolInfo = StatMemory.PoolInfo.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // Hash
        if (String.Contains("Hash"))
        {
            const FString SubStr1 = "Hash: ";
            const FString SubStr2 = "mb";
            StatMemory.Hash = GetMB(SubStr1, SubStr2);
            StatMemory.Hash = StatMemory.Hash.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // TLS
        if (String.Contains("TLS"))
        {
            const FString SubStr1 = "TLS: ";
            const FString SubStr2 = "mb";
            StatMemory.TLS = GetMB(SubStr1, SubStr2);
            StatMemory.TLS = StatMemory.TLS.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // Total allocated from OS
        if (String.Contains("Total allocated from OS"))
        {
            const FString SubStr1 = "Total allocated from OS: ";
            const FString SubStr2 = "mb";
            StatMemory.TotalAllocatedFromOS = GetMB(SubStr1, SubStr2);
            StatMemory.TotalAllocatedFromOS = StatMemory.TotalAllocatedFromOS.Replace(TEXT("m"), TEXT(""));
            continue;
        }

        // Cached free OS pages
        if (String.Contains("Cached free OS pages"))
        {
            const FString SubStr1 = "Cached free OS pages: ";
            const FString SubStr2 = "mb";
            StatMemory.CachedFreeOSPages = GetMB(SubStr1, SubStr2);
            StatMemory.CachedFreeOSPages = StatMemory.CachedFreeOSPages.Replace(TEXT("m"), TEXT(""));
        }

        // FMemStack (gamethread) current size
        if (String.Contains("FMemStack (gamethread) current size"))
        {
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 1)
            {
                StatMemory.GameThreadCurrentSize = Strings[0];
            }
            continue;
        }

        // FPageAllocator (all threads) allocation size [used/ unused]
        if (String.Contains("FPageAllocator (all threads) allocation size [used/ unused]"))
        {
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 2)
            {
                StatMemory.ThreadPageAllocatorUsed = Strings[0];
                StatMemory.ThreadPageAllocatorUnused = Strings[1];
            }
            continue;
        }

        // Nametable memory usage
        if (String.Contains("Nametable memory usage"))
        {
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 1)
            {
                StatMemory.NametableMemoryUsage = Strings[0];
            }
            continue;
        }

        // AssetRegistry memory usage
        if (String.Contains("AssetRegistry memory usage"))
        {
            const FRegexPattern RegexPattern(FString(TEXT("[0-9.]+")));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            TArray<FString> Strings;
            while (RegexMatcher.FindNext())
            {
                Strings.Add(RegexMatcher.GetCaptureGroup(0));
            }
            if (Strings.Num() >= 1)
            {
                StatMemory.AssetRegistryMemoryUsage = Strings[0];
            }
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
    UE_LOG(LogMemreportParser, Display, TEXT("Levels Count: %d"), Levels.Num());
    return Levels;
}

void UMemreportParserManager::ObjParser(const TArray<FString>& StringArray, const int& StartObj, const int& EndObj, FMemreportFile& FileData)
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
            FileData.ObjectsStat = AlphasortObjectsStat;
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("Objects Count: %d"), ObjectList.Num());
    FileData.ObjectList = ObjectList;
}

void UMemreportParserManager::SkeletalMeshParser(const TArray<FString>& StringArray, const int& StartSkeletalMesh, const int& EndSkeletalMesh, FMemreportFile& FileData)
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
            FileData.SkeletalMeshObjectsStat = SkeletalMeshObjectsStat;
        }
        else if (!String.IsEmpty() && EndSkeletalMesh - i < 5)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("SkeletalMesh Objects Count: %d"), SkeletalMeshObjects.Num());
    FileData.SkeletalMeshObjects = SkeletalMeshObjects;
}

void UMemreportParserManager::StaticMeshParser(const TArray<FString>& StringArray, const int& StartStaticMesh, const int& EndStaticMesh, FMemreportFile& FileData)
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
            FileData.StaticMeshObjectsStat = StaticMeshObjectsStat;
        }
        else if (!String.IsEmpty() && EndStaticMesh - i < 5)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("StaticMesh Objects Count: %d"), StaticMeshList.Num());
    FileData.StaticMeshObjects = StaticMeshList;
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
    UE_LOG(LogMemreportParser, Display, TEXT("Level Objects Count: %d"), LevelList.Num());
    return LevelList;
}

void UMemreportParserManager::StaticMeshComponentParser(const TArray<FString>& StringArray, const int& StartStaticMeshComponent, const int& EndStaticMeshComponent, FMemreportFile& FileData)
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
            FileData.StaticMeshComponentObjectsStat = StaticMeshComponentObjectsStat;
        }
        else if (!String.IsEmpty() && EndStaticMeshComponent - i < 5)
        {
            UE_LOG(LogMemreportParser, Display, TEXT("%s"), *String);
        }
    }
    UE_LOG(LogMemreportParser, Display, TEXT("StaticMeshComponent Objects Count: %d"), StaticMeshComponentList.Num());
    FileData.StaticMeshComponentObjects = StaticMeshComponentList;
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
    /* 处理形如下面的字符串
        Total size: InMem= 99.82 MB  OnDisk= 186.53 MB  Count=1175, CountApplicableToMin=921
        Total PF_Unknown size: InMem= 1.68 MB  OnDisk= 1.68 MB 
        Total PF_B8G8R8A8 size: InMem= 3.49 MB  OnDisk= 5.12 MB 
        Total PF_G8 size: InMem= 5.09 MB  OnDisk= 5.24 MB 
        Total PF_FloatRGBA size: InMem= 0.01 MB  OnDisk= 0.01 MB 
        Total PF_ASTC_4x4 size: InMem= 24.30 MB  OnDisk= 24.30 MB 
        Total PF_ASTC_6x6 size: InMem= 65.26 MB  OnDisk= 150.18 MB 
        Total TEXTUREGROUP_World size: InMem= 19.61 MB  OnDisk= 36.23 MB 
        Total TEXTUREGROUP_WorldNormalMap size: InMem= 5.84 MB  OnDisk= 7.32 MB 
        Total TEXTUREGROUP_WorldSpecular size: InMem= 5.16 MB  OnDisk= 5.87 MB 
        Total TEXTUREGROUP_Character size: InMem= 5.38 MB  OnDisk= 23.74 MB 
        Total TEXTUREGROUP_CharacterNormalMap size: InMem= 2.05 MB  OnDisk= 15.51 MB 
        Total TEXTUREGROUP_CharacterSpecular size: InMem= 3.54 MB  OnDisk= 13.78 MB 
        Total TEXTUREGROUP_Effects size: InMem= 23.67 MB  OnDisk= 49.50 MB 
        Total TEXTUREGROUP_Skybox size: InMem= 2.96 MB  OnDisk= 2.96 MB 
        Total TEXTUREGROUP_UI size: InMem= 24.45 MB  OnDisk= 24.45 MB 
        Total TEXTUREGROUP_Lightmap size: InMem= 2.80 MB  OnDisk= 2.80 MB 
        Total TEXTUREGROUP_Shadowmap size: InMem= 4.34 MB  OnDisk= 4.34 MB 
        Total TEXTUREGROUP_Bokeh size: InMem= 0.02 MB  OnDisk= 0.02 MB 
        Total TEXTUREGROUP_Pixels2D size: InMem= 0.02 MB  OnDisk= 0.02 MB 
     */
    FTextureTotalStat TextureTotalStat;
    for (int i = StartTextureTotal; i < EndTextureTotal; i++)
    {
        FString String = StringArray[i];
        // String为空则跳过
        if (String.IsEmpty())
        {
            continue;
        }
        // 获取第7个字符
        TCHAR Char = String[6];
        // 如果第7个字符是s，则说明是Total size: InMem= 99.82 MB  OnDisk= 186.53 MB  Count=1175, CountApplicableToMin=921
        if (Char == 's')
        {
            // 去掉String中所有的空格
            FString NewString = String.Replace(TEXT(" "), TEXT(""));
            // 再去掉所有的MB
            NewString = NewString.Replace(TEXT("MB"), TEXT(""));
            // 使用正则匹配[\d.]+ 检查是否一共有4个匹配项 InMem OnDisk Count CountApplicableToMin 如果是的话，就把这4个匹配项的值赋值给TextureTotalStat
            FRegexPattern RegexPattern(TEXT("[\\d.]+"));
            FRegexMatcher RegexMatcher(RegexPattern, NewString);
            int MatchCount = 0;
            while (RegexMatcher.FindNext())
            {
                FString MatchString = RegexMatcher.GetCaptureGroup(0);
                if (MatchCount == 0)
                {
                    TextureTotalStat.TotalSizeInMem = MatchString;
                }
                else if (MatchCount == 1)
                {
                    TextureTotalStat.TotalSizeOnDisk = MatchString;
                }
                else if (MatchCount == 2)
                {
                    TextureTotalStat.Count = MatchString;
                }
                else if (MatchCount == 3)
                {
                    TextureTotalStat.CountApplicableToMin = MatchString;
                }
                MatchCount++;
            }
        }

        // 如果第7个字符是P，说明是PF格式行
        else if (Char == 'P')
        {
            // 使用正则PF_\w+|[\d.]+进行匹配
            // 检查是否一共有3个匹配项 PF_XXX InMem OnDisk 如果是的话，就把这3个匹配项的值赋值给TextureTotalStat
            FRegexPattern RegexPattern(TEXT("PF_\\w+|[\\d.]+"));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            int MatchCount = 0;
            FPFFormatStat PFFormatStat;
            while (RegexMatcher.FindNext())
            {
                FString MatchString = RegexMatcher.GetCaptureGroup(0);
                if (MatchCount == 0)
                {
                    PFFormatStat.Name = MatchString;
                }
                else if (MatchCount == 1)
                {
                    PFFormatStat.InMem = MatchString;
                }
                else if (MatchCount == 2)
                {
                    PFFormatStat.OnDisk = MatchString;
                    TextureTotalStat.PFFormatStats.Add(PFFormatStat);
                }
                MatchCount++;
            }
        }

        // 如果第7个字符是T，说明是TEXTUREGROUP格式行
        else if (Char == 'T')
        {
            // 使用正则TEXTUREGROUP_\w+|[\d.]+进行匹配
            // 检查是否一共有3个匹配项 TEXTUREGROUP_XXX InMem OnDisk 如果是的话，就把这3个匹配项的值赋值给TextureTotalStat
            FRegexPattern RegexPattern(TEXT("TEXTUREGROUP_\\w+|[\\d.]+"));
            FRegexMatcher RegexMatcher(RegexPattern, String);
            int MatchCount = 0;
            FTextureGroupStat TextureGroupStat;
            while (RegexMatcher.FindNext())
            {
                FString MatchString = RegexMatcher.GetCaptureGroup(0);
                if (MatchCount == 0)
                {
                    TextureGroupStat.Name = MatchString;
                }
                else if (MatchCount == 1)
                {
                    TextureGroupStat.InMem = MatchString;
                }
                else if (MatchCount == 2)
                {
                    TextureGroupStat.OnDisk = MatchString;
                    TextureTotalStat.TextureGroupStats.Add(TextureGroupStat);
                }
                MatchCount++;
            }
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
        ObjectsStat.Count = OutStrings[0];
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
    case Obj: FileName += TEXT("-Obj");
        break;
    case SpawnedActors: FileName += TEXT("-SpawnedActors");
        break;
    case ConfigCache: FileName += TEXT("-ConfigCache");
        break;
    case Texture: FileName += TEXT("-Texture");
        break;
    case ParticleSystems: FileName += TEXT("-ParticleSystems");
        break;
    case SkeletalMeshes: FileName += TEXT("-SkeletalMeshes");
        break;
    case StaticMeshes: FileName += TEXT("-StaticMeshes");
        break;
    case StaticMeshComponents: FileName += TEXT("-StaticMeshComponents");
        break;
    default: break;
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
            UE_LOG(LogMemreportParser, Error, TEXT("FilesData.Count() <= 0"));
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
    if (!CheckCurrentFile()) return;

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
    if (!CheckCurrentFile()) return;

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
    if (!CheckCurrentFile()) return;

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
    if (!CheckCurrentFile()) return;

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

void UMemreportParserManager::SaveParticleSystemsToCSV()
{
    if (!CheckCurrentFile()) return;

    TArray<FParticleSystem> ParticleSystems = FilesData[CurrentFileName].ParticleSystems;
    const FString FileName = GetCSVFileName(FileNames[0], ECSVFileType::ParticleSystems);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FParticleSystem::GetHeader()));
    for (auto& ParticleSystem : ParticleSystems)
    {
        OutStrings.Add(TransFStringArrayToFString(ParticleSystem.GetDataArray()));
    }
    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save ParticleSystems to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save ParticleSystems to CSV Failed"));
    }
}

void UMemreportParserManager::SaveSkeletalMeshesToCSV()
{
    if (!CheckCurrentFile()) return;

    TArray<FObjClass> SkeletalMeshes = FilesData[CurrentFileName].SkeletalMeshObjects;
    const FString FileName = GetCSVFileName(FileNames[0], ECSVFileType::SkeletalMeshes);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FObjClass::GetHeader()));
    for (auto& SkeletalMesh : SkeletalMeshes)
    {
        OutStrings.Add(TransFStringArrayToFString(SkeletalMesh.GetDataArray()));
    }
    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save SkeletalMeshes to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save SkeletalMeshes to CSV Failed"));
    }
}

void UMemreportParserManager::SaveStaticMeshesToCSV()
{
    if (!CheckCurrentFile()) return;

    TArray<FObjClass> StaticMeshes = FilesData[CurrentFileName].StaticMeshObjects;
    const FString FileName = GetCSVFileName(FileNames[0], ECSVFileType::StaticMeshes);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FObjClass::GetHeader()));
    for (auto& StaticMesh : StaticMeshes)
    {
        OutStrings.Add(TransFStringArrayToFString(StaticMesh.GetDataArray()));
    }
    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save StaticMeshes to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save StaticMeshes to CSV Failed"));
    }
}

void UMemreportParserManager::SaveStaticMeshComponentsToCSV()
{
    if (!CheckCurrentFile()) return;

    TArray<FObjClass> StaticMeshComponents = FilesData[CurrentFileName].StaticMeshComponentObjects;
    const FString FileName = GetCSVFileName(FileNames[0], ECSVFileType::StaticMeshComponents);

    TArray<FString> OutStrings;
    OutStrings.Add(TransFStringArrayToFString(FObjClass::GetHeader()));
    for (auto& StaticMeshComponent : StaticMeshComponents)
    {
        OutStrings.Add(TransFStringArrayToFString(StaticMeshComponent.GetDataArray()));
    }
    const bool bSuccess = FFileHelper::SaveStringArrayToFile(OutStrings, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);
    if (bSuccess)
    {
        UE_LOG(LogMemreportParser, Display, TEXT("Save StaticMeshComponents to CSV Success : %s"), *FileName);
    }
    else
    {
        UE_LOG(LogMemreportParser, Error, TEXT("Save StaticMeshComponents to CSV Failed"));
    }
}

#undef LOCTEXT_NAMESPACE
