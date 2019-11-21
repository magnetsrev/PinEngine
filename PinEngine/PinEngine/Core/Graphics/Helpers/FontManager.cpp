#include "FontManager.h"
#include "..//..//..//Common//ErrorLogger.h"
#include "BC4TextureGenerator.h"

namespace PinEngine
{
	namespace UI
	{
		float PointsToPixels(float points)
		{
			return points * (96.0f / 72.0f);
		}

		FontManager::FontManager()
		{
			initialized = true;
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR           gdiplusToken;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		}

		std::shared_ptr<UIFont> FontManager::GetFont(std::wstring fontAlias, float fontPoint)
		{
			auto iterator = FontManager::fonts.find(std::pair<std::wstring, float>(fontAlias, fontPoint));
			if (iterator != FontManager::fonts.end())
			{
				return iterator->second;
			}

			std::shared_ptr<UIFont> font = GenerateFont(fontAlias, fontPoint);
			FontManager::fonts[std::pair<std::wstring, float>(fontAlias, fontPoint)] = font;

			return font;
		}

		std::shared_ptr<UIFont> FontManager::GenerateFont(std::wstring fontAlias, float fontPoint)
		{
			std::shared_ptr<UIFont> generatedFont = std::make_shared<UIFont>();

			std::unique_ptr<Gdiplus::FontFamily> fontFamily = std::make_unique<Gdiplus::FontFamily>(fontAlias.c_str());
			std::unique_ptr<Gdiplus::Font> font = std::make_unique<Gdiplus::Font>(fontFamily.get(), PointsToPixels(fontPoint), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			if (!fontFamily->IsAvailable() || !font->IsAvailable())
			{
				//If not available, try to default to times new roman
				fontFamily = std::make_unique<Gdiplus::FontFamily>(L"Times New Roman");
				font = std::make_unique<Gdiplus::Font>(fontFamily.get(), PointsToPixels(fontPoint), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
				if (!fontFamily->IsAvailable() || !font->IsAvailable())
				{
					ErrorLogger::Log(L"A critical error happened when trying to generate Times New Roman font. Is the PC missing Times New Roman? Application must close. Please report this error to developers.");
					abort();
				}
			}

			
			int pixelSize = PointsToPixels(fontPoint) * 4 / 3;
			int dimensions = 32;
			while (dimensions < pixelSize)
			{
				dimensions <<= 1;
			}

			std::unique_ptr<Gdiplus::Bitmap> bitmap = std::make_unique<Gdiplus::Bitmap>(dimensions, dimensions);
			std::unique_ptr<Gdiplus::Graphics> gfx = std::make_unique<Gdiplus::Graphics>(bitmap.get());
			std::unique_ptr<Gdiplus::Brush> brush = std::unique_ptr<Gdiplus::Brush>(Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255)).Clone());
			gfx->SetPixelOffsetMode(Gdiplus::PixelOffsetMode::PixelOffsetModeHighQuality);
			gfx->SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);
			gfx->SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAliasGridFit);
			float lineSpacing = font->GetHeight(gfx.get());
			
			std::vector<FontManager::ParseGlyph> glyphs;
			glyphs.reserve('~' - ' ' + 1);
			for (wchar_t letter = ' '; letter <= '~'; letter++)
			{
				glyphs.push_back(std::move(GenerateGlyphFromCharacter(letter, gfx.get(), bitmap.get(), font.get(), brush.get())));
			}

			for (int i = 0; i < (glyphs.size() - 1); i++)
			{
				for (int j = i + 1; j < glyphs.size(); j++)
				{
					if (glyphs[i].charWidth < glyphs[j].charWidth)
						std::swap(glyphs[i], glyphs[j]);
				}
			}

			int absoluteminY = 250;
			int glyphArea = 0;
			int maxGlyphWidth = 0;
			for (auto& g : glyphs)
			{
				absoluteminY = std::min(absoluteminY, g.yOffset);
				maxGlyphWidth = std::max(maxGlyphWidth, g.charWidth);
				glyphArea += (g.charHeight * g.charWidth);
			}
			maxGlyphWidth = std::max(maxGlyphWidth, (int)sqrt(glyphArea));

			int bitmapWidth = 2;
			while (bitmapWidth < maxGlyphWidth)
			{
				bitmapWidth <<= 1;
			}


			int requiredWidth = maxGlyphWidth;
			int requiredHeight = 0;
			for (int i = 0; i < glyphs.size(); i++)
			{
				requiredHeight = std::max(requiredHeight, glyphs[i].charHeight);
			}

			std::vector<uint8_t> pixelData;
			int fixedWidth = 32;
			while (fixedWidth < requiredWidth)
			{
				fixedWidth <<= 1;
			}
			int fixedHeight = fixedWidth;
			pixelData.resize(fixedWidth * fixedHeight, 0);

