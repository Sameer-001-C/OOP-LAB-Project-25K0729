#include "CLI.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        CLI cli;
        cli.initialize();
        cli.run();
    } catch (const std::exception& e) {
        std::cerr << "\n[FATAL] Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
