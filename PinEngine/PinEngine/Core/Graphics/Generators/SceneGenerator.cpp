#include "SceneGenerator.h"

namespace PinEngine
{
	std::shared_ptr<Scene> PinEngine::SceneGenerator::GenerateTestScene()
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		std::shared_ptr<RenderableEngineObject2D> obj1 = std::make_shared<RenderableEngineObject2D>();
		obj1->Initialize(AnchorPoint::Center, nullptr, AnchorPoint::Center);
		obj1->SetDimensions(64, 64);

		obj1->ToggleMouseInteraction(true);
		obj1->OnMouseOver += [](RenderableEngineObject2D* obj)
		{
			obj->SetScale(1.3, 1.3);
		};

		obj1->OnMouseExit += [](RenderableEngineObject2D* obj)
		{
			obj->SetScale(1, 1);
		};

		obj1->OnUpdate += [](RenderableEngineObject2D* obj)
		{
			if (obj->IsMouseOver())
			{
				obj->AdjustRotation(0, 0, 0.02f);
			}
			else
			{
				obj->AdjustRotation(0, 0, -0.02f);
			}
		};

		scene->AddObject(obj1);

		std::shared_ptr<RenderableEngineObject2D> obj2 = std::make_shared<RenderableEngineObject2D>();
		obj2->Initialize(AnchorPoint::TopLeft, obj1, AnchorPoint::BottomLeft);
		obj2->AssignTexture(L"Data/Textures/smiley.png");
		obj2->SetDimensions(32, 32);
		scene->AddObject(obj2);
		return scene;
	}
}