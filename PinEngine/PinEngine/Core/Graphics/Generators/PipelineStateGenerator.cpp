#include "PipelineStateGenerator.h"
#include "..//..//..//Common/ErrorLogger.h"
#include "..//..//..//Utility/BuildPath.h"
#include "..//ResourceManager.h"

namespace PinEngine
{
	bool PipelineStateGenerator::BuildPipelineStates(Microsoft::WRL::ComPtr<ID3D11Device> device)
	{
		this->device = device;
		std::wstring shaderfolder = BuildPath();

		{
			std::shared_ptr<PipelineState> defaultState = std::make_shared<PipelineState>("default");
			//Create depth stencil state
			CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

			HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, &defaultState->depthStencilState);
			defaultState->stencilRef = 0;
			COM_ERROR_IF_FAILED(hr, L"Failed to create depth stencil state.");


			//Create Rasterizer State
			CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
			hr = device->CreateRasterizerState(&rasterizerDesc, &defaultState->rasterizerState);
			COM_ERROR_IF_FAILED(hr, L"Failed to create rasterizer state.");

			std::wstring shaderfolder = BuildPath();

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
			};

			defaultState->vertexShader = std::make_shared<VertexShader>();
			defaultState->pixelShader = std::make_shared<PixelShader>();

			if (!defaultState->vertexShader->Initialize(device, shaderfolder + L"vs.cso", inputLayout))
				return false;
			if (!defaultState->pixelShader->Initialize(device, shaderfolder + L"ps.cso"))
				return false;

			ResourceManager::RegisterResource(L"default", defaultState);
		}




		{
			std::shared_ptr<PipelineState> state_default_2d = std::make_shared<PipelineState>("default_2d");
			//Create depth stencil state
			CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

			HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, &state_default_2d->depthStencilState);
			state_default_2d->stencilRef = 0;
			COM_ERROR_IF_FAILED(hr, L"Failed to create depth stencil state.");


			//Create Rasterizer State
			CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
			//rasterizerDesc.CullMode = D3D11_CULL_NONE;
			hr = device->CreateRasterizerState(&rasterizerDesc, &state_default_2d->rasterizerState);
			COM_ERROR_IF_FAILED(hr, L"Failed to create rasterizer state.");


			//Create sampler description for sampler state
			CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			hr = device->CreateSamplerState(&sampDesc, &state_default_2d->samplerState); //Create sampler state
			COM_ERROR_IF_FAILED(hr, L"Failed to create sampler state.");

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout =
			{
				{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
				{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  }
			};

			state_default_2d->vertexShader = std::make_shared<VertexShader>();
			state_default_2d->pixelShader = std::make_shared<PixelShader>();

			if (!state_default_2d->vertexShader->Initialize(device, shaderfolder + L"vs_2d.cso", inputLayout))
				return false;
			if (!state_default_2d->pixelShader->Initialize(device, shaderfolder + L"ps_2d.cso"))
				return false;

			ResourceManager::RegisterResource(L"default_2d", state_default_2d);
		}

		return true;
	}
}