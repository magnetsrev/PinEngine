#pragma once
#include "PipelineState.h"
#include <wrl/client.h>

namespace PinEngine
{
	class PipelineManager
	{
	public:
		static void SetPipelineState(std::shared_ptr<PipelineState> newstate);
		static void RegisterDevice(Microsoft::WRL::ComPtr<ID3D11Device> device);
		static void RegisterContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
		static Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext();

	private:
		static std::shared_ptr<PipelineState> state;
		static Microsoft::WRL::ComPtr<ID3D11Device> device;
		static Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	};
}