#include "UIFont.h"
#include <SpriteFont.h>
#include <fstream>
#include <vector>
#include <d3d11.h>
#include <wrl/client.h>
#include "..//PipelineManager.h"
#include "..//ResourceManager.h"
#include <algorithm>

using namespace PinEngine;
using namespace UI;
using namespace Microsoft::WRL;
using namespace std;

bool UIFont::LoadFont(wstring path)
{
	ifstream fontFile(path.c_str(), ios::binary);
	if (fontFile.is_open())
	{

		struct PinFileHeader
		{
			uint32_t glyphCount = 0;
			float lineSpacing;
			uint32_t defaultCharacter;
			uint32_t textureWidth;
			uint32_t textureHeight;
			DXGI_FORMAT textureFormat;
			uint32_t textureStride;
			uint32_t textureRows;
		} header;
		
		fontFile.read((char*)&header, sizeof(PinFileHeader));
		glyphs.resize(header.glyphCount);
		fontFile.read((char*)glyphs.data(), sizeof(FontGlyph)* glyphs.size());
		unique_ptr<uint8_t[]> textureData = make_unique<uint8_t[]>(header.textureRows * header.textureStride);
		fontFile.read((char*)textureData.get(), header.textureRows* header.textureStride);
		fontFile.close();
		textureWidth = header.textureWidth;
		textureHeight = header.textureHeight;
		lineSpacing = header.lineSpacing;
		defaultGlyph = FindGlyph(header.defaultCharacter);
		if (!ResourceManager::GetResource(path, texture))
		{
			texture = make_shared<Texture>(textureData.get(), header.textureFormat, header.textureWidth, header.textureHeight, header.textureStride);
			ResourceManager::RegisterResource(path, texture);
		}

		return true;
	}
	return false;
}

static inline bool operator< (UIFont::FontGlyph const& left, UIFont::FontGlyph const& right)
{
	return left.Character < right.Character;
}

static inline bool operator< (wchar_t left, UIFont::FontGlyph const& right)
{
	return left < right.Character;
}

static inline bool operator< (UIFont::FontGlyph const& left, wchar_t right)
{
	return left.Character < right;
}

UIFont::FontGlyph * UIFont::FindGlyph(wchar_t character)
{
	for (int i = 0; i < glyphs.size(); i++)
	{
		if (glyphs[i].Character == character)
			return &glyphs[i];
	}

	return defaultGlyph;
}
