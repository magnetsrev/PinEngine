#include "Label.h"
#include "..//PipelineManager.h"
#include "..//ResourceManager.h"
#include "..//..//..//Common//ErrorLogger.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;
using namespace DirectX;

void Label::SetFont(std::shared_ptr<UIFont> font)
{
	this->font = font;
}

void Label::SetText(std::wstring text)
{
	this->text = text;
	CalculateDimensions();

	letterCount = 0;
	ForEachGlyph(text.c_str(), [&](UIFont::FontGlyph const* glyph, float x, float y, float advance, Color charColor)
	{
			letterCount += 1;
	});

	vector<Vertex_2D_PositionTexcoordColor> verts;
	verts.reserve((size_t)letterCount * 6);

	float xAdjust = -textDimensions.x / 2;
	float yAdjust = textDimensions.y / 2;
	
	/*float xAdjust = 0;
	float yAdjust = 0;*/

	float xOffset = 0;
	float yOffset = 0;

	float initialXOffset = 0;
	


	/*
	float g = (float)color.GetG() / 255.0f;
	float b = (float)color.GetB() / 255.0f;
	float a = (float)color.GetA() / 255.0f;*/
	ForEachGlyph(text.c_str(), [&](UIFont::FontGlyph const* glyph, float x, float y, float advance, Color charColor)
	{
			const float width = glyph->Subrect.right - glyph->Subrect.left;
			const float height = glyph->Subrect.top - glyph->Subrect.bottom;
			const float x_left = x + xAdjust;
			const float x_right = x + width + xAdjust;
			const float y_top = y + yAdjust - glyph->YOffset;
			const float y_bottom = y + height + yAdjust - glyph->YOffset;
			/*const float y_top = y + yAdjust;
			const float y_bottom = y + height + yAdjust;*/
			const float u_left = glyph->Subrect.left / font->textureWidth;
			const float u_right = glyph->Subrect.right / font->textureWidth;
			const float v_top = glyph->Subrect.top / font->textureHeight;
			const float v_bottom = glyph->Subrect.bottom / font->textureHeight;

			float r = (float)charColor.rgba[0] / 255.0f;
			float g = (float)charColor.rgba[1] / 255.0f;
			float b = (float)charColor.rgba[2] / 255.0f;
			float a = (float)charColor.rgba[3] / 255.0f;

			verts.push_back(Vertex_2D_PositionTexcoordColor(x_left, y_top, 1, u_left, v_top, r, g, b, a));
			verts.push_back(Vertex_2D_PositionTexcoordColor(x_right, y_top, 1, u_right, v_top, r, g, b, a));
			verts.push_back(Vertex_2D_PositionTexcoordColor(x_left, y_bottom, 1, u_left, v_bottom, r, g, b, a));

			verts.push_back(Vertex_2D_PositionTexcoordColor(x_left, y_bottom, 1, u_left, v_bottom, r, g, b, a));
			verts.push_back(Vertex_2D_PositionTexcoordColor(x_right, y_top, 1, u_right, v_top, r, g, b, a));
			verts.push_back(Vertex_2D_PositionTexcoordColor(x_right, y_bottom, 1, u_right, v_bottom, r, g, b, a));

	});

	textVertices.Initialize(PipelineManager::GetDevice(), verts);

	SetDimensions(textDimensions.x, textDimensions.y);
}

wstring Label::GetText()
{
	return text;
}

void Label::SetFontSize(float fontSize)
{
	this->fontSize = fontSize;
}

void PinEngine::UI::Label::SetColor(Color color)
{
	this->color = color;
	SetText(text);
}

void Label::RenderOverride(DirectX::FXMMATRIX cameraMatrix)
{
	if (textVertices.VertexCount() > 0)
	{
		auto cb_wvp = PipelineManager::GetCameraConstantBuffer();
		cb_wvp->data = worldMatrix * cameraMatrix;
		cb_wvp->ApplyChanges();
		PipelineManager::SetPipelineState(pipelineState);
		deviceContext->PSSetShaderResources(0, 1, font->texture->GetTextureResourceViewAddress());
		UINT offsets = 0;
		deviceContext->IASetVertexBuffers(0, 1, textVertices.GetAddressOf(), textVertices.StridePtr(), &offsets);
		deviceContext->Draw(textVertices.VertexCount(), 0);
	}

	/*deviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetAddressOf(), vertexBuffer->StridePtr(), &offsets);
	deviceContext->Draw(vertexBuffer->VertexCount(), 0);*/
}

