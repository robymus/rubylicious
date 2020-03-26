#ifndef RUBYLICIOUS_RUBYWRAPPER_HPP
#define RUBYLICIOUS_RUBYWRAPPER_HPP

/*
 * C++ interface for Ruby C functions
 * Limited, extended when needed
 */

#include "ruby.h"
#include <initializer_list>
#include <algorithm>

/* anonymous namespace to hold implementation details */
namespace Ruby {

    /* reference to the Demo singleton object */
    VALUE rb_cDemo;

#define RUBYWRAPPER_TYPENAME_TO_STRING(s) #s

    /* ruby class wrapping data type */
    template <class T>
    const rb_data_type_t rb_data_type = {
            .wrap_struct_name = RUBYWRAPPER_TYPENAME_TO_STRING(T),
            .function = {
                    .dmark = NULL,
                    .dfree = [](void *t) { delete static_cast<T*>(t); },
                    .dsize = [](const void *) { return sizeof(T); }
            },
            .data = NULL,
            .flags = RUBY_TYPED_FREE_IMMEDIATELY
    };

    /* used to store ruby class references for C++ classes */
    template <class T> VALUE rb_class;

    /*
     * Some kind of magic to get method argument count
     * -> https://stackoverflow.com/questions/36797770/get-function-parameters-count
     */
    template <typename R, typename ... Types>
    constexpr std::integral_constant<unsigned, sizeof ...(Types)> getArgumentCount( R(*f)(Types ...))
    {
        return std::integral_constant<unsigned, sizeof ...(Types)>{};
    }


    /* Initialize VM, set up empty Demo object */
    void init() {
        ruby_init();

        rb_cDemo = rb_define_class("Demo", rb_cObject);
        rb_define_singleton_method_tmpl<-2>::define(rb_cDemo, "new",[](VALUE self, VALUE args) -> VALUE {
            rb_raise(rb_eRuntimeError, "Can't instantiate new Rubylicious Demo object");
        });
    }

    /*
     * Run main script (eg. enty point)
     * Note: should start with ./
     * Returns true, if terminated successfully, false in case of Exception (logged to stderr as well)
     */
    bool runScript(const char *mainScriptName) {
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
                fprintf(stderr, "Ruby exited with Exception\n%s\n", StringValueCStr(full_message));
            }
            else {
                fprintf(stderr, "Ruby exited with null Exception (I'm as confused as you are)\n");
            }
            return false;
        }
        else {
            return true;
        }
    }

    /*
     * Wrap a class as a Ruby class
     * Returns the ruby class object (needed to add new methods)
     */
    template<class T> VALUE wrapClass(const char *rubyClassName) {
        rb_class<T> = rb_define_class(rubyClassName, rb_cData);
        rb_define_alloc_func(rb_class<T>, [](VALUE self) {
            auto data = new T();
            return TypedData_Wrap_Struct(self, &rb_data_type<T>, data);
        });
        return rb_class<T>;
    }

    /*
     * Unwraps a ruby class to a C++ class pointer
     */
    template<class T> T* unwrap(VALUE obj) {
        T *data;
        TypedData_Get_Struct(obj, T, &rb_data_type<T>, data);
        return data;
    }

    /*
     * Creates a new ruby class instance (based on C++ class, that is already wrapped)
         * Arguments should be passed as initializer list (eg. newInstance<Template>({v1, v2})
         */
    template<class T> VALUE newInstance(std::initializer_list<VALUE> args) {
        int argCnt = args.size();
        if (argCnt == 0) {
            return rb_funcall(rb_class<T>, rb_intern("new"), 0);
        }
        else {
            // collect arguments as VALUE[]
            VALUE argsArray[argCnt];
            std::copy(args.begin(), args.end(), argsArray);
            return rb_funcallv(rb_class<T>, rb_intern("new"), argCnt, &argsArray[0]);
        }
    }

    /* Closes the VM */
    void finalize() {
        ruby_finalize();
    }

}

#endif //RUBYLICIOUS_RUBYWRAPPER_HPP
