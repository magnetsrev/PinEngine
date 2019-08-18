#include "PipelineStateGenerator.h"
#include "..//..//..//Common/ErrorLogger.h"
#include "..//..//..//Utility/BuildPath.h"
#include "..//ResourceManager.h"

namespace PinEngine
{
	bool PipelineStateGenerator::BuildPipelineStates(Microsoft::WRL::ComPtr<ID3D11Device> device)
	{
		this->device = device;

		std::shared_ptr<PipelineState> defaultState = std::make_shared<PipelineState>("default");
		//Create depth stencil state
		CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, &defaultState->depthStencilState);
		defaultState->stencilRef = 0;
		COM_ERROR_IF_FAILED(hr, L"Failed to create depth stencil state.");


		//Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		//rasterizerDesc.CullMode = D3D11_CULL_NONE;
		hr = device->CreateRasterizerState(&rasterizerDesc, &defaultState->rasterizerState);
		COM_ERROR_IF_FAILED(hr, L"Failed to create rasterizer state.");

		ResourceManager::RegisterResource(L"Rasterizer State", defaultState->rasterizerState);

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

		return true;
	}

	std::shared_ptr<PipelineState> PipelineStateGenerator::DefaultPipelineState()
	{
		std::shared_ptr<PipelineState> defaultState = nullptr;
		if (ResourceManager::GetResource(L"default", defaultState))
			return defaultState;
		ErrorLogger::Log(L"Failed to get pipeline state: default.");
		return nullptr; //error occurred...
	}
}