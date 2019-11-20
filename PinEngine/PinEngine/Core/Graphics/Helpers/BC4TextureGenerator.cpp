#include "BC4TextureGenerator.h"
#include "..//..//..//Common//ErrorLogger.h"
#include <algorithm>

inline void PrintBytes(uint8_t num)
{
	uint8_t temp;
	temp = (num & 0b10000000) >> 7;
	std::string msg = "Bytes [";
	msg += std::to_string(temp);

	temp = (num & 0b01000000) >> 6;
	msg += std::to_string(temp);

	temp = (num & 0b00100000) >> 5;
	msg += std::to_string(temp);

	temp = (num & 0b00010000) >> 4;
	msg += std::to_string(temp);

	temp = (num & 0b00001000) >> 3;
	msg += std::to_string(temp);

	temp = (num & 0b00000100) >> 2;
	msg += std::to_string(temp);

	temp = (num & 0b00000010) >> 1;
	msg += std::to_string(temp);

	temp = (num & 0b00000001);
	msg += std::to_string(temp);

	msg += "]\r\n";
	OutputDebugStringA(msg.c_str());
}

inline uint8_t FastToBits(const uint8_t input) //assumes red_0 = 255, red_1 = 0
{
	if (input == 255)
		return 0b000;
	if (input >= 218)
		return 0b010;
	if (input >= 182)
		return 0b011;
	if (input >= 145)
		return 0b100;
	if (input >= 109)
		return 0b101;
	if (input >= 72)
		return 0b110;
	if (input >= 36)
		return 0b111;
	return 0b001;
}

inline uint8_t AccurateToBits(const uint8_t input, const uint8_t min, const uint8_t max)
{
	if (input == max)
		return 0b000;
	if (input >= (max*218)/255)
		return 0b010;
	if (input >= (max * 182) / 255)
		return 0b011;
	if (input >= (max * 145) / 255)
		return 0b100;
	if (input >= (max * 109) / 255)
		return 0b101;
	if (input >= (max * 72) / 255)
		return 0b110;
	if (input >= (max * 36) / 255)
		return 0b111;
	return 0b001;
}


