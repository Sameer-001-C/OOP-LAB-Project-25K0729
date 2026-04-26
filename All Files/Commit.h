#pragma once
#include "CommitMetadata.h"
#include "FileEntry.h"
#include <vector>

class Commit : public CommitMetadata {
private:
    std::vector<FileEntry> files;

public:
    Commit();
    Commit(const std::string& message, const std::vector<FileEntry>& files);
    Commit(const std::string& id, const std::string& msg,
           const std::string& ts, int num,
           const std::vector<FileEntry>& files);

    const std::vector<FileEntry>& getFiles()     const;
    int                           getFileCount() const;
    void                          addFile(const FileEntry& file);

    void displayDetailed() const override;
};
