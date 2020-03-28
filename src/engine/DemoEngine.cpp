#include "DemoEngine.h"
#include "SDL.h"
#include <GL/glew.h>
#include <string>

/* initialize everything, returns non-null error message in case of error */
const char *DemoEngine::initialize() {
    // set GL version requirement (in case ruby doesn't set it)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        return SDL_GetError();
    }

    return nullptr;
}

/* shut down everything */
void DemoEngine::shutdown() {
    closeWindow();
    SDL_Quit();
}

/*
 * Create main openGL window, returns non-null error message in case of error
 * It creates OpenGL context and initializes glew with extensions
 */
const char *DemoEngine::createWindow(const char *title, int width, int height, bool fullScreen, int display) {
    // create window

    int posX, posY;
    if (fullScreen) posX = posY = SDL_WINDOWPOS_UNDEFINED_DISPLAY(display);
    else posX = posY = SDL_WINDOWPOS_CENTERED_DISPLAY(display);
    // window flags
    Uint32 flags = SDL_WINDOW_OPENGL;
    if (fullScreen) flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(title, posX, posY, width, height, flags);

    // if not successful, return error
    if (!window) return SDL_GetError();

    // create OpenGL context
    context = SDL_GL_CreateContext(window);
    if (!context) return SDL_GetError();

    // initialize OpenGL extensions with glew
    GLenum err = glewInit();
    if (err != GLEW_OK) return "OpenGL Extensions (GLEW) initialization failed";

    // check if required OpenGL version is available
    int glMajorVersion, glMinorVersion;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glMajorVersion);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glMinorVersion);
    std::string glVersionString = "GL_VERSION_";
    glVersionString += std::to_string(glMajorVersion);
    glVersionString += '_';
    glVersionString += std::to_string(glMinorVersion);
    if (!glewIsExtensionSupported(glVersionString.c_str())) return "Requested OpenGL version is not supported";

    // looks good
    this->width = width;
    this->height = height;
    windowReady = true;
    return nullptr;
}

/* Close OpenGL context and window */
void DemoEngine::closeWindow() {
    if (windowReady) {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        width = height = 0;
        windowReady = false;
    }
}
