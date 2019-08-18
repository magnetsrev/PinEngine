#include "EngineObject.h"
using namespace DirectX;
namespace PinEngine
{
	XMVECTOR EngineObject::GetPositionVector()
	{
		return posVector;
	}

	XMFLOAT3 EngineObject::GetPositionFloat3()
	{
		return this->pos;
	}

	XMVECTOR EngineObject::GetRotationVector()
	{
		return rotVector;
	}

	XMFLOAT3 EngineObject::GetRotationFloat3()
	{
		return rot;
	}

	void EngineObject::SetPosition(const XMVECTOR& pos)
	{
		XMStoreFloat3(&this->pos, pos);
		posVector = pos;
		UpdateMatrix();
	}

	void EngineObject::SetPosition(const XMFLOAT3& pos)
	{
		this->pos = pos;
		posVector = XMLoadFloat3(&this->pos);
		UpdateMatrix();
	}

	void EngineObject::SetPosition(float x, float y, float z)
	{
		pos = XMFLOAT3(x, y, z);
		posVector = XMLoadFloat3(&pos);
		UpdateMatrix();
	}

	void EngineObject::AdjustPosition(const XMVECTOR& pos)
	{
		posVector += pos;
		XMStoreFloat3(&this->pos, posVector);
		UpdateMatrix();
	}

	void EngineObject::AdjustPosition(const XMFLOAT3& pos)
	{
		this->pos.x += pos.y;
		this->pos.y += pos.y;
		this->pos.z += pos.z;
		posVector = XMLoadFloat3(&this->pos);
		UpdateMatrix();
	}

	void EngineObject::AdjustPosition(float x, float y, float z)
	{
		pos.x += x;
		pos.y += y;
		pos.z += z;
		posVector = XMLoadFloat3(&this->pos);
		UpdateMatrix();
	}

	void EngineObject::SetRotation(const XMVECTOR& rot)
	{
		rotVector = rot;
		XMStoreFloat3(&this->rot, rot);
		UpdateMatrix();
	}

	void EngineObject::SetRotation(const XMFLOAT3& rot)
	{
		this->rot = rot;
		rotVector = XMLoadFloat3(&this->rot);
		UpdateMatrix();
	}

	void EngineObject::SetRotation(float x, float y, float z)
	{
		rot = XMFLOAT3(x, y, z);
		rotVector = XMLoadFloat3(&rot);
		UpdateMatrix();
	}

	void EngineObject::AdjustRotation(const XMVECTOR& rot)
	{
		rotVector += rot;
		XMStoreFloat3(&this->rot, rotVector);
		UpdateMatrix();
	}

	void EngineObject::AdjustRotation(const XMFLOAT3& rot)
	{
		this->rot.x += rot.x;
		this->rot.y += rot.y;
		this->rot.z += rot.z;
		rotVector = XMLoadFloat3(&this->rot);
		UpdateMatrix();
	}

	void EngineObject::AdjustRotation(float x, float y, float z)
	{
		rot.x += x;
		rot.y += y;
		rot.z += z;
		rotVector = XMLoadFloat3(&this->rot);
		UpdateMatrix();
	}

	void EngineObject::SetScale(float xScale, float yScale, float zScale)
	{
		scale.x = xScale;
		scale.y = yScale;
		scale.z = zScale;
		UpdateMatrix();
	}

	void EngineObject::UpdateMatrix()
	{
		assert("UpdateMatrix must be overridden." && 0);
	}
}