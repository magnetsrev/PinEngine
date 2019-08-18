#pragma once
#include "..//EngineObject.h"
#include "VertexBuffer.h"

namespace PinEngine
{
	class RenderableEngineObject2D : public EngineObject
	{
	public:
		bool Initialize();
	private:
		void UpdateMatrix() override;
		VertexBuffer<DirectX::XMFLOAT3> v_positions;
	};
}