bool PinEngine::BC4TextureGenerator::GenerateFromData(std::vector<uint8_t> greyscaleData, int width, int height, bool fastGenerate)
{
	compressedData.clear();
	if ((width % 4) != 0 || (height % 4) != 0)
	{
		ErrorLogger::Log(L"Failed to generate BC4 texture from data. Invalid width/height. Width/height must be multiple of 4.");
		return false;
	}
	if (greyscaleData.size() != (width * height))
	{
		ErrorLogger::Log(L"Failed to generate BC4 texture from data. Greyscale data expects dimensions of width*height.");
		return false;
	}

	compressedData.resize((width * height) / 2);
	memset(compressedData.data(), 0, compressedData.size());

	int stride = width * 2;
	if (fastGenerate)
	{
		for (int y = 0; y < height; y += 4)
		{
			for (int x = 0; x < width; x += 4)
			{
				int yOffset = y * stride / 4;
				int xOffset = x * 2;
				int comboOffset = xOffset + yOffset;
				uint8_t red_0 = 255;
				uint8_t red_1 = 0;
				compressedData[comboOffset + 0] = red_0;
				compressedData[comboOffset + 1] = red_1;
				for (int i = 0; i < 2; i++) //process each 4x2 block
				{
					int blockOffset = (i*2)*width + y * width + x;
					uint8_t aIndex = greyscaleData[blockOffset + 0];
					uint8_t bIndex = greyscaleData[blockOffset + 1];
					uint8_t cIndex = greyscaleData[blockOffset + 2];
					uint8_t dIndex = greyscaleData[blockOffset + 3];
					uint8_t eIndex = greyscaleData[blockOffset + 0 + width];
					uint8_t fIndex = greyscaleData[blockOffset + 1 + width];
					uint8_t gIndex = greyscaleData[blockOffset + 2 + width];
					uint8_t hIndex = greyscaleData[blockOffset + 3 + width];
					uint8_t b1(0), b2(0), b3(0);
					const uint8_t a = FastToBits(aIndex);
					const uint8_t b = FastToBits(bIndex);
					const uint8_t c = FastToBits(cIndex);
					const uint8_t d = FastToBits(dIndex);
					const uint8_t e = FastToBits(eIndex);
					const uint8_t f = FastToBits(fIndex);
					const uint8_t g = FastToBits(gIndex);
					const uint8_t h = FastToBits(hIndex);

					b1 = a | (b << 3) | ((c & 0b011) << 6);
					b2 = (c >> 2) | (d << 1) | (e << 4) | ((f & 0b1) << 7);
					b3 = (f >> 1) | (g << 2) | (h << 5);

					int compressedBlockOffset = comboOffset + 2 + (3 * i);
					compressedData[compressedBlockOffset + 0] = b1;
					compressedData[compressedBlockOffset + 1] = b2;
					compressedData[compressedBlockOffset + 2] = b3;
				}
			}
		}
	}
	else
	{ 
		for (int y = 0; y < height; y += 4)
		{
			for (int x = 0; x < width; x += 4)
			{
				//First get max & min values for this 4x4
				uint8_t red_0 = 0;
				uint8_t red_1 = 255;
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						uint8_t pixelValue = greyscaleData[i + x + (j+y) * width];
						red_0 = std::max(red_0, pixelValue);
						red_1 = std::min(red_1, pixelValue);
					}
				}


				int yOffset = y * stride / 4;
				int xOffset = x * 2;
				int comboOffset = xOffset + yOffset;
				compressedData[comboOffset + 0] = red_0;
				compressedData[comboOffset + 1] = red_1;
				for (int i = 0; i < 2; i++) //process each 4x2 block
				{
					int blockOffset = (i*2)*width + y * width + x;
					uint8_t aIndex = greyscaleData[blockOffset + 0];
					uint8_t bIndex = greyscaleData[blockOffset + 1];
					uint8_t cIndex = greyscaleData[blockOffset + 2];
					uint8_t dIndex = greyscaleData[blockOffset + 3];
					uint8_t eIndex = greyscaleData[blockOffset + 0 + width];
					uint8_t fIndex = greyscaleData[blockOffset + 1 + width];
					uint8_t gIndex = greyscaleData[blockOffset + 2 + width];
					uint8_t hIndex = greyscaleData[blockOffset + 3 + width];
					uint8_t b1(0), b2(0), b3(0);
					const uint8_t a = AccurateToBits(aIndex, red_1, red_0);
					const uint8_t b = AccurateToBits(bIndex, red_1, red_0);
					const uint8_t c = AccurateToBits(cIndex, red_1, red_0);
					const uint8_t d = AccurateToBits(dIndex, red_1, red_0);
					const uint8_t e = AccurateToBits(eIndex, red_1, red_0);
					const uint8_t f = AccurateToBits(fIndex, red_1, red_0);
					const uint8_t g = AccurateToBits(gIndex, red_1, red_0);
					const uint8_t h = AccurateToBits(hIndex, red_1, red_0);

					b1 = a | (b << 3) | ((c & 0b011) << 6);
					b2 = (c >> 2) | (d << 1) | (e << 4) | ((f & 0b1) << 7);
					b3 = (f >> 1) | (g << 2) | (h << 5);

					int compressedBlockOffset = comboOffset + 2 + (3 * i);
					compressedData[compressedBlockOffset + 0] = b1;
					compressedData[compressedBlockOffset + 1] = b2;
					compressedData[compressedBlockOffset + 2] = b3;
				}
			}
		}
	}
	
	
	return true;
}

std::vector<uint8_t> PinEngine::BC4TextureGenerator::GetCompressedData()
{
	return compressedData;
}
