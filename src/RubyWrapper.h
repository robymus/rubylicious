#ifndef RUBYLICIOUS_RUBYWRAPPER_H
#define RUBYLICIOUS_RUBYWRAPPER_H

/*
 * C++ interface for Ruby C functions
 * Limited, extended when needed
 * Also a bunch of helpers :)
 */

#include <ruby.h>
#include <string>

/* anonymous namespace to hold implementation details */
namespace Ruby {

#define RUBYHELPER_TYPENAME_TO_STRING(s) #s

    /* ruby class wrapping data type */
    template <class T>
    const rb_data_type_t rb_data_type = {
            .wrap_struct_name = RUBYHELPER_TYPENAME_TO_STRING(T),
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

}


#endif //RUBYLICIOUS_RUBYWRAPPER_H
