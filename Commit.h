#pragma once
#include "CommitMetadata.h"
#include "FileEntry.h"
#include <vector>

// ============================================================
//  Derived Class: Commit   (inherits CommitMetadata)
//  Adds a snapshot of all FileEntry objects at commit time.
//  Demonstrates: Inheritance + Polymorphism (override displayDetailed)
// ============================================================
class Commit : public CommitMetadata {
private:
    std::vector<FileEntry> files;   // snapshot of working directory

public:
    Commit();
    // Create a brand-new commit (auto id/timestamp via base class)
    Commit(const std::string& message, const std::vector<FileEntry>& files);
    // Restore a commit from file (all metadata supplied explicitly)
    Commit(const std::string& id, const std::string& msg,
           const std::string& ts, int num,
           const std::vector<FileEntry>& files);

    // Accessors
    const std::vector<FileEntry>& getFiles()    const;
    int                           getFileCount() const;
    void                          addFile(const FileEntry& file);

    // Override pure virtual from CommitMetadata (polymorphism)
    void displayDetailed() const override;
};
