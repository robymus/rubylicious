/*
 Temporary - to test compilation / compatbility

 Compile with:

 gcc -Iarch/linux64/ruby-2.7.0/include/ -Iarch/linux64/ruby-2.7.0/include/x86_64-linux/ -Larch/linux64/ruby-2.7.0/lib -o helloworld helloworld.o -lruby-static -lz -lpthread -lrt -lrt -lgmp -ldl -lcrypt -lm

 i686-w64-mingw32-gcc -Iarch/win32/ruby-2.7.0/include/ -Iarch/win32/ruby-2.7.0/include/i386-mingw32/ -Larch/win32/ruby-2.7.0/lib/ -o helloworld.exe helloworld.c -lmsvcrt-ruby270-static -lshell32 -lws2_32 -liphlpapi -limagehlp -lshlwapi

 Makefile coming soon. 
*/


#include <stdio.h>
#include <ruby.h>

int main(int argc, char* argv[])
{
	int state;
	VALUE result;

	/* construct the VM */
	ruby_init();

	/* Ruby goes here */
	result = rb_eval_string_protect("puts 'Hello, world!'", &state);

	if (state) {
		fprintf(stderr, "Something went wrong: %d\n", state);
	}

	/* destruct the VM */
	return ruby_cleanup(0);
}
