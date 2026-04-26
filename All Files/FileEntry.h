#pragma once
#include <string>
#include <iostream>

class FileEntry {
private:
    std::string filename;
    std::string content;

public:
    FileEntry();
    FileEntry(const std::string& filename, const std::string& content);

    std::string getFilename() const;
    std::string getContent()  const;

    void setFilename(const std::string& newFilename);
    void setContent (const std::string& newContent);

    void display()         const;
    void displayDetailed() const;
};
