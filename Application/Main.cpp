
#include "pch.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Program.h"

int main(int argc, char** argv)
{
	gk::Renderer renderer;
	renderer.Startup();
	renderer.Create("OpenGL", 800, 600);

	// Initialization
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	};

	gk::Program program;
	program.CreateShaderFromFile("shaders\\basic.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders\\basic.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	// Create Vertex Buffers
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set Position Pipeline (Vertex Attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Set Color Pipeline (Vertex Attribute)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set UV Pipeline (Vertex Attribute)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Uniform
	glm::mat4 transform = glm::mat4(1.0f);
	program.SetUniform("transform", transform);

	gk::Texture texture;
	texture.CreateTexture("Textures\\llama.jpg");

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

		transform = glm::rotate(transform, 0.001f, glm::vec3{ 0, 0, 1 });
		program.SetUniform("transform", transform);

		renderer.BeginFrame();

		// Render Triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		renderer.EndFrame();
	}

	return 0;
}
