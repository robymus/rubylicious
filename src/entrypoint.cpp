/*
 * Demo engine executable entry point
 * TODO: make it a little more sophisticated, like parse arguments or config file to determine entry point, verbosity, etc.
 */

#include "RubyAPI.h"
#include <iostream>

int main(int argc, char *argv[]) {

    Ruby::initialize();

    const char *errorMessage = Ruby::runScript("./rb/main.rb");

    if (errorMessage != nullptr) {
        std::cerr << errorMessage << std::endl;
    }

    Ruby::shutdown();

    return errorMessage == nullptr ? 0 : 1;
}