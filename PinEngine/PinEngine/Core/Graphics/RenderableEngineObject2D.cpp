#include "RenderableEngineObject2D.h"
#include "PipelineManager.h"
#include "ResourceManager.h"
#include "..//Input//InputManager.h"
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;

namespace PinEngine
{
	bool RenderableEngineObject2D::Initialize(AnchorPoint elementAnchor, std::shared_ptr<RenderableEngineObject2D> parent, AnchorPoint parentAnchor)
	{
		if (elementAnchor == AnchorPoint::Uninitialized)
			elementAnchor = AnchorPoint::Center;
		if (parentAnchor == AnchorPoint::Uninitialized)
			parentAnchor = elementAnchor;
		this->elementAnchor = elementAnchor;
		this->parentAnchor = parentAnchor;
		this->parent = parent;

		ComPtr<ID3D11Device> device = PipelineManager::GetDevice();

		if (!ResourceManager::GetResource(L"vb_2d_d", v_positions))
		{
			v_positions = std::make_shared<VertexBuffer<Vertex_2D_Texture>>();

			std::vector<Vertex_2D_Texture> vertices
			{
				Vertex_2D_Texture(-0.5f,	+0.5f, 1.0f, 0, 0), //Top Left
				Vertex_2D_Texture(+0.5f,	+0.5f, 1.0f, 1, 0), //Top Right
				Vertex_2D_Texture(+0.5f,	-0.5f, 1.0f, 1, 1), //Bottom Right
				Vertex_2D_Texture(-0.5f,	+0.5f, 1.0f, 0, 0), //Top Left
				Vertex_2D_Texture(+0.5f,	-0.5f, 1.0f, 1, 1), //Bottom Right
				Vertex_2D_Texture(-0.5f,	-0.5f, 1.0f, 0, 1), //Bottom Left
			};
			COM_ERROR_IF_FAILED(v_positions->Initialize(device, vertices), L"Failed to initialize position vertex buffer for renderable 2d engine object.");
			
			ResourceManager::RegisterResource(L"vb_2d_d", v_positions);
		}

		if (!ResourceManager::GetResource(L"missingtexture", texture))
		{
			ErrorLogger::Log(L"Something went wrong. Missing texture not in resource manager.");
			return false;
		}

		if (!ResourceManager::GetResource(L"default_2d", pipelineState))
			return false;

		UpdateMatrix();
		return true;
	}

	void RenderableEngineObject2D::AddChild(std::shared_ptr<RenderableEngineObject2D> child)
	{
		children.push_back(child);
	}

	std::shared_ptr<Texture> RenderableEngineObject2D::GetTexture()
	{
		return texture;
	}

	void RenderableEngineObject2D::AssignTexture(std::wstring path)
	{
		std::shared_ptr<Texture> temp;
		if (ResourceManager::GetResource(path, temp)) //If found and exists
		{
			texture = temp;
		}
		else //If not found, try to generate
		{
			std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(path);
			if (newTexture->IsValid())
			{
				texture = newTexture;
				ResourceManager::RegisterResource(path, newTexture);
			}
			else //If not valid, attempt to set to the default missingtexture
			{
				if (!ResourceManager::GetResource(L"missingtexture", texture))
				{
					ErrorLogger::Log(L"Something went wrong. Missing texture not in resource manager.");
					return;
				}
			}
		}
	}

	void RenderableEngineObject2D::SetDimensions(float width, float height)
	{
		dimensions.x = width;
		dimensions.y = height;
		UpdateMatrix();
	}

	bool RenderableEngineObject2D::HasChild(std::shared_ptr<RenderableEngineObject2D> child)
	{
		for (auto& c : children)
		{
			if (c == child)
				return true;
		}
		return false;
	}

