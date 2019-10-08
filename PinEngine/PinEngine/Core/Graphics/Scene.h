#pragma once
#include <vector>
#include "RenderableEngineObject2D.h"

namespace PinEngine
{
	class Scene
	{
		friend class Renderer;
	public:
		void AddObject(std::shared_ptr<RenderableEngineObject2D> object);
		bool HasObject(std::shared_ptr<RenderableEngineObject2D> object);
		void RemoveObject(std::shared_ptr<RenderableEngineObject2D> object);
		std::vector<std::shared_ptr<RenderableEngineObject2D>> Get2DObjects();
	private:
		std::vector<std::shared_ptr<RenderableEngineObject2D>> objects_2d;

	};
}