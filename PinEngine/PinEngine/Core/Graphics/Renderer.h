#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include "..//Input//Mouse.h"
#include "..//Input//Keyboard.h"
#include <memory>
#include "Shaders.h"
#include "VertexBuffer.h"
#include "PipelineManager.h"
#include "Generators//PipelineStateGenerator.h"
#include "Scene.h"
#include "ConstantBuffer.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>

namespace PinEngine
{
	using namespace Microsoft::WRL;
	class Window;

	struct Vertex
	{
		float x, y, z;
	};
	class Renderer
	{
	public:
		bool Initialize(Window* window);
		void Resize(int width, int height);
		void RenderFrame();
		//void Render(RenderableEngineObject& obj);
		std::shared_ptr<Scene> GetActiveScene();
		void SetActiveScene(std::shared_ptr<Scene> scene);
	private:
		bool InitializeDirectX();
		ComPtr<IDXGISwapChain> swapchain;
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;

		ComPtr<ID3D11RenderTargetView> renderTargetView;
		ComPtr<ID3D11Texture2D> depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		PipelineStateGenerator pipelineStateGenerator;
		std::shared_ptr<PipelineState> pipelineState;
		ConstantBuffer<DirectX::XMMATRIX> cb_wvp;
		std::shared_ptr<Mouse> mouse;
		std::shared_ptr<Keyboard> keyboard;
		std::shared_ptr<Scene> currentScene = nullptr;

		Window* parentWindow = nullptr;
		HWND windowHandle = NULL;
		int width = 0;
		int height = 0;
	};
}