#pragma once
#include "..//EngineObject.h"
#include "VertexBuffer.h"
#include "PipelineState.h"
#include "AnchorPoint.h"
#include "Texture.h"
#include "Vertex.h"
#include "../Input/Mouse.h"
#include "../Event.h"
#include <SimpleMath.h>

namespace PinEngine
{
	class RenderableEngineObject2D : public EngineObject
	{
		friend class Renderer;
		friend class Scene;
	public:
		bool Initialize(AnchorPoint elementAnchor = AnchorPoint::Uninitialized, std::shared_ptr<RenderableEngineObject2D> parent = nullptr, AnchorPoint parentAnchor = AnchorPoint::Uninitialized);
		void AddChild(std::shared_ptr<RenderableEngineObject2D> child);
		std::shared_ptr<Texture> GetTexture();
		void AssignTexture(std::wstring path);
		void SetDimensions(float width, float height);
		bool HasChild(std::shared_ptr<RenderableEngineObject2D> child);
		void ProcessMouseEvent(MouseEvent mouseEvent);
		void OnUpdateTick();
		void ToggleMouseInteraction(bool isEnabled);
		bool IsMouseOver();
		bool IsClicked();
		void EnableDrag(bool isEnabled, float xSnap=0, float ySnap=0);
		bool IsFocused();
		Event<RenderableEngineObject2D> OnMouseOver;
		Event<RenderableEngineObject2D> OnMouseExit;
		Event<RenderableEngineObject2D> OnUpdate;
		Event<RenderableEngineObject2D> OnLeftClick;
		Event<RenderableEngineObject2D> OnLeftRelease;
	private:
		void UpdateMatrix() override;
		std::shared_ptr<VertexBuffer<Vertex_2D_Texture>> v_positions;
		std::shared_ptr<PipelineState> pipelineState;
		DirectX::XMMATRIX worldMatrix = { 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0 };
		DirectX::XMMATRIX uiChildMatrix = { 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0 };

		AnchorPoint elementAnchor = AnchorPoint::TopLeft;
		AnchorPoint parentAnchor = AnchorPoint::TopLeft;
		DirectX::XMFLOAT3 offsetToParent = { 0, 0, 0 };
		DirectX::XMFLOAT2 dimensions = { 1, 1 };
		std::shared_ptr<Texture> texture = nullptr;
		std::shared_ptr<RenderableEngineObject2D> parent = nullptr;
		std::vector<std::shared_ptr<RenderableEngineObject2D>> children;
		bool mouseOver = false;
		bool mouseInteractionEnabled = false;
		int processedEventsPerFrame = 0;
		bool isClicked = false;
		bool isDragging = false;
		bool isDragEnabled = false;
		DirectX::XMFLOAT2 dragDifference = { 0, 0 };
		DirectX::XMFLOAT2 dragSnapIncrement = { 0,0 };
		bool isFocusable = true;
		bool isFocused = false;
	};
}