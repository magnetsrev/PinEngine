#pragma once
#include "..//..//Common/ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <vector>

namespace PinEngine
{
	class VertexShader
	{
	public:
		bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc);
		ID3D11VertexShader* GetShader();
		ID3D10Blob* GetBuffer();
		ID3D11InputLayout* GetInputLayout();
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	};

	class PixelShader
	{
	public:
		bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath);
		ID3D11PixelShader* GetShader();
		ID3D10Blob* GetBuffer();
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	};
}