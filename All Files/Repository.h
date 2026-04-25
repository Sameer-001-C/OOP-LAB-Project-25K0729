#pragma once
#include "Commit.h"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>

// ============================================================
//  Repository
//  Manages the working directory, commit history, file I/O,
//  and all version control operations.
//  Uses dynamic memory (new/delete) for Commit objects.
// ============================================================
class Repository {
private:
    std::string            repoName;
    std::vector<FileEntry> workingDirectory;
    std::vector<Commit*>   commits;          // heap-allocated — deleted in destructor
    std::string            dataFilePath;

    // ---------- Internal helpers ----------
    int  findInWorkDir(const std::string& filename)  const;  // -1 if not found
    bool existsInWorkDir(const std::string& filename) const;

    // ---------- Persistence helpers ----------
    std::string encodeContent(const std::string& content) const;
    std::string decodeContent(const std::string& encoded) const;
    void        saveToFile()  const;
    void        loadFromFile();

public:
    explicit Repository(const std::string& name);
    ~Repository();

    // Non-copyable (owns raw pointers)
    Repository(const Repository&)            = delete;
    Repository& operator=(const Repository&) = delete;

    // ---------- Core CRUD operations ----------
    void addFile   (const std::string& filename, const std::string& content);
    bool deleteFile(const std::string& filename);
    bool updateFile(const std::string& filename, const std::string& content);
    void commit    (const std::string& message);

    // ---------- View operations ----------
    void showLog          ()            const;
    void showStatus       ()            const;
    void showCommitDetail (int number)  const;

    // ---------- Search & revert ----------
    void searchCommits(const std::string& keyword) const;
    bool revert       (int commitNumber);

    // ---------- Getters ----------
    std::string                    getRepoName()         const;
    int                            getCommitCount()      const;
    const std::vector<FileEntry>&  getWorkingDirectory() const;
    const std::vector<Commit*>&    getCommits()          const;
};
