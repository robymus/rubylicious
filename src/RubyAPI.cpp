/*
 * This file contains the Ruby API / Demo Engine linkage and exposure code.
 *
 * The RubyWrapper.hpp file contains some wrapping functions for the Ruby C API
 */

#include "RubyWrapper.h"
#include "RubyHelper.h"
#include <string>

#include "engine/DemoEngine.h"

namespace Ruby {

    // reference to the Demo singleton object
    VALUE rb_cDemo;

    // the demo engine is created as a static instance here
    DemoEngine demo;

    /*
     * Bind raw exposed SDL methods to the Demo class (where exposed functionality is identical to SDL)
     */
    void bindRawSDLInterface() {
        // Demo.messageBox(title, message, type)
        rb_define_singleton_method_tmpl<3>::define(rb_cDemo, "messageBox",
                [](VALUE self, VALUE title, VALUE message, VALUE type) -> VALUE {
                    int sdlType = SDL_MESSAGEBOX_INFORMATION;
                    auto typeStr = RubyHelper::valueToLowercase(type);
                    if (typeStr == "error") sdlType = SDL_MESSAGEBOX_ERROR;
                    if (typeStr == "warning") sdlType = SDL_MESSAGEBOX_WARNING;
                    SDL_ShowSimpleMessageBox(
                        sdlType,
                        StringValueCStr(title),
                        StringValueCStr(message),
                        nullptr
                    );
                    return Qnil;
                }
        );

        // Demo.setGLAttribute(attribute, value)
        rb_define_singleton_method_tmpl<2>::define(rb_cDemo, "setGLAttribute",
               [](VALUE self, VALUE attribute, VALUE value) -> VALUE {
                   SDL_GLattr attr;
                   try {
                       attr = RubyHelper::stringToGLattr(attribute);
                   } catch (const std::invalid_argument &e) {
                       rb_raise(rb_eRuntimeError, "Invalid attribute name");
                   }
                   int valInt;
                   if (attr == SDL_GL_CONTEXT_PROFILE_MASK) {
                       auto valStr = RubyHelper::valueToUppercase(value);
                       if (valStr == "CORE") valInt = SDL_GL_CONTEXT_PROFILE_CORE;
                       else if (valStr == "COMPATIBILITY") valInt = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
                       else if (valStr == "ES") valInt = SDL_GL_CONTEXT_PROFILE_ES;
                       else rb_raise(rb_eRuntimeError, "Value for CONTEXT_PROFILE MASK has to be CORE, COMPATIBILITY or ES");
                   }
                   else {
                       valInt = NUM2INT(value);
                   }
                   return RubyHelper::handleError( SDL_GL_SetAttribute(attr, valInt) );
               }
        );

        // Demo.setVSync(value)
        rb_define_singleton_method_tmpl<1>::define(rb_cDemo, "setVSync",
               [](VALUE self, VALUE value) -> VALUE {
                   int vsync = NUM2INT(value);
                   int res = SDL_GL_SetSwapInterval(vsync);
                   if (res == -1 && vsync == -1) {
                       // if adaptive vsync is not supported, try normal
                       vsync = 1;
                       res = SDL_GL_SetSwapInterval(vsync);
                   }
                   // if still not good, error
                   if (res) rb_raise(rb_eRuntimeError, SDL_GetError());
                   // otherwise return value set
                   return vsync;
               }
        );

        // Demo.pollEvents { |evt| ... }
        rb_define_singleton_method_tmpl<0>::define(rb_cDemo, "pollEvents",
               [](VALUE self) -> VALUE {
                   rb_need_block();
                   SDL_Event event;
                   while (SDL_PollEvent(&event) != 0) {
                       auto eventHash = RubyHelper::eventToHash(event);
                       if (eventHash != Qnil) rb_yield(eventHash);
                   }
                   return Qnil;
               }
        );
    }

