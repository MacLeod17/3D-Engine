
#include "pch.h"
#include <glad\glad.h>
#include "Engine/Graphics/Renderer.h"

int main(int argc, char** argv)
{
	gk::Renderer renderer;
	renderer.Startup();
	renderer.Create("OpenGL", 800, 600);

	// Initialization
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	std::string vertexShaderSource;
	gk::ReadFileToString("Shaders\\basic.vert", vertexShaderSource);

	std::string fragmentShaderSource;
	gk::ReadFileToString("Shaders\\basic.frag", fragmentShaderSource);

	// Create Vertex Buffers
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set Position Pipeline (Vertex Attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Set Color Pipeline (Vertex Attribute)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Create Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* str = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &str, nullptr);
	glCompileShader(vertexShader);

	// Create Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	str = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &str, nullptr);
	glCompileShader(fragmentShader);

	//Create Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glUseProgram(program);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();

		renderer.BeginFrame();

		// Render Triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		renderer.EndFrame();
	}

	return 0;
}
