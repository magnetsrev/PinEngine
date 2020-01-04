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
		Vertex_2D_Texture() {}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoords;
	};
	class Vertex_2D_PositionTexcoordColor
	{
	public:
		Vertex_2D_PositionTexcoordColor(float x, float y, float z, float u, float v, float r, float g, float b, float a)
		{
			position.x = x;
			position.y = y;
			position.z = z;
			texcoords.x = u;
			texcoords.y = v;
			color.x = r;
			color.y = g;
			color.z = b;
			color.w = a;
		}
		Vertex_2D_PositionTexcoordColor() {}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoords;
		DirectX::XMFLOAT4 color;
	};
	class Vertex_2D_PositionColor
	{
	public:
		Vertex_2D_PositionColor(float x, float y, float z, float r, float g, float b, float a)
		{
			position.x = x;
			position.y = y;
			position.z = z;
			color.x = r;
			color.y = g;
			color.z = b;
			color.w = a;
		}
		Vertex_2D_PositionColor() {}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}