#include "Repository.h"
#include <iostream>
#include <sstream>

Repository::Repository(const std::string& name) : repoName(name) {
    dataFilePath = repoName + "_repo.dat";
    try {
        loadFromFile();
        std::cout << "  [INFO] Repository '" << repoName
                  << "' loaded (" << commits.size() << " commits)." << std::endl;
    } catch (const std::exception&) {
        std::cout << "  [INFO] New repository '" << repoName << "' initialised." << std::endl;
    }
}

Repository::~Repository() {
    saveToFile();
    for (Commit* c : commits) {
        delete c;
    }
    commits.clear();
}

int Repository::findInWorkDir(const std::string& filename) const {
    for (int i = 0; i < (int)workingDirectory.size(); ++i) {
        if (workingDirectory[i].getFilename() == filename) return i;
    }
    return -1;
}

bool Repository::existsInWorkDir(const std::string& filename) const {
    return findInWorkDir(filename) != -1;
}

std::string Repository::encodeContent(const std::string& content) const {
    std::string out;
    out.reserve(content.size());
    for (char c : content) {
        if      (c == '\n') out += "<<NL>>";
        else if (c == '\r') out += "<<CR>>";
        else                out += c;
    }
    return out;
}

std::string Repository::decodeContent(const std::string& encoded) const {
    std::string out;
    out.reserve(encoded.size());
    size_t i = 0;
    while (i < encoded.size()) {
        if (i + 6 <= encoded.size() && encoded.substr(i, 6) == "<<NL>>") {
            out += '\n'; i += 6;
        } else if (i + 6 <= encoded.size() && encoded.substr(i, 6) == "<<CR>>") {
            out += '\r'; i += 6;
        } else {
            out += encoded[i++];
        }
    }
    return out;
}

