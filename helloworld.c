/*
 Temporary - to test compilation / compatbility

 Compile with:

 gcc -Iarch/linux64/ruby-2.7.0/include/ -Iarch/linux64/ruby-2.7.0/include/x86_64-linux/ -Larch/linux64/ruby-2.7.0/lib -o helloworld helloworld.o -lruby-static -lz -lpthread -lrt -lrt -lgmp -ldl -lcrypt -lm

 i686-w64-mingw32-gcc -Iarch/win32/ruby-2.7.0/include/ -Iarch/win32/ruby-2.7.0/include/i386-mingw32/ -Larch/win32/ruby-2.7.0/lib/ -o helloworld.exe helloworld.c -lmsvcrt-ruby270-static -lshell32 -lws2_32 -liphlpapi -limagehlp -lshlwapi

 Makefile coming soon. 
*/


#include <stdio.h>
#include "ruby.h"
#include "SDL.h" 

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


	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
	"SDL2Test",
	SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED,
	640,
	480,
	0
	);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	SDL_DestroyWindow(window);
	SDL_Quit();

	/* destruct the VM */
	return ruby_cleanup(0);
}

