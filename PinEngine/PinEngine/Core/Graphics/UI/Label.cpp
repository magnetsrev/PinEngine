#include "Label.h"
#include "..//PipelineManager.h"

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
	ForEachGlyph(text.c_str(), [&](UIFont::FontGlyph const* glyph, float x, float y, float advance)
	{
			letterCount += 1;
	});

	vector<Vertex_2D_Texture> verts;
	verts.reserve(letterCount * 6);

	float xAdjust = -textDimensions.x / 2;
	float yAdjust = textDimensions.y / 2;
	
	/*float xAdjust = 0;
	float yAdjust = 0;*/

	float xOffset = 0;
	float yOffset = 0;

	float initialXOffset = 0;

	ForEachGlyph(text.c_str(), [&](UIFont::FontGlyph const* glyph, float x, float y, float advance)
	{
			const float width = glyph->Subrect.right - glyph->Subrect.left;
			const float height = glyph->Subrect.top - glyph->Subrect.bottom;
			const float x_left = x + xAdjust;
			const float x_right = x + width + xAdjust;
			const float y_top = y + yAdjust - glyph->YOffset;
			const float y_bottom = y + height + yAdjust - glyph->YOffset;
			const float u_left = glyph->Subrect.left / font->textureWidth;
			const float u_right = glyph->Subrect.right / font->textureWidth;
			const float v_top = glyph->Subrect.top / font->textureHeight;
			const float v_bottom = glyph->Subrect.bottom / font->textureHeight;


			verts.push_back(Vertex_2D_Texture(x_left, y_top, 1, u_left, v_top));
			verts.push_back(Vertex_2D_Texture(x_right, y_top, 1, u_right, v_top));
			verts.push_back(Vertex_2D_Texture(x_left, y_bottom, 1, u_left, v_bottom));

			verts.push_back(Vertex_2D_Texture(x_left, y_bottom, 1, u_left, v_bottom));
			verts.push_back(Vertex_2D_Texture(x_right, y_top, 1, u_right, v_top));
			verts.push_back(Vertex_2D_Texture(x_right, y_bottom, 1, u_right, v_bottom));

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

void Label::Render()
{
	PipelineManager::SetPipelineState(pipelineState);
	deviceContext->PSSetShaderResources(0, 1, font->texture->GetTextureResourceViewAddress());
	UINT offsets = 0;
	deviceContext->IASetVertexBuffers(0, 1, textVertices.GetAddressOf(), textVertices.StridePtr(), &offsets);
	deviceContext->Draw(textVertices.VertexCount(), 0);


	/*deviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetAddressOf(), vertexBuffer->StridePtr(), &offsets);
	deviceContext->Draw(vertexBuffer->VertexCount(), 0);*/
}

void Label::OnInitialize()
{
	widgetType = WidgetType::Label;
	omitDimensionScalingMatrix = true;
}

void Label::CalculateDimensions()
{
	/*textDimensions.left = 0;
	textDimensions.right = 0;
	textDimensions.top = 0;
	textDimensions.bottom = 0;*/
	XMVECTOR result = XMVectorZero();

	ForEachGlyph(text.c_str(), [&](UIFont::FontGlyph const* glyph, float x, float y, float advance)
	{
		UNREFERENCED_PARAMETER(advance);

		auto w = static_cast<float>(glyph->Subrect.right - glyph->Subrect.left);
		auto h = static_cast<float>(glyph->Subrect.bottom - glyph->Subrect.top) + glyph->YOffset;

		h = std::max(h, font->lineSpacing);

		result = XMVectorMax(result, XMVectorSet(x + w, y + h, 0, 0));
	});
	XMStoreFloat2(&textDimensions, result);
}

template<typename TAction>
inline void Label::ForEachGlyph(wchar_t const* text, TAction action)
{
	float x = 0;
	float y = 0;

	for (; *text; text++)
	{
		wchar_t character = *text;

		switch (character)
		{
		case '\r':
			// Skip carriage returns.
			continue;

		case '\n':
			// New line.
			x = 0;
			y += font->lineSpacing;
			break;

		default:
			// Output this character.
			auto glyph = font->FindGlyph(character);

			if (x>0)
				x += glyph->XOffset;

			if (x < 0)
				x = 0;

			float advance = glyph->Subrect.right - glyph->Subrect.left + glyph->XAdvance;

			if (!iswspace(character)
				|| ((glyph->Subrect.right - glyph->Subrect.left) > 1)
				|| ((glyph->Subrect.bottom - glyph->Subrect.top) > 1))
			{
				action(glyph, x, y, advance);
			}

			x += advance;
			break;
		}
	}
}