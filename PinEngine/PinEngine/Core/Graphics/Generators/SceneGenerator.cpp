#include "SceneGenerator.h"
#include "..//UI//Label.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

shared_ptr<Scene> SceneGenerator::GenerateTestScene()
{
	auto scene = make_shared<Scene>();
	
	auto testSprite = make_shared<Sprite>();
	testSprite->Initialize(AnchorPoint::Center);
	testSprite->SetDimensions(100, 100);
	testSprite->AssignTexture(L"Data/Textures/smile.png");
	testSprite->EnableDrag(true);

	shared_ptr<UIFont> font = make_shared<UIFont>();
	bool result = font->LoadFont(L"Data/Fonts/times_new_roman_16.pinfont");

	shared_ptr<Label> lbl1 = make_shared<Label>();
	lbl1->Initialize(AnchorPoint::BottomLeft, testSprite.get(), AnchorPoint::TopLeft);
	lbl1->SetFont(font);
	lbl1->SetText(L"Topleft label.");

	shared_ptr<Label> lbl2 = make_shared<Label>();
	lbl2->Initialize(AnchorPoint::TopLeft, testSprite.get(), AnchorPoint::BottomLeft);
	lbl2->SetFont(font);
	lbl2->SetText(L"Bottomleft label.");

	scene->AddWidget(testSprite);
	scene->AddWidget(lbl1);
	scene->AddWidget(lbl2);

	return scene;


	/*testSprite->OnUpdate += [](Widget* widget)
	{
		widget->AdjustRotation(0, 0, 0.01f);
	};
	testSprite->OnLeftClick += [](Widget* widget)
	{
		widget->AdjustRotation(0, 0, 0.6f);
	};*/

	//scene->AddWidget(shared_ptr<Widget>(label));

	//auto bigSprite = std::make_shared<RenderableEngineObject2D>();
	//bigSprite->Initialize(UI::AnchorPoint::TopLeft, nullptr, UI::AnchorPoint::TopLeft);
	//bigSprite->SetDimensions(256, 256);

	//bigSprite->EnableDrag(true); //Allow drag, no snapping
	//bigSprite->OnLeftClick += [](RenderableEngineObject2D* obj)
	//{
	//	obj->AssignTexture(L"Data/Textures/mario.png");
	//};

	//bigSprite->OnLeftRelease += [](RenderableEngineObject2D* obj)
	//{
	//	obj->AssignTexture(L"missingtexture");
	//};

	//auto littleSprite = std::make_shared<RenderableEngineObject2D>();
	//littleSprite->EnableDrag(true, 32, 32); //Allow drag & snap every 32 px for x and y
	//littleSprite->Initialize(AnchorPoint::TopLeft, bigSprite, AnchorPoint::BottomLeft);
	//littleSprite->AssignTexture(L"Data/Textures/smiley.png");
	//littleSprite->SetDimensions(64, 64);

	//scene->AddObject(bigSprite);
	//scene->AddObject(littleSprite);
}