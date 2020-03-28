/*
 * Demo engine executable entry point
 */

#include "RubyAPI.h"
#include "SDL.h"

using namespace std;

int main(int argc, char *argv[]) {

    Ruby::initialize();

    Ruby::runScript("./rb/main.rb");

    Ruby::shutdown();

    return 0;
}