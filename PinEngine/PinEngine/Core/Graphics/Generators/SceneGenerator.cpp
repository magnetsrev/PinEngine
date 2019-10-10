#include "SceneGenerator.h"

namespace PinEngine
{
	std::shared_ptr<Scene> PinEngine::SceneGenerator::GenerateTestScene()
	{
		auto scene = std::make_shared<Scene>();
		auto bigSprite = std::make_shared<RenderableEngineObject2D>();
		bigSprite->Initialize(AnchorPoint::TopLeft, nullptr, AnchorPoint::TopLeft);
		bigSprite->SetDimensions(256, 256);

		bigSprite->EnableDrag(true); //Allow drag, no snapping
		bigSprite->OnLeftClick += [](RenderableEngineObject2D* obj)
		{
			obj->AssignTexture(L"Data/Textures/mario.png");
		};

		bigSprite->OnLeftRelease += [](RenderableEngineObject2D* obj)
		{
			obj->AssignTexture(L"missingtexture");
		};

		auto littleSprite = std::make_shared<RenderableEngineObject2D>();
		littleSprite->EnableDrag(true, 32, 32); //Allow drag & snap every 32 px for x and y
		littleSprite->Initialize(AnchorPoint::TopLeft, bigSprite, AnchorPoint::BottomLeft);
		littleSprite->AssignTexture(L"Data/Textures/smiley.png");
		littleSprite->SetDimensions(64, 64);

		scene->AddObject(bigSprite);
		scene->AddObject(littleSprite);
		return scene;
	}
}