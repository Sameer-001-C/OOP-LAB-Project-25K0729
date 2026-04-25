#include "VCOperation.h"
#include "Repository.h"
#include <stdexcept>
#include <limits>

// -------------------------------------------------------
//  Utility: flush cin and read a trimmed line
// -------------------------------------------------------
static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

// -------------------------------------------------------
//  Base class non-pure virtual
// -------------------------------------------------------
void VCOperation::displayInfo() const {
    std::cout << "  [" << getOperationName() << "] " << getDescription() << std::endl;
}

// ============================================================
//  StatusOperation::execute
// ============================================================
void StatusOperation::execute(Repository& repo) {
    std::cout << "\n--- Working Directory Status ---" << std::endl;
    repo.showStatus();
}

// ============================================================
//  AddFileOperation::execute
// ============================================================
void AddFileOperation::execute(Repository& repo) {
    std::cout << "\n--- Add / Update File ---" << std::endl;

    std::string filename = readLine("  Filename (e.g. main.cpp): ");
    if (filename.empty()) {
        std::cout << "  [ERROR] Filename cannot be empty." << std::endl;
        return;
    }

    // Validate: no spaces in filename
    if (filename.find(' ') != std::string::npos) {
        std::cout << "  [ERROR] Filename must not contain spaces." << std::endl;
        return;
    }

    std::cout << "  Content (single line): ";
    std::string content;
    std::getline(std::cin, content);

    repo.addFile(filename, content);
    std::cout << "  [OK] '" << filename << "' added/updated in working directory." << std::endl;
}

// ============================================================
//  DeleteFileOperation::execute
// ============================================================
void DeleteFileOperation::execute(Repository& repo) {
    std::cout << "\n--- Delete File ---" << std::endl;

    if (repo.getWorkingDirectory().empty()) {
        std::cout << "  [INFO] Working directory is empty. Nothing to delete." << std::endl;
        return;
    }

    repo.showStatus();
    std::string filename = readLine("  Enter filename to delete: ");

    if (filename.empty()) {
        std::cout << "  [ERROR] Filename cannot be empty." << std::endl;
        return;
    }

    if (repo.deleteFile(filename)) {
        std::cout << "  [OK] '" << filename << "' deleted." << std::endl;
    } else {
        std::cout << "  [ERROR] File '" << filename << "' not found in working directory." << std::endl;
    }
}

// ============================================================
//  CommitOperation::execute
// ============================================================
void CommitOperation::execute(Repository& repo) {
    std::cout << "\n--- Commit ---" << std::endl;

    if (repo.getWorkingDirectory().empty()) {
        std::cout << "  [ERROR] Nothing to commit. Add files first." << std::endl;
        return;
    }

    std::string message = readLine("  Commit message: ");
    if (message.empty()) {
        std::cout << "  [ERROR] Commit message cannot be empty." << std::endl;
        return;
    }

    repo.commit(message);
    std::cout << "  [OK] Commit #" << repo.getCommitCount()
              << " created successfully." << std::endl;
}

// ============================================================
//  LogOperation::execute
// ============================================================
void LogOperation::execute(Repository& repo) {
    std::cout << "\n--- Commit Log ---" << std::endl;
    repo.showLog();
}

// ============================================================
//  ShowCommitDetailOperation::execute
// ============================================================
void ShowCommitDetailOperation::execute(Repository& repo) {
    std::cout << "\n--- Commit Detail ---" << std::endl;

    if (repo.getCommits().empty()) {
        std::cout << "  [INFO] No commits yet." << std::endl;
        return;
    }

    repo.showLog();
    std::string input = readLine("  Enter commit number to inspect: ");

    try {
        int num = std::stoi(input);
        repo.showCommitDetail(num);
    } catch (const std::invalid_argument&) {
        std::cout << "  [ERROR] Invalid input. Please enter a number." << std::endl;
    } catch (const std::out_of_range&) {
        std::cout << "  [ERROR] Number out of range." << std::endl;
    }
}

// ============================================================
//  RevertOperation::execute
// ============================================================
void RevertOperation::execute(Repository& repo) {
    std::cout << "\n--- Revert to Commit ---" << std::endl;

    if (repo.getCommits().empty()) {
        std::cout << "  [INFO] No commits to revert to." << std::endl;
        return;
    }

    repo.showLog();
    std::string input = readLine("  Enter commit number to revert to: ");

    try {
        int num = std::stoi(input);
        if (repo.revert(num)) {
            std::cout << "  [OK] Working directory reverted to commit #" << num << "." << std::endl;
        } else {
            std::cout << "  [ERROR] Commit #" << num << " not found." << std::endl;
        }
    } catch (const std::invalid_argument&) {
        std::cout << "  [ERROR] Invalid input. Please enter a number." << std::endl;
    } catch (const std::out_of_range&) {
        std::cout << "  [ERROR] Number out of range." << std::endl;
    }
}

// ============================================================
//  SearchOperation::execute
// ============================================================
void SearchOperation::execute(Repository& repo) {
    std::cout << "\n--- Search Commits ---" << std::endl;

    std::string keyword = readLine("  Enter search keyword: ");
    if (keyword.empty()) {
        std::cout << "  [ERROR] Keyword cannot be empty." << std::endl;
        return;
    }

    repo.searchCommits(keyword);
}
