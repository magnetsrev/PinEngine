#pragma once
#include "..//..//EngineObject.h"
#include "AnchorPoint.h"
#include "..//VertexBuffer.h"
#include "../PipelineState.h"
#include "WidgetType.h"
#include "..//..//Event.h"
#include "..//..//Input//MouseEvent.h"
#include "..//..//Input//KeyboardEvent.h"
#include <memory>
#include <vector>
#include <d3d11.h>

namespace PinEngine
{
	class Scene;
	class Renderer;
	namespace UI
	{
		class Widget : public EngineObject
		{
			friend class Renderer;
			friend class Scene;
		public:
			bool Initialize(AnchorPoint elementAnchor = AnchorPoint::Uninitialized, Widget* parent = nullptr, AnchorPoint parentAnchor = AnchorPoint::Uninitialized);
			void AddChild(std::shared_ptr<Widget> child);
			bool HasChild(std::shared_ptr<Widget> child);
			void SetDimensions(float width, float height);
			void ProcessMouseEvent(MouseEvent mouseEvent);
			virtual void ProcessChar(wchar_t ch);
			void ToggleMouseInteraction(bool isEnabled);
			bool IsMouseOver();
			bool IsClicked();
			void Render(DirectX::FXMMATRIX cameraMatrix);
			void EnableDrag(bool isEnabled, float xSnap=0, float ySnap=0);
			Widget* GetParent();
			Event<Widget> OnMouseOver;
			Event<Widget> OnMouseExit;
			Event<Widget> OnUpdate;
			Event<Widget> OnLeftClick;
			Event<Widget> OnLeftRelease;
		protected:
			virtual void RenderOverride(DirectX::FXMMATRIX cameraMatrix);

			virtual void OnInitialize(); //This will be called after default initialize runs for classes inheriting widget
			void UpdateMatrix() override;
			WidgetType widgetType = WidgetType::Unknown;
			ID3D11DeviceContext* deviceContext = nullptr;
			Widget* parent = nullptr;
			std::shared_ptr<VertexBuffer<Vertex_2D_Texture>> vertexBuffer = nullptr;
			std::vector<std::shared_ptr<Widget>> children;
			std::shared_ptr<PipelineState> pipelineState = nullptr;
			DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX uiChildMatrix = DirectX::XMMatrixIdentity();
			AnchorPoint elementAnchor = AnchorPoint::TopLeft;
			AnchorPoint parentAnchor = AnchorPoint::TopLeft;
			DirectX::XMFLOAT3 offsetToParent = { 0, 0, 0 };
			DirectX::XMFLOAT2 dimensions = { 1, 1 };
			bool omitDimensionScalingMatrix = false;

		private:
			void OnUpdateTick();
			bool mouseOver = false;
			bool mouseInteractionEnabled = false;
			bool isClicked = false;
			bool isDragging = false;
			bool isDragEnabled = false;
			DirectX::XMFLOAT2 dragDifference = { 0, 0 };
			DirectX::XMFLOAT2 dragSnapIncrement = { 0,0 };
			bool isFocusable = true;
			bool isFocused = false;
		};
	}
}