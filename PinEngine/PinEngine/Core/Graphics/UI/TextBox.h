#pragma once
#include "Label.h"

namespace PinEngine
{
	namespace UI
	{
		class TextBox : public Widget
		{
		public:
			void ProcessChar(wchar_t ch) override;
			void SetMaxCharacters(int maxCharacters);
			void SetText(std::wstring text);
			std::wstring GetText();
			void SetBorderThickness(float thickness);
		private:
			void RenderOverride(DirectX::FXMMATRIX cameraMatrix) override;
			void OnInitialize() override;
			void OnResize() override;
			std::shared_ptr<Label> label = nullptr;
			VertexBuffer<Vertex_2D_PositionColor> borderVertices;
			int maxCharacters = 20;
			float borderThickness = 2;
			int caretPosition = 0;
			std::wstring text = L"";
		};
	}
}