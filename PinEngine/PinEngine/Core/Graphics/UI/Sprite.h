#pragma once
#include "Widget.h"
#include "../Texture.h"

namespace PinEngine
{
	namespace UI
	{
		class Sprite : public Widget
		{
		public:
			std::shared_ptr<Texture> GetTexture();
			void AssignTexture(std::wstring path);
			void AssignTexture(std::shared_ptr<Texture> texture);
			void RenderOverride(DirectX::FXMMATRIX cameraMatrix) override;
		protected:
			void OnInitialize() override;
		private:
			std::shared_ptr<Texture> texture = nullptr;
		};
	}
}