	void RenderableEngineObject2D::ProcessMouseEvent(MouseEvent mouseEvent)
	{
		if (!mouseInteractionEnabled)
			return;

		MousePoint mousePoint = mouseEvent.GetPos();
		mousePoint.x -= PipelineManager::GetWidth() / 2.0f;
		mousePoint.y -= PipelineManager::GetHeight() / 2.0f;
		mousePoint.y = -mousePoint.y;
		XMMATRIX inverse = XMMatrixInverse(nullptr, worldMatrix);
		XMFLOAT2 mousePointFloat2 = { (float)mousePoint.x, (float)mousePoint.y };
		XMVECTOR mouseVector = XMLoadFloat2(&mousePointFloat2);
		XMVECTOR transformedVec = XMVector2Transform(mouseVector, inverse);
		XMFLOAT2 newMousePoint;
		XMStoreFloat2(&newMousePoint, transformedVec);

		switch (mouseEvent.GetType())
		{
		case MouseEvent::LPress:
		{
			if (processedEventsPerFrame & EventHandlerType::OnLeftClick)
				return;
			if (abs(newMousePoint.x) < 0.5f && abs(newMousePoint.y) <= 0.5f)
			{
				processedEventsPerFrame |= EventHandlerType::OnLeftClick;
				isClicked = true;
				if (isFocusable)
				{
					isFocused = true;
				}
				if (isDragEnabled)
				{
					dragDifference.x = this->pos.x - mouseEvent.GetPosX();
					dragDifference.y = this->pos.y + mouseEvent.GetPosY();
					isDragging = true;
				}
				for (auto& fnc : OnLeftClick.callbacks)
				{
					fnc(this);
				}
			}
			break;
		}
		case MouseEvent::LRelease:
		{
			isDragging = false;
			if (isClicked)
			{
				isClicked = false;
				if (processedEventsPerFrame & EventHandlerType::OnLeftRelease)
					return;
				if (abs(newMousePoint.x) < 0.5f && abs(newMousePoint.y) <= 0.5f)
				{
					processedEventsPerFrame |= EventHandlerType::OnLeftRelease;
					for (auto& fnc : OnLeftRelease.callbacks)
					{
						fnc(this);
					}
				}
			}
			break;
		}
		case MouseEvent::Move:
		{
			if (isDragging && EventHandlerType::OnMouseMove)
			{
				float newPosX = (float)mouseEvent.GetPosX() + this->dragDifference.x;
				float newPosY = -(float)mouseEvent.GetPosY() + this->dragDifference.y;
				if (dragSnapIncrement.x != 0)
				{
					newPosX = newPosX - fmod(newPosX,dragSnapIncrement.x);
				}
				if (dragSnapIncrement.y != 0)
				{
					newPosY = newPosY - fmod(newPosY,dragSnapIncrement.y);

				}
				SetPosition(newPosX, newPosY);
				processedEventsPerFrame |= EventHandlerType::OnMouseMove;
			}
		case MouseEvent::Refresh:
			if (abs(newMousePoint.x) < 0.5f && abs(newMousePoint.y) <= 0.5f)
			{
				if (!mouseOver)
				{
					if (processedEventsPerFrame & EventHandlerType::OnMouseOver)
						return;
					processedEventsPerFrame |= EventHandlerType::OnMouseOver;
					for (auto& fnc : OnMouseOver.callbacks)
					{
						fnc(this);
					}
				}
				mouseOver = true;
			}
			else
			{
				if (mouseOver)
				{
					mouseOver = false;
					if (processedEventsPerFrame & EventHandlerType::OnMouseExit)
						return;
					processedEventsPerFrame |= EventHandlerType::OnMouseExit;
					for (auto& fnc : OnMouseExit.callbacks)
					{
						fnc(this);
					}
				}
			}
			break;
		}
		}
	}

	void RenderableEngineObject2D::OnUpdateTick()
	{
		for (auto& fnc : OnUpdate.callbacks)
		{
			fnc(this);
		}
	}

	void RenderableEngineObject2D::ToggleMouseInteraction(bool isEnabled)
	{
		mouseInteractionEnabled = isEnabled;
	}

	bool RenderableEngineObject2D::IsMouseOver()
	{
		return mouseOver;
	}

	bool RenderableEngineObject2D::IsClicked()
	{
		return isClicked;
	}

	void RenderableEngineObject2D::EnableDrag(bool isEnabled, float xSnap, float ySnap)
	{
		isDragEnabled = isEnabled;
		if (isEnabled)
			ToggleMouseInteraction(true);
		dragSnapIncrement.x = xSnap;
		dragSnapIncrement.y = ySnap;
	}

