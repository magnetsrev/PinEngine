#pragma once
#include <DirectXMath.h>

namespace PinEngine
{
	class EngineObject
	{
	public:
		DirectX::XMVECTOR GetPositionVector();
		DirectX::XMFLOAT3 GetPositionFloat3();
		DirectX::XMVECTOR GetRotationVector();
		DirectX::XMFLOAT3 GetRotationFloat3();

		void SetPosition(const DirectX::XMVECTOR& pos);
		void SetPosition(const DirectX::XMFLOAT3& pos);
		void SetPosition(float x, float y, float z = 0.0f);
		void AdjustPosition(const DirectX::XMVECTOR& pos);
		void AdjustPosition(const DirectX::XMFLOAT3& pos);
		void AdjustPosition(float x, float y, float z);
		void SetRotation(const DirectX::XMVECTOR& rot);
		void SetRotation(const DirectX::XMFLOAT3& rot);
		void SetRotation(float x, float y, float z);
		void AdjustRotation(const DirectX::XMVECTOR& rot);
		void AdjustRotation(const DirectX::XMFLOAT3& rot);
		void AdjustRotation(float x, float y, float z);
		void SetScale(float xScale, float yScale, float zScale = 1.0f);
	protected:
		virtual void UpdateMatrix();

		DirectX::XMVECTOR posVector = { 0,0,0,0 };
		DirectX::XMVECTOR rotVector = { 0,0,0,0 };
		DirectX::XMFLOAT3 pos = { 0, 0, 0 };
		DirectX::XMFLOAT3 rot = { 0, 0, 0 };

		DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	};
}