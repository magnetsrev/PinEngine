#pragma once
#include "PipelineState.h"
#include <wrl/client.h>
#include "ConstantBuffer.h"
#include <DirectXMath.h>

namespace PinEngine
{
	class PipelineManager
	{
	public:
		static void SetPipelineState(std::shared_ptr<PipelineState> newstate);
		static void RegisterDevice(Microsoft::WRL::ComPtr<ID3D11Device> device);
		static void RegisterContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
		static void RegisterSwapchain(Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain);
		static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext();
		static Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapchain();
		static int GetWidth();
		static int GetHeight();
		static void SetWidth(int width);
		static void SetHeight(int height);
		static void RegisterCameraConstantBuffer(PinEngine::ConstantBuffer<DirectX::XMMATRIX>* cameraConstantBuffer);
		static PinEngine::ConstantBuffer<DirectX::XMMATRIX> * GetCameraConstantBuffer();
	private:
		static int width;
		static int height;
		static std::shared_ptr<PipelineState> state;
		static Microsoft::WRL::ComPtr<ID3D11Device> device;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
		static Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
		static PinEngine::ConstantBuffer<DirectX::XMMATRIX>* cameraConstantBuffer;
	};
}