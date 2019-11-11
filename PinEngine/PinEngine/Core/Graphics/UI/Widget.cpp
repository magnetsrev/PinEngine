#include "Widget.h"
#include "..//PipelineManager.h"
#include "..//ResourceManager.h"
#include "..//..//Input//InputManager.h"

using namespace DirectX;
using namespace PinEngine;
using namespace PinEngine::UI;

bool Widget::Initialize(AnchorPoint elementAnchor, Widget* parent, AnchorPoint parentAnchor)
{
	if (elementAnchor == AnchorPoint::Uninitialized)
		elementAnchor = AnchorPoint::Center;
	if (parentAnchor == AnchorPoint::Uninitialized)
		parentAnchor = elementAnchor;
	this->elementAnchor = elementAnchor;
	this->parentAnchor = parentAnchor;
	this->parent = parent;
	deviceContext = PipelineManager::GetDeviceContext().Get();

	if (!ResourceManager::GetResource(L"2d_widget_default", vertexBuffer))
	{
		vertexBuffer = std::make_shared<VertexBuffer<PinEngine::Vertex_2D_Texture>>();
				
		std::vector<PinEngine::Vertex_2D_Texture> vertices
		{
			Vertex_2D_Texture(-0.5f,	+0.5f, 1.0f, 0, 0), //Top Left
			Vertex_2D_Texture(+0.5f,	+0.5f, 1.0f, 1, 0), //Top Right
			Vertex_2D_Texture(+0.5f,	-0.5f, 1.0f, 1, 1), //Bottom Right
			Vertex_2D_Texture(-0.5f,	+0.5f, 1.0f, 0, 0), //Top Left
			Vertex_2D_Texture(+0.5f,	-0.5f, 1.0f, 1, 1), //Bottom Right
			Vertex_2D_Texture(-0.5f,	-0.5f, 1.0f, 0, 1), //Bottom Left
		};
		COM_ERROR_IF_FAILED(vertexBuffer->Initialize(PipelineManager::GetDevice(), vertices), L"Failed to initialize vertex buffer for 2d widget.");

		ResourceManager::RegisterResource(L"2d_widget_default", vertexBuffer);
	}


	if (!ResourceManager::GetResource(L"default_2d", pipelineState))
		return false;

	OnInitialize();

	assert(widgetType != WidgetType::Unknown, "The custom widget's OnInitialize() function needs to assign widgetType to something other than WidgetType::Unknown");
	
	UpdateMatrix();
	return true;
}

void Widget::AddChild(std::shared_ptr<Widget> child)
{
	if (std::find(children.begin(), children.end(), child) != children.end())
	{
		//If child is already in children vector, don't do anything
	}
	else
	{
		children.push_back(child);
	}
}

bool Widget::HasChild(std::shared_ptr<Widget> child)
{
	for (auto& c : children)
	{
		if (c == child)
			return true;
	}
	return false;
}

void Widget::SetDimensions(float width, float height)
{
	dimensions.x = width;
	dimensions.y = height;
	UpdateMatrix();
}

void Widget::RenderOverride(FXMMATRIX cameraMatrix)
{
	assert("RenderOverride must be overridden." && 0);
}

void Widget::OnInitialize()
{
	assert("OnInitialize should be overridden with a function to initialize the custom widgetType." && 0);
}

void Widget::Render(FXMMATRIX cameraMatrix)
{
	for (auto& c : children)
	{
		c->Render(cameraMatrix);
	}
	RenderOverride(cameraMatrix);
}

