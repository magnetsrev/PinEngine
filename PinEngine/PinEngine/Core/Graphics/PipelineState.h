#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Shaders.h"
#include <memory>

namespace PinEngine
{
	class PipelineState
	{
	public:
		PipelineState() {}
		PipelineState(std::string name)
		{
			id = name;
		}
		std::string id = "";
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
		UINT stencilRef = 0;
		std::shared_ptr<VertexShader> vertexShader = nullptr;
		std::shared_ptr<PixelShader> pixelShader = nullptr;
	};
}