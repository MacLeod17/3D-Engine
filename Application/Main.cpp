
#include "pch.h"
#include "Engine/Engine.h"

int main(int argc, char** argv)
{
	gk::Engine engine;
	engine.Startup();

	static float vertices[] = {
		// front
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		// back
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	};

	// Unsigned short - 16bit
	static GLushort indices[] =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	gk::Program program;
	program.CreateShaderFromFile("shaders\\basic.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders\\basic.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	gk::VertexIndexArray vertexArray;
	vertexArray.Create("vertex");
	vertexArray.CreateBuffer(sizeof(vertices), sizeof(vertices) / (sizeof(float) * 5), vertices);
	vertexArray.SetAttribute(0, 3, 5 * sizeof(float), 0);
	vertexArray.SetAttribute(1, 2, 5 * sizeof(float), (3 * sizeof(float)));
	vertexArray.CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(indices) / sizeof(GLushort), indices);

	// Uniform
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800 / (float)600, 0.01f, 1000.0f);

	glm::vec3 eye{ 0, 0, 5 };
	glm::mat4 view = glm::lookAt(eye, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });

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
		engine.Update();

		float angle = 0;
		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_E) == gk::InputSystem::eButtonState::HELD)
		{
			angle = 2.0f;
		}
		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_Q) == gk::InputSystem::eButtonState::HELD)
		{
			angle = -2.0f;
		}
		model = glm::rotate(model, angle * engine.GetTimer().DeltaTime(), glm::vec3{ 0, 1, 0 });

		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_A) == gk::InputSystem::eButtonState::HELD)
		{
			eye.x -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_D) == gk::InputSystem::eButtonState::HELD)
		{
			eye.x += 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_W) == gk::InputSystem::eButtonState::HELD)
		{
			eye.z -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_S) == gk::InputSystem::eButtonState::HELD)
		{
			eye.z += 4 * engine.GetTimer().DeltaTime();
		}
		view = glm::lookAt(eye, eye + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

		glm::mat4 mvp = projection * view * model;
		program.SetUniform("transform", mvp);

		engine.GetSystem<gk::Renderer>()->BeginFrame();

		// Render Triangle
		vertexArray.Draw();

		engine.GetSystem<gk::Renderer>()->EndFrame();
	}

	engine.Shutdown();

	return 0;
}
