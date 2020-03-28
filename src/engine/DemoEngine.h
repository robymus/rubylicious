#ifndef RUBYLICIOUS_DEMOENGINE_H
#define RUBYLICIOUS_DEMOENGINE_H

#include "SDL.h"

/**
 * The main methods of the demo engine
 * Additional functionality is available in different classes
 *
 * Wherever lowlevel C API is wrapped, related C datatypes are used (eg. char *), to avoid wrapping/unwrapping objects
 */
class DemoEngine {
public:

    constexpr static int OPENGL_MAJOR_VERSION = 4;
    constexpr static int OPENGL_MINOR_VERSION = 6;

    // setup / teardown related stuff

    /* initialize everything, returns non-null error message in case of error */
    const char *initialize();

    /* shut down everything */
    void shutdown();

    /*
     * Create main openGL window, returns non-null error message in case of error
     * It creates OpenGL context and initializes glew with extensions
     */
    const char *createWindow(const char *title, int width, int height, bool fullScreen = false, int display = 0);

    /* Close OpenGL context and window */
    void closeWindow();

    // window size, after createWindow
    int width = 0;
    int height = 0;

private:
    bool sdlInitialized = false;
    bool windowReady = false;
    SDL_Window *window;
    SDL_GLContext context;
};


#endif //RUBYLICIOUS_DEMOENGINE_H
