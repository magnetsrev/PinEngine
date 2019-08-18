#include "RenderableEngineObject2D.h"
#include "PipelineManager.h"
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;

namespace PinEngine
{
	bool RenderableEngineObject2D::Initialize()
	{
		ComPtr<ID3D11Device> device = PipelineManager::GetDevice();

		std::vector<XMFLOAT3> positions
		{
			{	-1.0f,	+1.0f, 0.0f	}, //Top Left
			{	+1.0f,	+1.0f, 0.0f	}, //Top Right
			{	+1.0f,	-1.0f, 0.0f	}, //Bottom Right
			{	-1.0f,	-1.0f, 0.0f	}, //Bottom Left
		};
		COM_ERROR_IF_FAILED(v_positions.Initialize(device, positions), L"Failed to initialize position vertex buffer for renderable 2d engine object.");

		return true;
	}

	void RenderableEngineObject2D::UpdateMatrix()
	{

	}
}
