#include "TextBox.h"
#include "..//Helpers//FontManager.h"
#include "..//PipelineManager.h"
#include "..//ResourceManager.h"

namespace PinEngine
{
	namespace UI
	{
		void TextBox::ProcessChar(wchar_t ch)
		{
			std::wstring text = label->GetText();

			switch (ch)
			{
			case 0x8: //backspace
				if (text.size() > 0) //This is rigged up, but pretty much we're not going to allow multicolored text codes with pipes in textboxes for now. Will come back to this later.
				{
					auto lastCharIndex = text.size() - 1;
					if (text[lastCharIndex] == '|')
					{
						text = text.substr(0, text.size() - 2);
					}
					else
					{
						text = text.substr(0, text.size() - 1);
					}
					label->SetText(text);
				}
				break;
			case 0x03: //Copy
				break;
			case 0x16: //Paste
				break;
			case 0x18: //Cut
				break;
			case 0x1B: //Escape
				break;
			case '|':
				if (text.size() > maxCharacters)
				{
					break;
				}
				text += L"||";
				label->SetText(text);
				break;
			default:
				if (text.size() > maxCharacters)
				{
					break;
				}
				text += ch;
				label->SetText(text);
				break;
			}
		}

		void TextBox::SetMaxCharacters(int maxCharacters)
		{
			this->maxCharacters = maxCharacters;
		}

		void TextBox::SetText(std::wstring text)
		{
			this->text = text;
			label->SetText(text);
		}

		std::wstring TextBox::GetText()
		{
			return text;
		}

		void TextBox::SetBorderThickness(float thickness)
		{
			borderThickness = thickness;
			OnResize();
		}

		void TextBox::RenderOverride(DirectX::FXMMATRIX cameraMatrix)
		{
			if (borderVertices.VertexCount() > 0)
			{
				auto cb_wvp = PipelineManager::GetCameraConstantBuffer();
				cb_wvp->data = worldMatrix * cameraMatrix;
				cb_wvp->ApplyChanges();
				PipelineManager::SetPipelineState(pipelineState);
				UINT offsets = 0;
				deviceContext->IASetVertexBuffers(0, 1, borderVertices.GetAddressOf(), borderVertices.StridePtr(), &offsets);
				deviceContext->Draw(borderVertices.VertexCount(), 0);
			}
		}

		void TextBox::OnInitialize()
		{
			omitDimensionScalingMatrix = true;
			ResourceManager::GetResource(L"2d_color", pipelineState);
			widgetType = WidgetType::TextBox;
			label = std::make_shared<Label>();
			label->Initialize(AnchorPoint::TopLeft, this);
			auto font = FontManager::GetFont(L"Times New Roman", 16);
			label->SetFont(font);
			label->SetText(L"TEMPORARY");
			AddChild(label);
			SetDimensions(100, font->lineSpacing + borderThickness * 2);
		}

		void TextBox::OnResize()
		{
			float lineSpacing = label->GetFont()->lineSpacing;
			float width = this->dimensions.x;
			float height = this->dimensions.y;
			label->SetPosition(borderThickness + label->GetFontSize()/4.0f, -(height - lineSpacing) / 2.0f);


			float outerLeftX = -width / 2.0f;
			float innerLeftX = -width / 2.0f + borderThickness;
			float outerRightX = width / 2.0f;
			float innerRightX = width / 2.0f - borderThickness;
			float outerTopY = height / 2.0f;
			float innerTopY = height / 2.0f - borderThickness;
			float outerBottomY = -height / 2.0f;
			float innerBottomY = -height / 2.0f + borderThickness;
			std::vector<Vertex_2D_PositionColor> borderVerts =
			{
				//TOP BORDER																				____
				Vertex_2D_PositionColor(outerLeftX, outerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		|  /
				Vertex_2D_PositionColor(outerRightX, outerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		| /
				Vertex_2D_PositionColor(outerLeftX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		|/
																									//		
				Vertex_2D_PositionColor(outerLeftX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		   /|
				Vertex_2D_PositionColor(outerRightX, outerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		  / |
				Vertex_2D_PositionColor(outerRightX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		 /__|

				//BOTTOM BORDER																				____
				Vertex_2D_PositionColor(outerLeftX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		|  /
				Vertex_2D_PositionColor(outerRightX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		| /
				Vertex_2D_PositionColor(outerLeftX, outerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		|/
																									//
				Vertex_2D_PositionColor(outerLeftX, outerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		   /|
				Vertex_2D_PositionColor(outerRightX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		  / |
				Vertex_2D_PositionColor(outerRightX, outerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		 /__|

				//LEFT BORDER                                                                       //      ____
				Vertex_2D_PositionColor(outerLeftX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		|  /
				Vertex_2D_PositionColor(innerLeftX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		| /
				Vertex_2D_PositionColor(outerLeftX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		|/
																									//
				Vertex_2D_PositionColor(outerLeftX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		   /|
				Vertex_2D_PositionColor(innerLeftX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		  / |
				Vertex_2D_PositionColor(innerLeftX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		 /__|

				//RIGHT BORDER                                                                      //      ____
				Vertex_2D_PositionColor(innerRightX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		|  /
				Vertex_2D_PositionColor(outerRightX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		| /
				Vertex_2D_PositionColor(innerRightX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		|/
																									//
				Vertex_2D_PositionColor(innerRightX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		   /|
				Vertex_2D_PositionColor(outerRightX, innerTopY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),		//		  / |
				Vertex_2D_PositionColor(outerRightX, innerBottomY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),	//		 /__|
			};
			borderVertices.Initialize(PipelineManager::GetDevice(), borderVerts);
		}

	}
}

