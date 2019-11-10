#include "SceneGenerator.h"
#include "..//UI//CheckBox.h"

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
	lbl1->SetColor(Color(75, 75, 255));
	lbl1->SetText(L"This is a |cFFFF00FFmulticolored|r text test. Pipe check: ||One ||||Two ||||||Three\nNew Line Test");

	shared_ptr<CheckBox> cb1 = make_shared<CheckBox>();
	cb1->Initialize(AnchorPoint::TopLeft, testSprite.get(), AnchorPoint::BottomLeft);
	cb1->SetPosition(0, -5);
	cb1->SetFont(font);
	cb1->SetText(L"Click the checkbox!");
	cb1->OnChecked += [](CheckBox* cb)
	{
		cb->SetText(L"The checkbox was checked!");
		cb->SetTextColor(Color(45, 200, 45));
	};
	cb1->OnUnchecked += [](CheckBox* cb)
	{
		cb->SetText(L"The checkbox was unchecked! :O");
		cb->SetTextColor(Color(200, 45, 45));
	};

	testSprite->AddChild(lbl1);
	testSprite->AddChild(cb1);
	scene->AddWidget(testSprite);

	return scene;

}