void Repository::saveToFile() const {
    std::ofstream file(dataFilePath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open data file for writing: " + dataFilePath);

    file << "REPO_NAME:"      << repoName                       << "\n";
    file << "COMMIT_COUNTER:" << CommitMetadata::getCounter()   << "\n";
    file << "WORKDIR_COUNT:"  << workingDirectory.size()        << "\n";

    for (const auto& fe : workingDirectory) {
        file << "WFILE_START\n";
        file << "WFILENAME:" << fe.getFilename()                  << "\n";
        file << "WCONTENT:"  << encodeContent(fe.getContent())    << "\n";
        file << "WFILE_END\n";
    }

    file << "COMMIT_COUNT:" << commits.size() << "\n";

    for (const Commit* c : commits) {
        file << "COMMIT_START\n";
        file << "CID:"         << c->getCommitId()                    << "\n";
        file << "CNUM:"        << c->getCommitNumber()                << "\n";
        file << "CTIMESTAMP:"  << c->getTimestamp()                   << "\n";
        file << "CMESSAGE:"    << encodeContent(c->getMessage())      << "\n";
        file << "CFILE_COUNT:" << c->getFileCount()                   << "\n";
        for (const auto& fe : c->getFiles()) {
            file << "CFILE_START\n";
            file << "CFILENAME:" << fe.getFilename()               << "\n";
            file << "CCONTENT:"  << encodeContent(fe.getContent()) << "\n";
            file << "CFILE_END\n";
        }
        file << "COMMIT_END\n";
    }

    file.close();
}

void Repository::loadFromFile() {
    std::ifstream file(dataFilePath);
    if (!file.is_open())
        throw std::runtime_error("Data file not found");

    auto readKey = [](const std::string& line, const std::string& prefix) -> std::string {
        if (line.size() > prefix.size() && line.substr(0, prefix.size()) == prefix)
            return line.substr(prefix.size());
        return "";
    };

    std::string line;

    if (!std::getline(file, line)) throw std::runtime_error("Corrupt data file");
    repoName = readKey(line, "REPO_NAME:");

    if (!std::getline(file, line)) throw std::runtime_error("Corrupt data file");
    {
        std::string val = readKey(line, "COMMIT_COUNTER:");
        if (!val.empty()) CommitMetadata::setCounter(std::stoi(val));
    }

    if (!std::getline(file, line)) throw std::runtime_error("Corrupt data file");
    int wdCount = 0;
    {
        std::string val = readKey(line, "WORKDIR_COUNT:");
        if (!val.empty()) wdCount = std::stoi(val);
    }

    for (int i = 0; i < wdCount; ++i) {
        std::getline(file, line);
        std::string fname, fcontent;
        std::getline(file, line);
        fname    = readKey(line, "WFILENAME:");
        std::getline(file, line);
        fcontent = decodeContent(readKey(line, "WCONTENT:"));
        std::getline(file, line);
        workingDirectory.emplace_back(fname, fcontent);
    }

    if (!std::getline(file, line)) throw std::runtime_error("Corrupt data file");
    int cCount = 0;
    {
        std::string val = readKey(line, "COMMIT_COUNT:");
        if (!val.empty()) cCount = std::stoi(val);
    }

    for (int i = 0; i < cCount; ++i) {
        std::getline(file, line);
        std::string cid, cmsg, cts;
        int cnum = 0, cfcount = 0;

        std::getline(file, line); cid     =              readKey(line, "CID:");
        std::getline(file, line); cnum    = std::stoi(   readKey(line, "CNUM:"));
        std::getline(file, line); cts     =              readKey(line, "CTIMESTAMP:");
        std::getline(file, line); cmsg    = decodeContent(readKey(line, "CMESSAGE:"));
        std::getline(file, line); cfcount = std::stoi(   readKey(line, "CFILE_COUNT:"));

        std::vector<FileEntry> cfiles;
        for (int j = 0; j < cfcount; ++j) {
            std::getline(file, line);
            std::string cfname, cfcontent;
            std::getline(file, line); cfname    =              readKey(line, "CFILENAME:");
            std::getline(file, line); cfcontent = decodeContent(readKey(line, "CCONTENT:"));
            std::getline(file, line);
            cfiles.emplace_back(cfname, cfcontent);
        }

        commits.push_back(new Commit(cid, cmsg, cts, cnum, cfiles));
        std::getline(file, line);
    }

    file.close();
}

void Repository::addFile(const std::string& filename, const std::string& content) {
    if (filename.empty())
        throw std::invalid_argument("Filename cannot be empty");

    int idx = findInWorkDir(filename);
    if (idx != -1) {
        workingDirectory[idx].setContent(content);
    } else {
        workingDirectory.emplace_back(filename, content);
    }
}

bool Repository::deleteFile(const std::string& filename) {
    int idx = findInWorkDir(filename);
    if (idx == -1) return false;
    workingDirectory.erase(workingDirectory.begin() + idx);
    return true;
}

bool Repository::updateFile(const std::string& filename, const std::string& content) {
    int idx = findInWorkDir(filename);
    if (idx == -1) return false;
    workingDirectory[idx].setContent(content);
    return true;
}

void Repository::commit(const std::string& message) {
    if (message.empty())
        throw std::invalid_argument("Commit message cannot be empty");
    if (workingDirectory.empty())
        throw std::runtime_error("Nothing to commit — working directory is empty");

    commits.push_back(new Commit(message, workingDirectory));
}

void Repository::showLog() const {
    if (commits.empty()) {
        std::cout << "  No commits yet." << std::endl;
        return;
    }
    std::cout << "  Total commits: " << commits.size() << std::endl;
    std::cout << "  ----------------------------------------" << std::endl;
    for (const Commit* c : commits) {
        c->display();
        std::cout << "  Files (" << c->getFileCount() << "): ";
        for (const auto& f : c->getFiles())
            std::cout << f.getFilename() << "  ";
        std::cout << "\n  ----------------------------------------" << std::endl;
    }
}

void Repository::showStatus() const {
    std::cout << "  Repository    : " << repoName << std::endl;
    std::cout << "  Commits so far: " << commits.size() << std::endl;
    std::cout << "  Working directory (" << workingDirectory.size() << " file(s)):" << std::endl;
    if (workingDirectory.empty()) {
        std::cout << "    (empty)" << std::endl;
        return;
    }
    for (const auto& f : workingDirectory)
        f.display();
}

void Repository::showCommitDetail(int number) const {
    for (const Commit* c : commits) {
        if (c->getCommitNumber() == number) {
            c->displayDetailed();
            return;
        }
    }
    std::cout << "  [ERROR] Commit #" << number << " not found." << std::endl;
}

void Repository::searchCommits(const std::string& keyword) const {
    std::string lkw = keyword;
    std::transform(lkw.begin(), lkw.end(), lkw.begin(), ::tolower);

    bool found = false;
    std::cout << "  Search results for \"" << keyword << "\":" << std::endl;
    std::cout << "  ----------------------------------------" << std::endl;

    for (const Commit* c : commits) {
        std::string lmsg = c->getMessage();
        std::transform(lmsg.begin(), lmsg.end(), lmsg.begin(), ::tolower);

        bool matchMsg  = lmsg.find(lkw) != std::string::npos;
        bool matchFile = false;

        for (const auto& f : c->getFiles()) {
            std::string lname = f.getFilename();
            std::transform(lname.begin(), lname.end(), lname.begin(), ::tolower);
            if (lname.find(lkw) != std::string::npos) { matchFile = true; break; }
        }

        if (matchMsg || matchFile) {
            found = true;
            c->display();
            if (matchMsg  && !matchFile) std::cout << "  (matched in commit message)" << std::endl;
            if (matchFile && !matchMsg)  std::cout << "  (matched in filename)"        << std::endl;
            if (matchMsg  &&  matchFile) std::cout << "  (matched in message and filename)" << std::endl;
            std::cout << "  ----------------------------------------" << std::endl;
        }
    }

    if (!found)
        std::cout << "  No commits matched \"" << keyword << "\"." << std::endl;
}

bool Repository::revert(int commitNumber) {
    for (const Commit* c : commits) {
        if (c->getCommitNumber() == commitNumber) {
            workingDirectory = c->getFiles();
            return true;
        }
    }
    return false;
}

std::string                   Repository::getRepoName()         const { return repoName;           }
int                           Repository::getCommitCount()      const { return (int)commits.size(); }
const std::vector<FileEntry>& Repository::getWorkingDirectory() const { return workingDirectory;   }
const std::vector<Commit*>&   Repository::getCommits()          const { return commits;             }
