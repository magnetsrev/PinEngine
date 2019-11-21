#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include "..//Texture.h"

namespace PinEngine
{
	class SpritesheetGenerator
	{
	public:
		SpritesheetGenerator()
		{
		}
		class ParseGlyph
		{
		public:
			Gdiplus::Bitmap* bitmap = nullptr;
			int charWidth = 0;
			int charHeight = 0;
			int xOffset = 0;
			int yOffset = 0;
			wchar_t character = 0;
			int drawOffsetX = 0;
			int drawOffsetY = 0;
		};
		bool GenerateSheet(std::wstring fontName, float fontSize);
		std::shared_ptr<Texture> GetSheetTexture();
		std::vector<ParseGlyph> glyphs;
		float lineSpacing = 0;

	private:
		ParseGlyph GenerateGlyphFromCharacter(wchar_t character);
		float PointsToPixels(float fontPoints);
		std::shared_ptr<Texture> texture = nullptr;
		std::unique_ptr<Gdiplus::Bitmap> bitmap;
		std::unique_ptr<Gdiplus::Graphics> gfx;

		std::unique_ptr<Gdiplus::FontFamily> fontFamily;
		std::unique_ptr<Gdiplus::Font> font;

		Gdiplus::Brush* brush = nullptr;
	};
}