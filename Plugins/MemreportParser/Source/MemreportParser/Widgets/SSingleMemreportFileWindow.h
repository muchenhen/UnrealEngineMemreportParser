#pragma once
#include "MemreportParser/Widgets/TextureTab/STexturesTab.h"
#include "MemreportParser/Public/MemreportStructs.h"

typedef TSharedPtr<FTextureMemory> FTextureMemoryPtr;

class SSingleMemreportFileWindow : public SWindow
{
public:
    SLATE_BEGIN_ARGS(SWindow)
    {}

    SLATE_END_ARGS()

    virtual ~SSingleMemreportFileWindow() override;

public:

    FMemreportFile MemreportFile;

public:
    
    /**
     * @brief Texture相关内存的Tab页
     */
    TSharedPtr<STexturesTab> TextureTab = MakeShared<STexturesTab>();

public:

    TSharedRef<SDockTab> MakeTextureTab(const FSpawnTabArgs& SpawnTabArgs);

    
public:

    void SetStats(FMemreportFile InMemreportFile);
    
    void Open();

    void Refresh() const;
};
