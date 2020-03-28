#ifndef RUBYLICIOUS_RUBYHELPER_H
#define RUBYLICIOUS_RUBYHELPER_H

/*
 * Generic Ruby helper with C++ wrappers, datatype converters and more
 */

#include <SDL.h>
#include <ruby.h>
#include <string>
#include <stdexcept>

namespace RubyHelper {

    // we have some symbol interns prepared for faster operation
    extern ID s_new;
    extern ID s_store;

    /** initialize interns, RubyAPI::initialize calls this after VM is set up */
    void initializeInterns();

    /* Helper method, if the res parameter is non-zero, raises RuntimeException with SDL error message */
    VALUE handleError(int res);

    /* Helper method, if the res parameter is non-null, raises RuntimeException with detailed error message */
    VALUE handleError(const char *res);

    /* Helper method, converts ruby string to lowercase c++ string */
    std::string valueToLowercase(VALUE str);

    /* Helper method, converts ruby string to lowercase c++ string */
    std::string valueToUppercase(VALUE str);

    /* Converts ruby string to GLattr, throws exception if invalid */
    SDL_GLattr stringToGLattr(VALUE attr);

    /* Convert an SDL_Event to a Ruby hash map, returns Qnil if event type is not mapped/supported  */
    VALUE eventToHash(SDL_Event &event);
}

#endif //RUBYLICIOUS_RUBYHELPER_H
