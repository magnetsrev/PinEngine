#include "PipelineManager.h"

namespace PinEngine
{
	void PipelineManager::SetPipelineState(std::shared_ptr<PipelineState> newstate)
	{
		if (state != newstate)
		{
			if (state == nullptr)
			{
				state = newstate;
				deviceContext->RSSetState(newstate->rasterizerState.Get());
				deviceContext->PSSetSamplers(0, 1, newstate->samplerState.GetAddressOf());
				deviceContext->OMSetDepthStencilState(newstate->depthStencilState.Get(), newstate->stencilRef);
				deviceContext->IASetInputLayout(newstate->vertexShader->GetInputLayout());
				deviceContext->VSSetShader(newstate->vertexShader->GetShader(), nullptr, 0);
				deviceContext->PSSetShader(newstate->pixelShader->GetShader(), nullptr, 0);
				return;
			}
			if (state->rasterizerState != newstate->rasterizerState)
			{
				deviceContext->RSSetState(newstate->rasterizerState.Get());
			}
			if (state->samplerState != newstate->samplerState)
			{
				deviceContext->PSSetSamplers(0, 1, newstate->samplerState.GetAddressOf());
			}
			if (state->depthStencilState != newstate->depthStencilState)
			{
				deviceContext->OMSetDepthStencilState(newstate->depthStencilState.Get(), newstate->stencilRef);
			}
			if (state->vertexShader != newstate->vertexShader)
			{
				deviceContext->IASetInputLayout(newstate->vertexShader->GetInputLayout());
				deviceContext->VSSetShader(newstate->vertexShader->GetShader(), nullptr, 0);
			}
			if (state->pixelShader != newstate->pixelShader)
			{
				deviceContext->PSSetShader(newstate->pixelShader->GetShader(), nullptr, 0);
			}
			state = newstate;
		}
	}

	void PipelineManager::RegisterDevice(Microsoft::WRL::ComPtr<ID3D11Device> device)
	{
		PipelineManager::device = device;
	}

	void PipelineManager::RegisterContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
	{
		PipelineManager::deviceContext = deviceContext;
	}

	void PipelineManager::RegisterSwapchain(Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain)
	{
		PipelineManager::swapchain = swapchain;
	}

	Microsoft::WRL::ComPtr<ID3D11Device> PipelineManager::GetDevice()
	{
		return PipelineManager::device;
	}

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> PipelineManager::GetDeviceContext()
	{
		return PipelineManager::deviceContext;
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain> PipelineManager::GetSwapchain()
	{
		return PipelineManager::swapchain;
	}

	int PipelineManager::GetWidth()
	{
		return width;
	}

	int PipelineManager::GetHeight()
	{
		return height;
	}

	void PipelineManager::SetWidth(int width)
	{
		PipelineManager::width = width;
	}

	void PipelineManager::SetHeight(int height)
	{
		PipelineManager::height = height;
	}

	void PipelineManager::RegisterCameraConstantBuffer(PinEngine::ConstantBuffer<DirectX::XMMATRIX>* cameraConstantBuffer)
	{
		PipelineManager::cameraConstantBuffer = cameraConstantBuffer;
	}

	PinEngine::ConstantBuffer<DirectX::XMMATRIX>* PipelineManager::GetCameraConstantBuffer()
	{
		return PipelineManager::cameraConstantBuffer;
	}

	std::shared_ptr<PipelineState> PipelineManager::state = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> PipelineManager::device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> PipelineManager::deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> PipelineManager::swapchain;
	int PipelineManager::width = 0;
	int PipelineManager::height = 0;
	PinEngine::ConstantBuffer<DirectX::XMMATRIX>* PipelineManager::cameraConstantBuffer = nullptr;
}