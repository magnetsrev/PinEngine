#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Color.h"
#include <assimp/material.h>

namespace PinEngine
{
	enum class TextureStorageType
	{
		Invalid,
		None,
		EmbeddedIndexCompressed,
		EmbeddedIndexNonCompressed,
		EmbeddedCompressed,
		EmbeddedNonCompressed,
		Disk
	};

	class Texture
	{
	public:
		Texture(const Color& color, aiTextureType type);
		Texture(const Color* colorData, UINT width, UINT height, aiTextureType type); //Generate texture of specific color data
		Texture(const std::wstring& filePath, aiTextureType type = aiTextureType::aiTextureType_UNKNOWN);
		Texture(const uint8_t* pData, size_t size, aiTextureType type = aiTextureType::aiTextureType_UNKNOWN);
		Texture(const uint8_t* pData, DXGI_FORMAT textureFormat, uint32_t textureWidth, uint32_t textureHeight, uint32_t textureStride);

		aiTextureType GetType();
		ID3D11ShaderResourceView* GetTextureResourceView();
		ID3D11ShaderResourceView** GetTextureResourceViewAddress();
		bool IsValid();

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;

		void Initialize1x1ColorTexture(const Color& colorData, aiTextureType type);
		void InitializeColorTexture(const Color* colorData, UINT width, UINT height, aiTextureType type);
		Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
		aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
		bool isValid = false;
	};
}