			int xoffset = 0;
			int yoffset = 0;
			int maxY = 0;
			for (auto& glyph : glyphs)
			{
				if (xoffset + glyph.charWidth > fixedWidth)
				{
					xoffset = 0;
					yoffset += maxY;
					maxY = 0;
				}
				maxY = std::max(maxY, glyph.charHeight);

				if ((yoffset + glyph.charHeight) > fixedHeight)
				{
					fixedHeight <<= 1;
					pixelData.resize(fixedWidth * fixedHeight, 0);
				}

				glyph.drawOffsetX = xoffset;
				glyph.drawOffsetY = yoffset;
				Gdiplus::BitmapData bitmapData;
				const Gdiplus::Rect glyphRect(glyph.xOffset, glyph.yOffset, glyph.charWidth, glyph.charHeight);
				glyph.bitmap->LockBits(&glyphRect, Gdiplus::ImageLockMode::ImageLockModeRead | Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
				for (int y = 0; y < bitmapData.Height; y++)
				{
					for (int x = 0; x < bitmapData.Width; x++)
					{
						union PixColor
						{
							BYTE rgba[4];
						} pixelColor;
						PixColor* pixelPtr = ((PixColor*)bitmapData.Scan0 + ((y)*bitmapData.Stride / 4 + (x)));
						if (pixelPtr->rgba[3] > 0)
						{
							pixelData[x + xoffset + (yoffset + y) * fixedWidth] = pixelPtr->rgba[3];
						}
					}
				}
				glyph.bitmap->UnlockBits(&bitmapData);
				xoffset += bitmapData.Width;
			}
			for (auto& glyph : glyphs)
			{
				glyph.yOffset -= absoluteminY;
			}

			BC4TextureGenerator textureGen;
			textureGen.GenerateFromData(pixelData, fixedWidth, fixedHeight);
			std::vector<uint8_t> compressedData = textureGen.GetCompressedData();
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(compressedData.data(), DXGI_FORMAT::DXGI_FORMAT_BC4_UNORM, fixedWidth, fixedHeight, fixedWidth * 2);
			
			for (auto& g : glyphs)
			{
				UIFont::FontGlyph glyph;
				glyph.Character = g.character;
				RECT area;
				area.left = g.drawOffsetX;
				area.top = g.drawOffsetY;
				area.bottom = g.drawOffsetY + g.charHeight;
				area.right = g.drawOffsetX + g.charWidth;
				glyph.Subrect = area;
				glyph.YOffset = g.yOffset;
				glyph.XAdvance = 0;
				glyph.XOffset = 0;
				generatedFont->glyphs.push_back(glyph);
			}
			generatedFont->lineSpacing = lineSpacing;
			generatedFont->textureHeight = texture->GetDimensions().y;
			generatedFont->textureWidth = texture->GetDimensions().x;
			generatedFont->defaultGlyph = generatedFont->FindGlyph('?');
			generatedFont->texture = texture;

			fonts[std::pair<std::wstring, float>(fontAlias, fontPoint)] = generatedFont;
			return generatedFont;
		}

		FontManager::ParseGlyph FontManager::GenerateGlyphFromCharacter(wchar_t character, Gdiplus::Graphics * gfx, Gdiplus::Bitmap * bitmap, Gdiplus::Font * font, Gdiplus::Brush * brush)
		{
			Gdiplus::RectF rect;
			Gdiplus::StringFormat sf;
			sf.SetFormatFlags(Gdiplus::StringFormatFlags::StringFormatFlagsNoFontFallback);

			std::wstring charString = L"";
			charString += character;
			gfx->MeasureString(charString.c_str(), 1, font, Gdiplus::PointF(), &sf, &rect);
			int charWidth = ceil(rect.Width);
			int charHeight = ceil(rect.Height);

			gfx->Clear(Gdiplus::Color(0, 0, 0));
			gfx->DrawString(charString.c_str(), 1, font, Gdiplus::PointF(0, 0), brush);
			gfx->Flush();

			const Gdiplus::Rect glyphRect(0, 0, charWidth, charHeight);

			Gdiplus::Bitmap* glyphBitmap = bitmap->Clone(glyphRect, PixelFormat32bppARGB);
			Gdiplus::BitmapData bitmapData;
			glyphBitmap->LockBits(&glyphRect, Gdiplus::ImageLockMode::ImageLockModeRead | Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
			int actualWidth = 0;
			int actualHeight = 0;
			int minX = 999999;
			int minY = 999999;
			for (int y = 0; y < bitmapData.Height; y++)
			{
				for (int x = 0; x < bitmapData.Width; x++)
				{
					union PixColor
					{
						BYTE rgba[4];
					} pixelColor;
					PixColor* pixelPtr = ((PixColor*)bitmapData.Scan0 + (y * bitmapData.Stride / 4 + x));
					pixelPtr->rgba[3] = (pixelPtr->rgba[0] + pixelPtr->rgba[1] + pixelPtr->rgba[2]) / 3; //alpha = rgb average
					if (pixelPtr->rgba[3] > 0)
					{
						minX = std::min(minX, x);
						minY = std::min(minY, y);
						actualWidth = std::max(actualWidth, x - minX);
						actualHeight = std::max(actualHeight, y - minY);
					}
				}
			}

			if (actualWidth <= 0 || actualHeight <= 0)
			{
				actualHeight = 0;
				actualWidth = charWidth;
				minY = 0;
				minX = 0;
			}
			else
			{
				if (minX > 0) minX -= 1;
				if (minY > 0) minY -= 1;
				actualWidth += 2;
				actualHeight += 2;
			}
			glyphBitmap->UnlockBits(&bitmapData);
			ParseGlyph glyph;
			glyph.bitmap = std::unique_ptr<Gdiplus::Bitmap>(glyphBitmap);
			glyph.character = character;
			glyph.charHeight = actualHeight;
			glyph.charWidth = actualWidth;
			glyph.xOffset = minX;
			glyph.yOffset = minY;
			return glyph;
		}

		bool FontManager::initialized = false;
		std::unordered_map<std::pair<std::wstring, float>, std::shared_ptr<UIFont>, FontManager::hash_pair> FontManager::fonts;
		namespace priv
		{
			FontManager fg;
		}
	}
}
