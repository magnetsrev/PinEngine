#pragma once
#include "..//Scene.h"

namespace PinEngine
{
	class SceneGenerator
	{
	public:
		static std::shared_ptr<Scene> GenerateTestScene();
	};
}