#include "FileEntry.h"

FileEntry::FileEntry() : filename(""), content("") {}

FileEntry::FileEntry(const std::string& filename, const std::string& content)
    : filename(filename), content(content) {}

std::string FileEntry::getFilename() const { return filename; }
std::string FileEntry::getContent()  const { return content;  }

void FileEntry::setFilename(const std::string& newFilename) { filename = newFilename; }
void FileEntry::setContent (const std::string& newContent)  { content  = newContent;  }

void FileEntry::display() const {
    std::cout << "    >> " << filename << std::endl;
}

void FileEntry::displayDetailed() const {
    std::cout << "    File    : " << filename << std::endl;
    std::cout << "    Content : " << content  << std::endl;
}
