#pragma once
#include "Commit.h"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>

class Repository {
private:
    std::string            repoName;
    std::vector<FileEntry> workingDirectory;
    std::vector<Commit*>   commits;
    std::string            dataFilePath;

    int  findInWorkDir(const std::string& filename)   const;
    bool existsInWorkDir(const std::string& filename) const;

    std::string encodeContent(const std::string& content) const;
    std::string decodeContent(const std::string& encoded) const;
    void        saveToFile()  const;
    void        loadFromFile();

public:
    explicit Repository(const std::string& name);
    ~Repository();

    Repository(const Repository&)            = delete;
    Repository& operator=(const Repository&) = delete;

    void addFile   (const std::string& filename, const std::string& content);
    bool deleteFile(const std::string& filename);
    bool updateFile(const std::string& filename, const std::string& content);
    void commit    (const std::string& message);

    void showLog         ()           const;
    void showStatus      ()           const;
    void showCommitDetail(int number) const;

    void searchCommits(const std::string& keyword) const;
    bool revert       (int commitNumber);

    std::string                   getRepoName()         const;
    int                           getCommitCount()      const;
    const std::vector<FileEntry>& getWorkingDirectory() const;
    const std::vector<Commit*>&   getCommits()          const;
};
