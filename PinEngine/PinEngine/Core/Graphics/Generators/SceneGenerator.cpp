#include "SceneGenerator.h"
#include "..//UI//CheckBox.h"
#include "..//..//..//Utility//Timer.h"
#include "..//ResourceManager.h"
#include "..//Helpers//BC4TextureGenerator.h"
#include "..//Helpers//FontManager.h"
#include "..//UI//TextBox.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

shared_ptr<Scene> SceneGenerator::GenerateTestScene()
{
	auto scene = make_shared<Scene>();


	std::shared_ptr<TextBox> tb = std::make_shared<TextBox>();
	tb->Initialize(AnchorPoint::TopRight, nullptr, AnchorPoint::Center);
	tb->SetText(L"Test");
	scene->AddWidget(tb);

	auto font = FontManager::GetFont(L"Times New Roman", 16);
	std::shared_ptr<Sprite> spritesheet = std::make_shared<Sprite>();
	spritesheet->Initialize(AnchorPoint::TopLeft);
	spritesheet->SetDimensions(font->textureWidth, font->textureHeight);
	spritesheet->AssignTexture(font->texture);
	scene->AddWidget(spritesheet);

	return scene;
}

