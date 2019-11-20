#pragma once
#include <vector>
#include <stdint.h>

namespace PinEngine
{
	class BC4TextureGenerator
	{
	public:
		bool GenerateFromData(std::vector<uint8_t> greyscaleData, int width, int height, bool fastGenerate = false);
		std::vector<uint8_t> GetCompressedData();
	private:
		std::vector<uint8_t> compressedData;
	};
}