#pragma once
#include "../PipelineState.h"
#include <wrl/client.h>
#include <vector>

namespace PinEngine
{
	class PipelineStateGenerator
	{
	public:
		bool BuildPipelineStates(Microsoft::WRL::ComPtr<ID3D11Device> device);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		std::vector<std::shared_ptr<PipelineState>> pipelineStates;
	};
}