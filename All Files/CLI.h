#pragma once
#include "Repository.h"
#include "VCOperation.h"
#include <vector>
#include <string>

class CLI {
private:
    Repository*               repo;
    std::vector<VCOperation*> operations;
    bool                      running;

    void displayBanner() const;
    void displayMenu()   const;
    void registerOperations();
    void freeOperations();

    std::string promptInput(const std::string& prompt) const;

public:
    CLI();
    ~CLI();

    CLI(const CLI&)            = delete;
    CLI& operator=(const CLI&) = delete;

    void initialize();
    void run();
};
