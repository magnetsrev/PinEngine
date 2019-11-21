#include "SpritesheetGenerator.h"
#include "BC4TextureGenerator.h"
#include "..//..//..//Utility//Timer.h"

namespace PinEngine
{
	bool SpritesheetGenerator::GenerateSheet(std::wstring fontName, float fontSize)
	{
		fontFamily = std::make_unique<Gdiplus::FontFamily>(fontName.c_str());
		font = std::make_unique<Gdiplus::Font>(fontFamily.get(), PointsToPixels(fontSize), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		if (!fontFamily->IsAvailable() || !font->IsAvailable())
		{
			//If not available, try to default to times new roman
			fontFamily = std::make_unique<Gdiplus::FontFamily>(L"Times New Roman");
			font = std::make_unique<Gdiplus::Font>(fontFamily.get(), PointsToPixels(fontSize), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			if (!fontFamily->IsAvailable() || !font->IsAvailable())
			{
				return false;
			}
		}

		lineSpacing = font->GetHeight(gfx.get());
		int pixelSize = PointsToPixels(fontSize) * 4/3;
		int dimensions = 32;
		while (dimensions < pixelSize)
		{
			dimensions <<= 1;
		}

		bitmap = std::make_unique<Gdiplus::Bitmap>(dimensions, dimensions);
		gfx = std::make_unique<Gdiplus::Graphics>(bitmap.get());
		brush = Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255)).Clone();
		gfx->SetPixelOffsetMode(Gdiplus::PixelOffsetMode::PixelOffsetModeHighQuality);
		gfx->SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);
		gfx->SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAliasGridFit);

		glyphs.reserve('~' - ' ' + 1);
		for (wchar_t letter = ' '; letter <= '~'; letter++)
		{
			glyphs.push_back(GenerateGlyphFromCharacter(letter));
		}

		for (int i = 0; i < (glyphs.size()-1); i++)
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
					PixColor* pixelPtr = ((PixColor*)bitmapData.Scan0 + ((y) * bitmapData.Stride / 4 + (x)));
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
		texture = std::make_shared<Texture>(compressedData.data(), DXGI_FORMAT::DXGI_FORMAT_BC4_UNORM, fixedWidth, fixedHeight, fixedWidth*2);

		return true;
	}

	std::shared_ptr<Texture> SpritesheetGenerator::GetSheetTexture()
	{
		return texture;
	}

	SpritesheetGenerator::ParseGlyph SpritesheetGenerator::GenerateGlyphFromCharacter(wchar_t character)
	{
		Gdiplus::RectF rect;
		Gdiplus::StringFormat sf;
		sf.SetFormatFlags(Gdiplus::StringFormatFlags::StringFormatFlagsNoFontFallback);

		std::wstring charString = L"";
		charString += character;
		gfx->MeasureString(charString.c_str(), 1, font.get(), Gdiplus::PointF(), &sf, &rect);
		int charWidth = ceil(rect.Width);
		int charHeight = ceil(rect.Height);


		gfx->Clear(Gdiplus::Color(0, 0, 0));
		gfx->DrawString(charString.c_str(), 1, font.get(), Gdiplus::PointF(0, 0), brush);
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
			if (minX>0) minX -= 1;
			if (minY>0) minY -= 1;
			actualWidth += 2;
			actualHeight += 2;
		}
		glyphBitmap->UnlockBits(&bitmapData);
		ParseGlyph glyph;
		glyph.bitmap = glyphBitmap;
		glyph.character = character;
		glyph.charHeight = actualHeight;
		glyph.charWidth = actualWidth;
		glyph.xOffset = minX;
		glyph.yOffset = minY;
		return glyph;
	}

	float SpritesheetGenerator::PointsToPixels(float points)
	{
		return points * (96.0f / 72.0f);
	}
}