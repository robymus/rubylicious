#include "RubyHelper.h"
#include "RubyWrapper.h"

/*
 * Implementation of  Ruby helper with C++ wrappers, datatype converters and more
 */

namespace RubyHelper {

    // we have some symbol interns prepared for faster operation
    ID s_new;
    ID s_store;

    /** initialize interns, RubyAPI::initialize calls this after VM is set up */
    void initializeInterns() {
        s_new = rb_intern("new");
        s_store = rb_intern("store");
    }
}

/* Helper method, if the res parameter is non-null, raises RuntimeException with detailed error message */
VALUE RubyHelper::handleError(int res) {
    if (res) rb_raise(rb_eRuntimeError, "%s", SDL_GetError());
    else return Qnil;
}

/* Helper method, if the res parameter is non-null, raises RuntimeException with detailed error message */
VALUE RubyHelper::handleError(const char *res) {
    if (res) rb_raise(rb_eRuntimeError, "%s", res);
    else return Qnil;
}

/* Helper method, converts ruby string to lowercase c++ string */
std::string RubyHelper::valueToLowercase(VALUE str) {
    std::string result;
    for (const char *p = StringValueCStr(str); *p; p++) {
        result += tolower(*p);
    }
    return result;
}

/* Helper method, converts ruby string to lowercase c++ string */
std::string RubyHelper::valueToUppercase(VALUE str) {
    std::string result;
    for (const char *p = StringValueCStr(str); *p; p++) {
        result += toupper(*p);
    }
    return result;
}



/* Converts ruby string to GLattr, throws exception if invalid */
SDL_GLattr RubyHelper::stringToGLattr(VALUE str) {
    SDL_GLattr attr;
    bool attrValid = false;
    auto attrStr = RubyHelper::valueToUppercase(str);
    // this is ugly with macro, but there is no nice way to convert string to enum
    #define RUBYAPI_GLATTR_MAP(a) if (attrStr == #a) { attrValid = true; attr = SDL_GL_##a; }
    RUBYAPI_GLATTR_MAP(RED_SIZE)
    RUBYAPI_GLATTR_MAP(GREEN_SIZE)
    RUBYAPI_GLATTR_MAP(BLUE_SIZE)
    RUBYAPI_GLATTR_MAP(ALPHA_SIZE)
    RUBYAPI_GLATTR_MAP(BUFFER_SIZE)
    RUBYAPI_GLATTR_MAP(DOUBLEBUFFER)
    RUBYAPI_GLATTR_MAP(DEPTH_SIZE)
    RUBYAPI_GLATTR_MAP(STENCIL_SIZE)
    RUBYAPI_GLATTR_MAP(ACCUM_RED_SIZE)
    RUBYAPI_GLATTR_MAP(ACCUM_GREEN_SIZE)
    RUBYAPI_GLATTR_MAP(ACCUM_BLUE_SIZE)
    RUBYAPI_GLATTR_MAP(ACCUM_ALPHA_SIZE)
    RUBYAPI_GLATTR_MAP(STEREO)
    RUBYAPI_GLATTR_MAP(MULTISAMPLEBUFFERS)
    RUBYAPI_GLATTR_MAP(MULTISAMPLESAMPLES)
    RUBYAPI_GLATTR_MAP(ACCELERATED_VISUAL)
    RUBYAPI_GLATTR_MAP(CONTEXT_MAJOR_VERSION)
    RUBYAPI_GLATTR_MAP(CONTEXT_MINOR_VERSION)
    RUBYAPI_GLATTR_MAP(CONTEXT_FLAGS)
    RUBYAPI_GLATTR_MAP(CONTEXT_PROFILE_MASK)
    RUBYAPI_GLATTR_MAP(SHARE_WITH_CURRENT_CONTEXT)
    RUBYAPI_GLATTR_MAP(FRAMEBUFFER_SRGB_CAPABLE)
    RUBYAPI_GLATTR_MAP(CONTEXT_RELEASE_BEHAVIOR)
    if (!attrValid) throw std::invalid_argument(attrStr);
    return attr;
}

// anonymous namespace for local thingies
namespace {

    // magical code again: enum to integer - https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
    template <typename Enumeration> auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }

    // helper mapping method for keyboard events
    VALUE keyboardEventToHash(SDL_Event &event) {
        auto ret = rb_funcall(rb_cHash, RubyHelper::s_new, 0);
        rb_funcall(ret, RubyHelper::s_store, 2, ID2SYM(rb_intern("source")), rb_str_new_cstr("keyboard"));
        rb_funcall(ret, RubyHelper::s_store, 2, ID2SYM(rb_intern("type")), rb_str_new_cstr(event.type == SDL_KEYUP ? "KEYUP" : "KEYDOWN"));
        rb_funcall(ret, RubyHelper::s_store, 2, ID2SYM(rb_intern("scancode")), INT2NUM(as_integer(event.key.keysym.scancode)));
        rb_funcall(ret, RubyHelper::s_store, 2, ID2SYM(rb_intern("key")), INT2NUM(event.key.keysym.sym));
        rb_funcall(ret, RubyHelper::s_store, 2, ID2SYM(rb_intern("modifier")), UINT2NUM(event.key.keysym.mod));
        return ret;
    }

    // helper mapping method for quit events
    VALUE quitEventToHash(SDL_Event &event) {
        auto ret = rb_funcall(rb_cHash, RubyHelper::s_new, 0);
        rb_funcall(ret, RubyHelper::s_store, 2, ID2SYM(rb_intern("source")), rb_str_new_cstr("quit"));
        rb_funcall(ret, RubyHelper::s_store, 2, ID2SYM(rb_intern("type")), rb_str_new_cstr("QUIT"));
        return ret;
    }
}

/* Convert an SDL_Event to a Ruby hash map, returns Qnil if event type is not mapped/supported  */
VALUE RubyHelper::eventToHash(SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            return keyboardEventToHash(event);
            break;
        case SDL_QUIT:
            return quitEventToHash(event);
            break;
        default:
            return Qnil;
    }
}