    /**
     * Bind the DemoEngine objects methods to Demo class (singleton object)
     */
    void bindDemoObject() {
        // Demo.new
        rb_define_singleton_method_tmpl<-2>::define(rb_cDemo, "new", [](VALUE self, VALUE args) -> VALUE {
            rb_raise(rb_eRuntimeError, "Can't instantiate new Rubylicious Demo object");
        });

        // Demo.init
        rb_define_singleton_method_tmpl<0>::define(rb_cDemo, "init",
                [](VALUE self) -> VALUE { return RubyHelper::handleError(demo.initialize()); }
        );

        // Demo.shutdown
        rb_define_singleton_method_tmpl<0>::define(rb_cDemo, "shutdown",
               [](VALUE self) -> VALUE { demo.shutdown(); return Qnil; }
        );

        // Demo.createWindow(title width, height, fullScreen = false, display = 0)
        rb_define_singleton_method_tmpl<-1>::define(rb_cDemo, "createWindow",
                [](int argc, VALUE *argv, VALUE self) -> VALUE {
                    if (argc < 3) rb_raise(rb_eRuntimeError, "createWindow requires at least 3 arguments");
                    const char *title = StringValueCStr(argv[0]);
                    int width = NUM2INT(argv[1]);
                    int height = NUM2INT(argv[2]);
                    bool fullscreen = argc > 3 ? RTEST(argv[3]) : false;
                    int display = argc > 4 ? NUM2INT(argv[4]) : 0;
                    RubyHelper::handleError( demo.createWindow(title, width, height, fullscreen, display) );
                    return Qnil;
                }
        );

        // Demo.closeWindow
        rb_define_singleton_method_tmpl<0>::define(rb_cDemo, "closeWindow",
               [](VALUE self) -> VALUE { demo.closeWindow(); return Qnil; }
        );

        // Demo.width / Demo.height
        rb_define_singleton_method_tmpl<0>::define(rb_cDemo, "width",
                [](VALUE self) -> VALUE { return INT2NUM(demo.width); }
        );
        rb_define_singleton_method_tmpl<0>::define(rb_cDemo, "height",
               [](VALUE self) -> VALUE { return INT2NUM(demo.height); }
        );

    }


    /*
     * Texture class
        // Demo.renderFinal
        rb_define_singleton_method_tmpl<0>::define(Ruby::rb_cDemo, "renderFinal", [](VALUE self)->VALUE {
            printf("renderFinal\n");
            if (rb_block_given_p()) {
                rb_yield(Qundef);
            }
            printf("renderFinal done\n");
            return Qnil;
        });

        // Demo.renderTexture
        rb_define_singleton_method_tmpl<1>::define(Ruby::rb_cDemo, "renderTexture", [](VALUE self, VALUE arg1) -> VALUE {
            printf("render to texture: %s\n", StringValueCStr(arg1));

            VALUE textureRubyObj = Qnil; // rb_funcall(rb_class<Texture>, s_new, 1, arg1);

            if (rb_block_given_p()) {
                rb_yield(Qundef);
            }
            printf("render to texture: %s - done\n", StringValueCStr(arg1));

            return textureRubyObj;
        });
    void bindTextureClass() {

        VALUE textureClass = Ruby::wrapClass<Texture>("Texture");

        rb_define_method_tmpl<1>::define(textureClass, "initialize", [](VALUE self, VALUE name)->VALUE {
            auto data = Ruby::unwrap<Texture>(self);
            data->name = StringValueCStr(name);
            return self;
        });

        rb_define_method_tmpl<0>::define(textureClass, "name", [](VALUE self)->VALUE {
            auto data = Ruby::unwrap<Texture>(self);
            return rb_str_new_cstr(data->name.c_str());
        });
    }
     */

    /* Initialize VM, set up API */
    void initialize() {
        ruby_init();
        RubyHelper::initializeInterns();

        // create Demo class
        rb_cDemo = rb_define_class("Demo", rb_cObject);

        bindRawSDLInterface();
        bindDemoObject();
        // bindTextureClass();
    }

    /*
     * Run main script (eg. enty point)
     * Note: should start with ./
     * Returns exception as string, if terminated with Exception, nullptr otherwise
     */
    const char *runScript(const char *mainScriptName) {
        // call requireMainRubyScript with exception handler
        int runRubyScriptState;
        rb_protect(
                [](VALUE filename) { return rb_f_require(Qnil, filename); },
                rb_str_new_cstr(mainScriptName), &runRubyScriptState);
        if (runRubyScriptState) {
            // in case of an exception is caught
            auto exception = rb_errinfo();
            // check for not-null
            if (RTEST(exception)) {
                // invoke .full_message to get full message
                auto full_message = rb_funcall(exception, rb_intern("full_message"), 0);
                return StringValueCStr(full_message);
            }
            else {
                return"Ruby exited with null Exception (I'm as confused as you are)";
            }
        }
        else {
            // no error
            return nullptr;
        }
    }

    /* Closes the VM */
    void shutdown() {
        demo.shutdown(); // just in case ruby didn't close down the demo engine correctly
        ruby_finalize();
    }


}