void Widget::ProcessMouseEvent(MouseEvent mouseEvent)
{
	if (!mouseInteractionEnabled)
		return;
		
	for (auto& child : children)
	{
		child->ProcessMouseEvent(mouseEvent);
	}

	MousePoint mousePoint = mouseEvent.GetPos();
	mousePoint.x -= PipelineManager::GetWidth() / 2;
	mousePoint.y -= PipelineManager::GetHeight() / 2;
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
		if (abs(newMousePoint.x) < 0.5f && abs(newMousePoint.y) <= 0.5f)
		{
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
			if (abs(newMousePoint.x) < 0.5f && abs(newMousePoint.y) <= 0.5f)
			{
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
		}
	case MouseEvent::Refresh:
		if (abs(newMousePoint.x) < 0.5f && abs(newMousePoint.y) <= 0.5f)
		{
			if (!mouseOver)
			{
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

void Widget::ToggleMouseInteraction(bool isEnabled)
{
	mouseInteractionEnabled = isEnabled;
}

bool Widget::IsMouseOver()
{
	return mouseOver;
}

bool Widget::IsClicked()
{
	return isClicked;
}

void Widget::EnableDrag(bool isEnabled, float xSnap, float ySnap)
{
	isDragEnabled = isEnabled;
	if (isEnabled)
		ToggleMouseInteraction(true);
	dragSnapIncrement.x = xSnap;
	dragSnapIncrement.y = ySnap;
}

Widget* PinEngine::UI::Widget::GetParent()
{
	return parent;
}

void Widget::UpdateMatrix()
{
	float childXOffset(0), childYOffset(0);
	float parentXOffset(0), parentYOffset(0);

	if (parent == nullptr)
	{
		switch (parentAnchor)
		{
		case AnchorPoint::TopLeft:
			parentXOffset = -static_cast<float>(PipelineManager::GetWidth()) / 2.0f;
			parentYOffset = PipelineManager::GetHeight() / 2;
			break;
		case AnchorPoint::TopRight:
			parentXOffset = static_cast<float>(PipelineManager::GetWidth()) / 2.0f;
			parentYOffset = PipelineManager::GetHeight() / 2;
			break;
		case AnchorPoint::Center:

			break;
		case AnchorPoint::BottomLeft:
			parentXOffset = -static_cast<float>(PipelineManager::GetWidth()) / 2.0f;
			parentYOffset = -PipelineManager::GetHeight() / 2;
			break;
		case AnchorPoint::BottomRight:
			parentXOffset = static_cast<float>(PipelineManager::GetWidth()) / 2.0f;
			parentYOffset = -static_cast<float>(PipelineManager::GetHeight()) / 2.0f;
			break;
		}

		switch (elementAnchor)
		{
		case AnchorPoint::TopLeft:
			childXOffset = dimensions.x*scale.x / 2;
			childYOffset = -dimensions.y*scale.y / 2;
			break;
		case AnchorPoint::TopRight:
			childXOffset = -dimensions.x * scale.x / 2;
			childYOffset = -dimensions.y * scale.y / 2;
			break;
		case AnchorPoint::Center:

			break;
		case AnchorPoint::BottomLeft:
			childXOffset = dimensions.x * scale.x / 2;
			childYOffset = dimensions.y * scale.y / 2;
			break;
		case AnchorPoint::BottomRight:
			childXOffset = -dimensions.x * scale.x / 2;
			childYOffset = dimensions.y * scale.y / 2;
			break;
		}

		if (omitDimensionScalingMatrix)
		{
			worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z);
			uiChildMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z);
		}
		else
		{
			worldMatrix = XMMatrixScaling(dimensions.x * scale.x, dimensions.y * scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z);
			uiChildMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z);
		}

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
			childXOffset = dimensions.x * scale.x / 2;
			childYOffset = -dimensions.y * scale.y / 2;
			break;
		case AnchorPoint::TopRight:
			childXOffset = -dimensions.x * scale.x / 2;
			childYOffset = -dimensions.y * scale.y / 2;
			break;
		case AnchorPoint::Center:

			break;
		case AnchorPoint::BottomLeft:
			childXOffset = dimensions.x * scale.x / 2;
			childYOffset = dimensions.y * scale.y / 2;
			break;
		case AnchorPoint::BottomRight:
			childXOffset = -dimensions.x * scale.x / 2;
			childYOffset = dimensions.y * scale.y / 2;
			break;
		}
		/*switch (elementAnchor)
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
		}*/
		if (omitDimensionScalingMatrix)
		{
			worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z) * parent->uiChildMatrix;
			uiChildMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z) * parent->uiChildMatrix;
		}
		else
		{
			worldMatrix = XMMatrixScaling(dimensions.x * scale.x, dimensions.y * scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z) * parent->uiChildMatrix;
			uiChildMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixTranslation(childXOffset, childYOffset, 0) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(parentXOffset + pos.x, parentYOffset + pos.y, pos.z) * parent->uiChildMatrix;
		}
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

void Widget::OnUpdateTick()
{
	for (auto& fnc : OnUpdate.callbacks)
	{
		fnc(this);
	}
}
