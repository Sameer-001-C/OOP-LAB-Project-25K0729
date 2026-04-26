#include "Commit.h"
#include <iostream>

Commit::Commit() : CommitMetadata() {}

Commit::Commit(const std::string& message, const std::vector<FileEntry>& files)
    : CommitMetadata(message), files(files) {}

Commit::Commit(const std::string& id, const std::string& msg,
               const std::string& ts, int num,
               const std::vector<FileEntry>& files)
    : CommitMetadata(id, msg, ts, num), files(files) {}

const std::vector<FileEntry>& Commit::getFiles()     const { return files; }
int                           Commit::getFileCount()  const { return (int)files.size(); }
void                          Commit::addFile(const FileEntry& f) { files.push_back(f); }

void Commit::displayDetailed() const {
    std::cout << "  ===========================================" << std::endl;
    display();
    std::cout << "  Files in this snapshot (" << files.size() << "):" << std::endl;
    std::cout << "  -------------------------------------------" << std::endl;
    for (const auto& f : files) {
        f.displayDetailed();
        std::cout << "  -------------------------------------------" << std::endl;
    }
    std::cout << "  ===========================================" << std::endl;
}
