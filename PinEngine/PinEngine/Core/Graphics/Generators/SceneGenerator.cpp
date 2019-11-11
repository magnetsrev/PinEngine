#include "SceneGenerator.h"
#include "..//UI//CheckBox.h"
#include "..//..//..//Utility//Timer.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

shared_ptr<Scene> SceneGenerator::GenerateTestScene()
{
	auto scene = make_shared<Scene>();

	shared_ptr<UIFont> font = make_shared<UIFont>();
	bool result = font->LoadFont(L"Data/Fonts/times_new_roman_16.pinfont");

	/*shared_ptr<Label> exampleLabel = make_shared<Label>();
	exampleLabel->Initialize(AnchorPoint::TopLeft);
	exampleLabel->SetFont(font);
	exampleLabel->SetScale(2, 2);
	exampleLabel->SetText(L"Multicolored example. |cFF0000FFRed|r, |c00FF00FFGreen|r, and |c0000FFFFBlue|r.");

	scene->AddWidget(exampleLabel);*/

	/*shared_ptr<Sprite> sp = make_shared<Sprite>();
	sp->Initialize(AnchorPoint::TopLeft);
	sp->SetDimensions(64, 64);
	sp->SetScale(2, 2);
	scene->AddWidget(sp);*/

	shared_ptr<Label> fpsLabel = make_shared<Label>();
	fpsLabel->Initialize(AnchorPoint::TopLeft);
	fpsLabel->SetFont(font);
	fpsLabel->SetScale(2, 2);
	static Timer timer;
	timer.Start();
	fpsLabel->OnUpdate += [](Widget* widget)
	{
		Label* label = (Label*)widget;

		static int frameCount = 0;

		frameCount += 1;
		const double updateInterval = 4; //Update 8 times a second
		if ((timer.GetMilisecondsElapsed()) >= (1000/updateInterval))
		{
			timer.Restart();
			if (frameCount >= (7000/updateInterval))
			{
				label->SetText(L"FPS: |c55FF55FF" + to_wstring(frameCount * (int)updateInterval));
			}
			else
			{
				label->SetText(L"FPS: |cFF5555FF" + to_wstring(frameCount * (int)updateInterval));
			}
			frameCount = 0;
		}
	};

	shared_ptr<Label> exampleLabel = make_shared<Label>();
	exampleLabel->Initialize(AnchorPoint::TopLeft, fpsLabel.get(), AnchorPoint::BottomLeft);
	exampleLabel->SetFont(font);
	exampleLabel->SetText(L"Multicolored example. |cFF0000FFRed|r, |c00FF00FFGreen|r, and |c0000FFFFBlue|r.");
	exampleLabel->SetScale(0.5f, 0.5f);

	fpsLabel->AddChild(exampleLabel);
	scene->AddWidget(fpsLabel);
	return scene;

}