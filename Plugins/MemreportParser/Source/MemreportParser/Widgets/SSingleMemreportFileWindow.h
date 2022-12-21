#pragma once
#include "ExportTab/SExportTab.h"
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

    /**
     * @brief 一些导出按钮的Tab页
     */
    TSharedPtr<SExportTab> ExportTab = MakeShared<SExportTab>();

public:

    TSharedRef<SDockTab> MakeTextureTab(const FSpawnTabArgs& SpawnTabArgs);

    TSharedRef<SDockTab> MakeExportTab(const FSpawnTabArgs& SpawnTabArgs);
    
public:

    void SetStats(FMemreportFile InMemreportFile);
    
    void Open();

    void Refresh() const;
};
