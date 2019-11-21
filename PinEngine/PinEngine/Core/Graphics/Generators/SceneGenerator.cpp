#include "SceneGenerator.h"
#include "..//UI//CheckBox.h"
#include "..//..//..//Utility//Timer.h"
#include "..//ResourceManager.h"
#include "..//Helpers//BC4TextureGenerator.h"
#include "..//Helpers//SpritesheetGenerator.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

shared_ptr<Scene> SceneGenerator::GenerateTestScene()
{
	auto scene = make_shared<Scene>();

	/*shared_ptr<UIFont> font = make_shared<UIFont>();
	bool result = font->LoadFont(L"Data/Fonts/times_new_roman_16.pinfont");*/

	Timer t;
	t.Start();
	SpritesheetGenerator sg;
	sg.GenerateSheet(L"Arial", 20);


	shared_ptr<UIFont> font = make_shared<UIFont>();
	for (auto& g : sg.glyphs)
	{
		UIFont::FontGlyph glyph;
		glyph.Character = g.character;
		RECT area;
		area.left = g.drawOffsetX;
		area.top = g.drawOffsetY;
		area.bottom = g.drawOffsetY + g.charHeight;
		area.right = g.drawOffsetX + g.charWidth;
		glyph.Subrect = area;
		glyph.YOffset = g.yOffset;
		glyph.XAdvance = 0;
		glyph.XOffset = 0;
		font->glyphs.push_back(glyph);
	}
	font->lineSpacing = sg.lineSpacing;
	font->textureHeight = sg.GetSheetTexture()->GetDimensions().y;
	font->textureWidth = sg.GetSheetTexture()->GetDimensions().x;
	font->defaultGlyph = font->FindGlyph('?');
	font->texture = sg.GetSheetTexture();
	//bool result = font->LoadFont(L"Data/Fonts/times_new_roman_16.pinfont");

	t.Stop();
	float dt = t.GetMilisecondsElapsed();
	OutputDebugStringA("A:");
	OutputDebugStringA(std::to_string(dt).c_str());
	OutputDebugStringA("\n");
	ResourceManager::RegisterResource(L"spritesheet", sg.GetSheetTexture());
	shared_ptr<Sprite> sp = make_shared<Sprite>();
	sp->Initialize(AnchorPoint::Center);
	sp->AssignTexture(L"spritesheet");
	sp->SetDimensions(sg.GetSheetTexture()->GetDimensions().x, sg.GetSheetTexture()->GetDimensions().y);
	scene->AddWidget(sp);

	/*shared_ptr<Label> brLabel = make_shared<Label>();
	brLabel->Initialize(AnchorPoint::BottomRight);
	brLabel->SetFont(font);
	brLabel->SetText(L"FFFFFFFF!");
	scene->AddWidget(brLabel);*/

	shared_ptr<Label> fpsLabel = make_shared<Label>();
	fpsLabel->Initialize(AnchorPoint::TopLeft);
	fpsLabel->SetFont(font);
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


	fpsLabel->AddChild(exampleLabel);

	scene->AddWidget(fpsLabel);
	return scene;

}