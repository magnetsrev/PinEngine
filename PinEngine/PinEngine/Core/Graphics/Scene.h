#pragma once
#include <vector>
#include "RenderableEngineObject2D.h"
#include "UI//Sprite.h"

namespace PinEngine
{
	class Scene
	{
		friend class Renderer;
	public:
		/*void AddObject(std::shared_ptr<RenderableEngineObject2D> object);
		bool HasObject(std::shared_ptr<RenderableEngineObject2D> object);
		void RemoveObject(std::shared_ptr<RenderableEngineObject2D> object);
		std::vector<std::shared_ptr<RenderableEngineObject2D>> Get2DObjects();
		std::shared_ptr<RenderableEngineObject2D> focusedWidget = nullptr;*/
		void AddWidget(std::shared_ptr<UI::Widget> widget);
		bool HasWidget(std::shared_ptr<UI::Widget> widget);
		void RemoveWidget(std::shared_ptr<UI::Widget> widget);
		std::vector<std::shared_ptr<UI::Widget>> GetWidgets();
	private:
		//std::vector<std::shared_ptr<RenderableEngineObject2D>> objects_2d;
		std::vector<std::shared_ptr<UI::Widget>> widgets;
	};
}