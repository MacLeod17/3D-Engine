#include "pch.h"
#include "Scene.h"

namespace gk
{
	void Scene::Update(float dt)
	{
		for (Object* object : m_objects)
		{
			object->Update(dt);
		}
	}
	
	void Scene::Draw()
	{

	}
}