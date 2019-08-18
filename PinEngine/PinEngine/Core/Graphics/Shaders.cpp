#include "Shaders.h"

namespace PinEngine
{
	bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc)
	{
		HRESULT hr = D3DReadFileToBlob(
			shaderpath.c_str(), 
			shader_buffer.GetAddressOf());

		if (FAILED(hr))
		{
			std::wstring errorMsg = L"Failed to load shader: ";
			errorMsg += shaderpath;
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}

		hr = device->CreateVertexShader(
			shader_buffer->GetBufferPointer(), 
			shader_buffer->GetBufferSize(), 
			NULL, 
			&shader);

		if (FAILED(hr))
		{
			std::wstring errorMsg = L"Failed to create vertex shader: ";
			errorMsg += shaderpath;
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}

		hr = device->CreateInputLayout(
			layoutDesc.data(), 
			layoutDesc.size(),
			shader_buffer->GetBufferPointer(), 
			shader_buffer->GetBufferSize(), 
			&inputLayout);

		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, L"Failed to create input layout.");
			return false;
		}

		return true;
	}

	ID3D11VertexShader* VertexShader::GetShader()
	{
		return shader.Get();
	}

	ID3D10Blob* VertexShader::GetBuffer()
	{
		return shader_buffer.Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout()
	{
		return inputLayout.Get();
	}

	bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
	{
		HRESULT hr = D3DReadFileToBlob(
			shaderpath.c_str(), 
			&shader_buffer);

		if (FAILED(hr))
		{
			std::wstring errorMsg = L"Failed to load shader: ";
			errorMsg += shaderpath;
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}

		hr = device->CreatePixelShader(
			shader_buffer.Get()->GetBufferPointer(), 
			shader_buffer.Get()->GetBufferSize(), 
			NULL, 
			&shader);

		if (FAILED(hr))
		{
			std::wstring errorMsg = L"Failed to create pixel shader: ";
			errorMsg += shaderpath;
			ErrorLogger::Log(hr, errorMsg);
			return false;
		}

		return true;
	}

	ID3D11PixelShader* PixelShader::GetShader()
	{
		return shader.Get();
	}

	ID3D10Blob* PixelShader::GetBuffer()
	{
		return shader_buffer.Get();
	}
}