#pragma once
#include <string>
#include "..//Texture.h"
#include <memory>
#include <vector>


namespace PinEngine
{
	namespace UI
	{
		class UIFont
		{
		public:
			struct FontGlyph
			{
				uint32_t Character;
				RECT Subrect;
				float YOffset;
				float XAdvance; //Currently ignoring this, ||
			};
			std::shared_ptr<Texture> texture = nullptr;
			FontGlyph * FindGlyph(wchar_t character);
			float lineSpacing = 0;
			FontGlyph * defaultGlyph = nullptr;
			float textureWidth = 0;
			float textureHeight = 0;
			std::vector<FontGlyph> glyphs;
		};

		
	}
}