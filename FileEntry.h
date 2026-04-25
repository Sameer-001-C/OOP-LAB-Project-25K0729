#pragma once
#include <string>
#include <iostream>

// Represents a single file (name + content) stored in working directory or commit snapshot
class FileEntry {
private:
    std::string filename;
    std::string content;

public:
    FileEntry();
    FileEntry(const std::string& filename, const std::string& content);

    // Getters
    std::string getFilename() const;
    std::string getContent()  const;

    // Setters
    void setFilename(const std::string& newFilename);
    void setContent (const std::string& newContent);

    // Display
    void display()         const;   // short  (one-line)
    void displayDetailed() const;   // full   (name + content)
};
