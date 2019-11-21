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
#include <memory>
#include <unordered_map>
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
				int charHeight = 0;
				int xOffset = 0;
				int yOffset = 0;
				wchar_t character = 0;
				int drawOffsetX = 0;
				int drawOffsetY = 0;
			};
			static std::shared_ptr<UIFont> GenerateFont(std::wstring fontAlias, float fontPoint);
			static ParseGlyph GenerateGlyphFromCharacter(wchar_t character, Gdiplus::Graphics* gfx, Gdiplus::Bitmap* bitmap, Gdiplus::Font* font, Gdiplus::Brush* brush);
			struct hash_pair 
			{
				template <class T1, class T2>
				size_t operator()(const std::pair<T1, T2>& p) const
				{
					auto hash1 = std::hash<T1>{}(p.first);
					auto hash2 = std::hash<T2>{}(p.second);
					return hash1 ^ hash2;
				}
			};
			static std::unordered_map<std::pair<std::wstring, float>, std::shared_ptr<UIFont>, hash_pair> fonts;
			static bool initialized;
		};
	}
}