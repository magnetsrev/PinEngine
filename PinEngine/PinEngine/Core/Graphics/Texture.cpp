#include "Texture.h"
#include "../../Common/ErrorLogger.h"
#include "../../Utility/StringHelper.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "PipelineManager.h"

namespace PinEngine
{

	Texture::Texture(const Color& color, aiTextureType type)
	{
		Initialize1x1ColorTexture(color, type);
	}

	Texture::Texture(const Color* colorData, UINT width, UINT height, aiTextureType type)
	{
		InitializeColorTexture(colorData, width, height, type);
	}

	Texture::Texture(const std::wstring& filePath, aiTextureType type)
	{
		this->type = type;
		auto device = PipelineManager::GetDevice().Get();

		if (StringHelper::GetFileExtension(filePath) == L".dds")
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFile(device, filePath.c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
			if (FAILED(hr))
			{
				Initialize1x1ColorTexture(Colors::UnloadedTextureColor, type);
				return;
			}
		}
		else
		{
			HRESULT hr = DirectX::CreateWICTextureFromFile(device, filePath.c_str(), nullptr, textureView.GetAddressOf());
			if (FAILED(hr))
			{
				Initialize1x1ColorTexture(Colors::UnloadedTextureColor, type);
				return;
			}
		}
		isValid = true;
	}

	Texture::Texture(const uint8_t* pData, size_t size, aiTextureType type)
	{
		this->type = type;
		auto device = PipelineManager::GetDevice().Get();
		HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, texture.GetAddressOf(), textureView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create Texture from memory.");
	}

	Texture::Texture(const uint8_t* pData, DXGI_FORMAT textureFormat, uint32_t textureWidth, uint32_t textureHeight, uint32_t textureStride)
	{
		type = aiTextureType::aiTextureType_UNKNOWN;
		CD3D11_TEXTURE2D_DESC textureDesc(textureFormat, textureWidth, textureHeight, 1, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_IMMUTABLE);
		CD3D11_SHADER_RESOURCE_VIEW_DESC viewDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureFormat);
		D3D11_SUBRESOURCE_DATA initData = { pData, textureStride, 0 };
		
		auto device = PipelineManager::GetDevice();
		HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, (ID3D11Texture2D**)(texture.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, L"Failed to create Texture from memory.");
		hr = device->CreateShaderResourceView(texture.Get(), &viewDesc, &textureView);
		COM_ERROR_IF_FAILED(hr, L"Failed to create Texture from memory.");
		dimensions.x = textureWidth;
		dimensions.y = textureHeight;
	}

	aiTextureType Texture::GetType()
	{
		return type;
	}

	ID3D11ShaderResourceView* Texture::GetTextureResourceView()
	{
		return textureView.Get();
	}

	ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
	{
		return textureView.GetAddressOf();
	}

	bool Texture::IsValid()
	{
		return isValid;
	}

	DirectX::XMFLOAT2 Texture::GetDimensions()
	{
		return dimensions;
	}

	void Texture::Initialize1x1ColorTexture(const Color& colorData, aiTextureType type)
	{
		InitializeColorTexture(&colorData, 1, 1, type);
	}

	void Texture::InitializeColorTexture(const Color* colorData, UINT width, UINT height, aiTextureType type)
	{
		this->type = type;
		CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

		ID3D11Texture2D* p2DTexture = nullptr;
		D3D11_SUBRESOURCE_DATA initialData{};
		initialData.pSysMem = colorData;
		initialData.SysMemPitch = width * sizeof(Color);

		auto device = PipelineManager::GetDevice().Get();
		HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
		COM_ERROR_IF_FAILED(hr, L"Failed to initialize texture from color data.");
		texture = static_cast<ID3D11Texture2D*>(p2DTexture);
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
		hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create shader resource view from texture generated from color data.");
	}
}