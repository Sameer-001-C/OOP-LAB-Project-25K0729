#include "CLI.h"
#include <iostream>
#include <stdexcept>

CLI::CLI() : repo(nullptr), running(false) {}

CLI::~CLI() {
    freeOperations();
    delete repo;
}

void CLI::freeOperations() {
    for (VCOperation* op : operations)
        delete op;
    operations.clear();
}

// Register all operations — order defines menu numbering
void CLI::registerOperations() {
    operations.push_back(new StatusOperation());
    operations.push_back(new AddFileOperation());
    operations.push_back(new DeleteFileOperation());
    operations.push_back(new CommitOperation());
    operations.push_back(new LogOperation());
    operations.push_back(new ShowCommitDetailOperation());
    operations.push_back(new RevertOperation());
    operations.push_back(new SearchOperation());
}

void CLI::displayBanner() const {
    std::cout << "\n";
    std::cout << "  +======================================+" << std::endl;
    std::cout << "  |   Mini Git - Version Control System  |" << std::endl;
    std::cout << "  |   OOP Lab Semester Project  (C++)    |" << std::endl;
    std::cout << "  +======================================+" << std::endl;
    std::cout << "\n";
}

void CLI::displayMenu() const {
    std::cout << "\n  +---[ MENU ]----------------------------+" << std::endl;
    for (int i = 0; i < (int)operations.size(); ++i) {
        std::cout << "  |  " << (i + 1) << ". "
                  << operations[i]->getOperationName() << std::endl;
    }
    std::cout << "  |  " << (operations.size() + 1) << ". Exit" << std::endl;
    std::cout << "  +---------------------------------------+" << std::endl;
}

std::string CLI::promptInput(const std::string& prompt) const {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

void CLI::initialize() {
    displayBanner();

    std::string name = promptInput("  Enter repository name: ");
    if (name.empty()) name = "MyProject";

    // Validate: name must not contain spaces
    for (char c : name) {
        if (c == ' ') {
            std::cout << "  [WARN] Repository name may not contain spaces. "
                         "Using 'MyProject'." << std::endl;
            name = "MyProject";
            break;
        }
    }

    try {
        repo = new Repository(name);
        registerOperations();
        running = true;
    } catch (const std::exception& e) {
        std::cout << "\n  [FATAL] Could not initialise repository: "
                  << e.what() << std::endl;
        running = false;
    }
}

void CLI::run() {
    if (!running || !repo) return;

    while (running) {
        displayMenu();
        std::string input = promptInput("\n  Enter choice: ");

        if (input.empty()) continue;

        int choice = -1;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "  [ERROR] Invalid input — enter a number." << std::endl;
            continue;
        }

        int exitChoice = (int)operations.size() + 1;

        if (choice == exitChoice) {
            std::cout << "\n  Data saved. Goodbye!\n" << std::endl;
            running = false;

        } else if (choice >= 1 && choice <= (int)operations.size()) {
            try {
                // Polymorphic dispatch: correct derived execute() is called
                operations[choice - 1]->execute(*repo);
            } catch (const std::exception& e) {
                std::cout << "  [ERROR] " << e.what() << std::endl;
            }

        } else {
            std::cout << "  [ERROR] Please enter a number between 1 and "
                      << exitChoice << "." << std::endl;
        }
    }
}