UIFont* PinEngine::UI::Label::GetFont()
{
	return font.get();
}

float PinEngine::UI::Label::GetFontSize()
{
	return fontSize;
}

void Label::OnInitialize()
{
	widgetType = WidgetType::Label;
	omitDimensionScalingMatrix = true;
	if (!ResourceManager::GetResource(L"default_2d_text", pipelineState))
	{
		ErrorLogger::Log(L"Failed to load the default text pipeline state for Label widget.");
	}
	color = Colors::DefaultTextColor;
}

void Label::CalculateDimensions()
{
	/*textDimensions.left = 0;
	textDimensions.right = 0;
	textDimensions.top = 0;
	textDimensions.bottom = 0;*/
	XMVECTOR result = XMVectorZero();

	ForEachGlyph(text.c_str(), [&](UIFont::FontGlyph const* glyph, float x, float y, float advance, Color charColor)
	{
		//UNREFERENCED_PARAMETER(advance);

		auto w = static_cast<float>(glyph->Subrect.right - glyph->Subrect.left);
		auto h = static_cast<float>(glyph->Subrect.bottom - glyph->Subrect.top) + glyph->YOffset;

		h = std::max(h, font->lineSpacing);

		result = XMVectorMax(result, XMVectorSet(x + w, -y + h, 0, 0));
	});
	XMStoreFloat2(&textDimensions, result);
}

unsigned int HexToUnsignedInt(std::wstring hex)
{
	assert(hex.length() == 8);
	char hexstr[9];
	for (int i = 0; i < 8; i++)
	{
		hexstr[7 - i] = hex[i];
	}
	for (int i = 0; i < 8; i += 2)
	{
		swap(hexstr[i], hexstr[i + 1]);
	}
	hexstr[8] = 0;
	unsigned long hex_value
		= std::strtoul(hexstr, 0, 16);
	return hex_value;
}

template<typename TAction>
inline void Label::ForEachGlyph(wchar_t const* text, TAction action)
{
	float x = 0;
	float y = 0;
	wchar_t prevchar = 0;
	wchar_t character = 0;
	Color charColor = color;
	bool isSpecialColor = false;
	bool isColorSequence = false;
	std::wstring colorhex = L"";
	int remainingHexChars = 0;
	for (; *text; text++)
	{
		prevchar = character;
		character = *text;

		switch (character)
		{
		case '\r':
			// Skip carriage returns.
			continue;

		case '\n':
			// New line.
			x = 0;
			y -= font->lineSpacing;
			break;

		case '|':
			if (prevchar != character)
			{
				continue;
			}
		default:
			
			if (character == 'c')
			{
				if (prevchar == '|')
				{
					isColorSequence = true;
					remainingHexChars = 8;
					continue;
				}
			}
			if (character == 'r')
			{
				if (prevchar == '|')
				{
					isSpecialColor = false;
					charColor = color;
					continue;
				}
			}
			if (isColorSequence)
			{
				prevchar = 0;
				remainingHexChars -= 1;
				colorhex += character;
				if (remainingHexChars == 0)
				{
					isColorSequence = false;
					isSpecialColor = true;
					Color newColor;
					newColor.color = HexToUnsignedInt(colorhex);
					charColor = newColor;
					colorhex = L"";
				}
				continue;
			}
			// Output this character.
			auto glyph = font->FindGlyph(character);

			if (x < 0)
				x = 0;

			float advance = glyph->Subrect.right - glyph->Subrect.left + glyph->XAdvance;

			if (!iswspace(character)
				|| ((glyph->Subrect.right - glyph->Subrect.left) > 1)
				|| ((glyph->Subrect.bottom - glyph->Subrect.top) > 1))
			{
				action(glyph, x, y, advance, charColor);
				if (character == '|')
					character = 0;
			}

			x += advance;
			break;
		}
	}
}