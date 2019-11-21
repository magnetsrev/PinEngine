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

UIFont::FontGlyph* UIFont::FindGlyph(wchar_t character)
{
	for (int i = 0; i < glyphs.size(); i++)
	{
		if (glyphs[i].Character == character)
			return &glyphs[i];
	}

	return defaultGlyph;
}