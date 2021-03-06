
#include "pch.h"
#include "Engine/Engine.h"

int main(int argc, char** argv)
{
	gk::Engine engine;
	engine.Startup();

	gk::Scene scene{ &engine };

	gk::Program program;
	program.CreateShaderFromFile("shaders/phong.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders/phong.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	gk::VertexArray vertexArray = gk::Model::Load("models/ogre.obj");

	gk::Texture texture;
	texture.CreateTexture("Textures/ogre_diffuse_flip.bmp");

	gk::Material material{ glm::vec3{ 1 }, glm::vec3{ 1 }, glm::vec3{ 1 }, 32.0f };
	material.AddTexture(texture);
	material.SetProgram(program);

	gk::Model model{ "model", gk::Transform{}, vertexArray, program, material };
	scene.Add(&model);

	// Camera
	glm::vec3 eye{ 0, 0, 5 };

	gk::Camera camera{ "camera" };
	scene.Add(&camera);
	camera.SetProjection(45.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
	camera.SetLookAt(eye, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });

	gk::Light light{ "light", 
		gk::Transform{ glm::vec3{5, 2, 5} }, 
		glm::vec3{ 0.1f }, 
		glm::vec3{ 1 },
		glm::vec3{ 1 } };
	scene.Add(&light);

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

		scene.Update(engine.GetTimer().DeltaTime());

		float angle = 0;
		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_E) == gk::InputSystem::eButtonState::HELD)
		{
			angle = 2.0f;
		}
		if (engine.GetSystem<gk::InputSystem>()->GetButtonState(SDL_SCANCODE_Q) == gk::InputSystem::eButtonState::HELD)
		{
			angle = -2.0f;
		}
		model.transform().rotation.y += angle * engine.GetTimer().DeltaTime();

		std::vector<gk::Light*> lights = scene.Get<gk::Light>();
		if (!lights.empty())
		{
			for (auto light : lights)
			{
				light->SetProgram(program);
			}
		}

		engine.GetSystem<gk::Renderer>()->BeginFrame();

		scene.Draw();

		engine.GetSystem<gk::Renderer>()->EndFrame();
	}

	engine.Shutdown();

	return 0;
}
