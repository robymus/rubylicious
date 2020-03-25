#include <stdio.h>
#include "ruby.h"
#include "SDL.h"
#include "GL/glew.h"

#define nullptr ((void*)0)

void render()
{
    GLfloat vertices[] =
            {
                    -0.5f, -0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,
                    0.0f,  0.5f, 0.0f
            };
    static const char *vertexShader =
            "#version 330 core\n"
            "layout(location = 0) in vec2 posAttr;\n"
            "void main() {\n"
            "gl_Position = vec4(posAttr, 0.0, 1.0); }";
    static const char *fragmentShader =
            "#version 330 core\n"
            "out vec4 col;\n"
            "void main() {\n"
            "col = vec4(1.0, 0.0, 0.0, 1.0); }";
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
    glCompileShader(vertexShaderID);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentShader , nullptr);
    glCompileShader(fragmentShaderID);
    GLuint shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);
    glDetachShader(shaderProgramID, vertexShaderID);
    glDetachShader(shaderProgramID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glUseProgram(shaderProgramID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
    glDisableVertexAttribArray(0);
}

int main(int argc, char* argv[])
{
	int state;
	VALUE result;

	ruby_init();

	result = rb_eval_string_protect("puts 'Hello, world!'", &state);

	if (state) {
		fprintf(stderr, "Something went wrong: %d\n", state);
	}

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "sdl init problem\n");
        return -1;
	}


    SDL_Window *window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	
    SDL_GLContext context = SDL_GL_CreateContext(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        exit(1); // or handle the error in a nicer way
    if (!GLEW_VERSION_4_6)  // check that the machine supports the 2.1 API.
        exit(2); // or handle the error in a nicer way


    int running = 1;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    ruby_cleanup(0);
	return 0;
}

