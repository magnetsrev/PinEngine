#pragma once
#include "../PipelineState.h"
#include <wrl/client.h>
#include <vector>

namespace PinEngine
{
	class PipelineStateGenerator
	{
	public:
		enum PipelineStateSlots
		{
			Default = 0
		};
		bool BuildPipelineStates(Microsoft::WRL::ComPtr<ID3D11Device> device);
		std::shared_ptr<PipelineState> DefaultPipelineState();
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		std::vector<std::shared_ptr<PipelineState>> pipelineStates;
	};
}