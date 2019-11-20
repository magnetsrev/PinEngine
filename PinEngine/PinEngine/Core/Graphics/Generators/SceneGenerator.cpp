#include "SceneGenerator.h"
#include "..//UI//CheckBox.h"
#include "..//..//..//Utility//Timer.h"
#include "..//ResourceManager.h"
#include "..//Helpers//BC4TextureGenerator.h"
using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

shared_ptr<Scene> SceneGenerator::GenerateTestScene()
{
	auto scene = make_shared<Scene>();

	shared_ptr<UIFont> font = make_shared<UIFont>();
	bool result = font->LoadFont(L"Data/Fonts/times_new_roman_16.pinfont");

	int width = 256;
	int height = 256;
	vector<uint8_t> uncompressedTextureData(width * height);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			uint8_t brightness = x * 255 / width;
			const int index = (x + y * width);
			uncompressedTextureData[index] = brightness;
		}
	}

	BC4TextureGenerator compressedTextureGenerator;
	compressedTextureGenerator.GenerateFromData(uncompressedTextureData, width, height, false);
	vector<uint8_t> textureData = compressedTextureGenerator.GetCompressedData();

	shared_ptr<Texture> texture = make_shared<Texture>(textureData.data(), DXGI_FORMAT::DXGI_FORMAT_BC4_UNORM, width, height, width * 2);
	ResourceManager::RegisterResource(L"test", texture);


	shared_ptr<Sprite> sp = make_shared<Sprite>();
	sp->Initialize(AnchorPoint::Center);
	sp->AssignTexture(L"test");
	sp->SetDimensions(256, 256);
	scene->AddWidget(sp);

	shared_ptr<Label> brLabel = make_shared<Label>();
	brLabel->Initialize(AnchorPoint::BottomRight);
	brLabel->SetFont(font);
	brLabel->SetText(L"This is anchored at the bottom right!");
	scene->AddWidget(brLabel);

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
				label->SetText(L"'_FPS: |cFF5555FF" + to_wstring(frameCount * (int)updateInterval));
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