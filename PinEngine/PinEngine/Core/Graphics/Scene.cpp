#include "Scene.h"

namespace PinEngine
{
	void Scene::AddObject(std::shared_ptr<RenderableEngineObject2D> object)
	{
		objects_2d.push_back(object);
	}

	bool Scene::HasObject(std::shared_ptr<RenderableEngineObject2D> object)
	{
		for (int i = 0; i < objects_2d.size(); i++)
		{
			if (objects_2d[i] == object)
			{
				return true;
			}
		}
		return false;
	}

	void Scene::RemoveObject(std::shared_ptr<RenderableEngineObject2D> object)
	{
		for (int i=0; i<objects_2d.size(); i++)
		{
			if (objects_2d[i] == object)
			{
				objects_2d.erase(objects_2d.begin() + i);
				return;
			}
		}
	}
}