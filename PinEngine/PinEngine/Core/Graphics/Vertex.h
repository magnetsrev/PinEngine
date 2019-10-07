#pragma once
#include <DirectXMath.h>

namespace PinEngine
{
	class Vertex_2D_Texture
	{
	public:
		Vertex_2D_Texture(float x, float y, float z, float u, float v)
		{
			position.x = x;
			position.y = y;
			position.z = z;
			texcoords.x = u;
			texcoords.y = v;
		}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoords;
	};
}