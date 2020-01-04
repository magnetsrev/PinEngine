#pragma once
#include "Widget.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "UIFont.h"

namespace PinEngine
{
	namespace UI
	{
		class Label : public Widget
		{
		public:
			void SetFont(std::shared_ptr<UIFont> font);
			void SetText(std::wstring text);
			std::wstring GetText();
			void SetFontSize(float fontSize);
			void SetColor(Color color);
			void RenderOverride(DirectX::FXMMATRIX cameraMatrix) override;
			UIFont* GetFont();
			float GetFontSize();
		protected:
			void OnInitialize() override;
		private:
			void CalculateDimensions();
			template<typename TAction>
			inline void ForEachGlyph(wchar_t const* text, TAction action);
			VertexBuffer<Vertex_2D_PositionTexcoordColor> textVertices;
			float fontSize = 16;
			uint32_t letterCount = 0;
			DirectX::XMFLOAT2 textDimensions = { 0, 0 };
			std::wstring text = L"UNINITIALIZED TEXT FIELD";
			Color color;
			std::shared_ptr<UIFont> font = nullptr;
			std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;
			std::unique_ptr<DirectX::SpriteFont> spriteFont = nullptr;
		};
	}
}