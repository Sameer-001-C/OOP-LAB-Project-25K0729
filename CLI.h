#pragma once
#include "Repository.h"
#include "VCOperation.h"
#include <vector>
#include <string>

// ============================================================
//  CLI  (Command Line Interface)
//  Owns a Repository and a polymorphic vector of VCOperation*.
//  Dispatches user input to the correct derived execute() method,
//  demonstrating polymorphism in action.
// ============================================================
class CLI {
private:
    Repository*              repo;
    std::vector<VCOperation*> operations;  // heap-allocated; deleted in destructor
    bool                     running;

    void displayBanner() const;
    void displayMenu()   const;
    void registerOperations();
    void freeOperations();

    std::string promptInput(const std::string& prompt) const;

public:
    CLI();
    ~CLI();

    // Non-copyable (owns raw pointers)
    CLI(const CLI&)            = delete;
    CLI& operator=(const CLI&) = delete;

    void initialize();  // prompt for repo name, set everything up
    void run();         // main event loop
};