	bool RenderableEngineObject2D::IsFocused()
	{
		return isFocused;
	}

	void RenderableEngineObject2D::UpdateMatrix()
	{
		float childXOffset(0), childYOffset(0);
		float parentXOffset(0), parentYOffset(0);

		if (parent == nullptr)
		{
			switch (parentAnchor)
			{
			case AnchorPoint::TopLeft:
				parentXOffset = -PipelineManager::GetWidth() / 2;
				parentYOffset = PipelineManager::GetHeight() / 2;
				break;
			case AnchorPoint::TopRight:
				parentXOffset = PipelineManager::GetWidth() / 2;
				parentYOffset = PipelineManager::GetHeight() / 2;
				break;
			case AnchorPoint::Center:

				break;
			case AnchorPoint::BottomLeft:
				parentXOffset = -PipelineManager::GetWidth() / 2;
				parentYOffset = -PipelineManager::GetHeight() / 2;
				break;
			case AnchorPoint::BottomRight:
				parentXOffset = PipelineManager::GetWidth() / 2;
				parentYOffset = -PipelineManager::GetHeight() / 2;
				break;
			}

			switch (elementAnchor)
			{
			case AnchorPoint::TopLeft:
				childXOffset = dimensions.x / 2;
				childYOffset = -dimensions.y / 2;
				break;
			case AnchorPoint::TopRight:
				childXOffset = -dimensions.x / 2;
				childYOffset = -dimensions.y / 2;
				break;
			case AnchorPoint::Center:

				break;
			case AnchorPoint::BottomLeft:
				childXOffset = dimensions.x / 2;
				childYOffset = dimensions.y / 2;
				break;
			case AnchorPoint::BottomRight:
				childXOffset = -dimensions.x / 2;
				childYOffset = dimensions.y / 2;
				break;
			}

			worldMatrix = XMMatrixScaling(dimensions.x * scale.x, dimensions.y * scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z);
			uiChildMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z);

		}
		else
		{
			switch (parentAnchor)
			{
			case AnchorPoint::TopLeft:
				parentXOffset = -parent->dimensions.x / 2;
				parentYOffset = parent->dimensions.y / 2;
				break;
			case AnchorPoint::TopRight:
				parentXOffset = parent->dimensions.x / 2;
				parentYOffset = parent->dimensions.y / 2;
				break;
			case AnchorPoint::Center:

				break;
			case AnchorPoint::BottomLeft:
				parentXOffset = -parent->dimensions.x / 2;
				parentYOffset = -parent->dimensions.y / 2;
				break;
			case AnchorPoint::BottomRight:
				parentXOffset = parent->dimensions.x / 2;
				parentYOffset = -parent->dimensions.y / 2;
				break;
			}

			switch (elementAnchor)
			{
			case AnchorPoint::TopLeft:
				childXOffset = dimensions.x / 2;
				childYOffset = -dimensions.y / 2;
				break;
			case AnchorPoint::TopRight:
				childXOffset = -dimensions.x / 2;
				childYOffset = -dimensions.y / 2;
				break;
			case AnchorPoint::Center:

				break;
			case AnchorPoint::BottomLeft:
				childXOffset = dimensions.x / 2;
				childYOffset = dimensions.y / 2;
				break;
			case AnchorPoint::BottomRight:
				childXOffset = -dimensions.x / 2;
				childYOffset = dimensions.y / 2;
				break;
			}

			worldMatrix = XMMatrixScaling(dimensions.x * scale.x, dimensions.y * scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z) * parent->uiChildMatrix;
			uiChildMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z) * parent->uiChildMatrix;

		}

		for (auto& child : children)
		{
			child->UpdateMatrix();
		}

		if (mouseInteractionEnabled)
		{
			MouseEvent mouseUpdate(MouseEvent::Refresh, InputManager::GetMouse()->GetPosX(), InputManager::GetMouse()->GetPosY());
			ProcessMouseEvent(mouseUpdate);
		}
	}
}
