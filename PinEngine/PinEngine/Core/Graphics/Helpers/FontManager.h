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
#include "HashPair.h"
#include "..//UI//UIFont.h"

namespace PinEngine
{
	namespace UI
	{
		class FontManager
		{
		public:
			FontManager();
			static std::shared_ptr<UIFont> GetFont(std::wstring fontAlias, float fontPoint);

		private:
			class ParseGlyph
			{
			public:
				std::unique_ptr<Gdiplus::Bitmap> bitmap = nullptr;
				int charWidth = 0;
				int suggestedCharWidth = 0;
				int charHeight = 0;
				int xOffset = 0;
				int yOffset = 0;
				wchar_t character = 0;
				int drawOffsetX = 0;
				int drawOffsetY = 0;
			};
			static std::shared_ptr<UIFont> GenerateFont(std::wstring fontAlias, float fontPoint);
			static ParseGlyph GenerateGlyphFromCharacter(wchar_t character, Gdiplus::Graphics* gfx, Gdiplus::Bitmap* bitmap, Gdiplus::Font* font, Gdiplus::Brush* brush);
			
			static std::unordered_map<std::pair<std::wstring, float>, std::shared_ptr<UIFont>, HashPair> fonts;
			static bool initialized;
		};
	}
}