/*
 * Demo engine entry point
 */

#include "RubyWrapper.hpp"
#include <string>

using namespace std;

class Texture
{
public:
    string name;
};

int main(int argc, char *argv[]) {

    Ruby::init();

    rb_define_singleton_method_tmpl<0>::define(Ruby::rb_cDemo, "renderFinal", [](VALUE self)->VALUE {
        printf("renderFinal\n");
        if (rb_block_given_p()) {
            rb_yield(Qundef);
        }
        printf("renderFinal done\n");
        return Qnil;
    });
    rb_define_singleton_method_tmpl<1>::define(Ruby::rb_cDemo, "renderTexture", [](VALUE self, VALUE arg1) -> VALUE {
        printf("render to texture: %s\n", StringValueCStr(arg1));

        VALUE textureRubyObj = Ruby::newInstance<Texture>({arg1});

        if (rb_block_given_p()) {
            rb_yield(Qundef);
        }
        printf("render to texture: %s - done\n", StringValueCStr(arg1));

        return textureRubyObj;
    });

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

    Ruby::runScript("./rb/main.rb");

    Ruby::finalize();

    return 0;
}