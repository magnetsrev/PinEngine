#include "SceneGenerator.h"
#include "..//UI//CheckBox.h"
#include "..//..//..//Utility//Timer.h"
#include "..//ResourceManager.h"
#include "..//Helpers//BC4TextureGenerator.h"
#include "..//Helpers//SpritesheetGenerator.h"
#include "..//Helpers//FontManager.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

shared_ptr<Scene> SceneGenerator::GenerateTestScene()
{
	auto scene = make_shared<Scene>();

	shared_ptr<UIFont> font1 = FontManager::GetFont(L"Arial", 20);
	shared_ptr<UIFont> font2 = FontManager::GetFont(L"Times New Roman", 20);
	shared_ptr<UIFont> font3 = FontManager::GetFont(L"Algerian", 20);
	shared_ptr<UIFont> font4 = FontManager::GetFont(L"Castellar", 20);

	shared_ptr<Label> fpsLabel = make_shared<Label>();
	fpsLabel->Initialize(AnchorPoint::TopLeft);
	fpsLabel->SetFont(font1);
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

	shared_ptr<Label> label1 = make_shared<Label>();
	label1->Initialize(AnchorPoint::TopLeft, fpsLabel.get(), AnchorPoint::BottomLeft);
	label1->SetFont(font1);
	label1->SetText(L"[Arial] Multicolored example. |cFF0000FFRed|r, |c00FF00FFGreen|r, and |c0000FFFFBlue|r.");

	shared_ptr<Label> label2 = make_shared<Label>();
	label2->Initialize(AnchorPoint::TopLeft, label1.get(), AnchorPoint::BottomLeft);
	label2->SetFont(font2);
	label2->SetText(L"[Times New Roman] Multicolored example. |cFF0000FFRed|r, |c00FF00FFGreen|r, and |c0000FFFFBlue|r.");


	shared_ptr<Label> label3 = make_shared<Label>();
	label3->Initialize(AnchorPoint::TopLeft, label2.get(), AnchorPoint::BottomLeft);
	label3->SetFont(font3);
	label3->SetText(L"[Algerian] Multicolored example. |cFF0000FFRed|r, |c00FF00FFGreen|r, and |c0000FFFFBlue|r.");

	shared_ptr<Label> label4 = make_shared<Label>();
	label4->Initialize(AnchorPoint::TopLeft, label3.get(), AnchorPoint::BottomLeft);
	label4->SetFont(font4);
	label4->SetText(L"[Castellar] Multicolored example. |cFF0000FFRed|r, |c00FF00FFGreen|r, and |c0000FFFFBlue|r.");

	scene->AddWidget(fpsLabel);
	scene->AddWidget(label1);
	scene->AddWidget(label2);
	scene->AddWidget(label3);
	scene->AddWidget(label4);

	return scene;
}

