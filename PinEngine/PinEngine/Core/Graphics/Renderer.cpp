#include "Renderer.h"
#include "../Window.h"
#include "..//..//Utility/BuildPath.h"
#include "VertexBuffer.h"

namespace PinEngine
{
	bool Renderer::Initialize(Window* window)
	{
		mouse = window->mouse;
		keyboard = window->keyboard;
		parentWindow = window;
		height = window->height;
		width = window->width;

		InitializeDirectX();

		return false;
	}

	void Renderer::Resize(int width, int height)
	{
	}

	void Renderer::RenderFrame()
	{
		float backgroundColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->ClearRenderTargetView(renderTargetView.Get(), backgroundColor);
		deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		PipelineManager::SetPipelineState(pipelineState);


		UINT offsets = 0;
		deviceContext->IASetVertexBuffers(0, 1, test.GetAddressOf(), test.StridePtr(), &offsets);
		deviceContext->Draw(3, 0);

		swapchain->Present(1, NULL);
	}

	bool Renderer::InitializeDirectX()
	{
		try
		{
			DXGI_SWAP_CHAIN_DESC scd = { 0 };

			scd.BufferDesc.Width = width;
			scd.BufferDesc.Height = height;
			scd.BufferDesc.RefreshRate.Numerator = 60;
			scd.BufferDesc.RefreshRate.Denominator = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;

			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.BufferCount = 1; //double buffered by default in windowed mode
			scd.OutputWindow = parentWindow->GetHWND();
			scd.Windowed = TRUE;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			HRESULT hr;
			hr = D3D11CreateDeviceAndSwapChain(nullptr, //IDXGI Adapter
				D3D_DRIVER_TYPE_HARDWARE,
				0, //FOR SOFTWARE DRIVER TYPE
				0, //FLAGS FOR RUNTIME LAYERS
				nullptr, //FEATURE LEVELS ARRAY
				0, //# OF FEATURE LEVELS IN ARRAY
				D3D11_SDK_VERSION,
				&scd, //Swapchain description
				&swapchain, //Swapchain Address
				&device, //Device Address
				NULL, //Supported feature level
				&deviceContext); //Device Context Address

			COM_ERROR_IF_FAILED(hr, L"Failed to create device and swapchain.");

			PipelineManager::RegisterContext(deviceContext);
			pipelineStateGenerator.BuildPipelineStates(device);

			Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
			hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
			COM_ERROR_IF_FAILED(hr, L"GetBuffer failed.");

			hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
			COM_ERROR_IF_FAILED(hr, L"Failed to create render target view.");

			//Describe our Depth/Stencil Buffer
			CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, width, height);
			depthStencilTextureDesc.MipLevels = 1;
			depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			hr = device->CreateTexture2D(&depthStencilTextureDesc, NULL, &depthStencilBuffer);
			COM_ERROR_IF_FAILED(hr, L"Failed to create depth stencil buffer.");

			hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, &depthStencilView);
			COM_ERROR_IF_FAILED(hr, L"Failed to create depth stencil view.");

			deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

			CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
			deviceContext->RSSetViewports(1, &viewport);

			if (!pipelineStateGenerator.BuildPipelineStates(device))
			{
				return false;
			}

			pipelineState = pipelineStateGenerator.DefaultPipelineState();

			std::vector<Vertex> data =
			{
				{-1.0f, -1.0f, 0.0f}, //bottomleft
				{0.0f, 1.0f, 0.0f}, //topmid
				{1.0f, -1.0f, 0.5f}, //bottomright
			};
			COM_ERROR_IF_FAILED(test.Initialize(device, data), L"Failed to initialize vertex buffer.");

		}
		catch (COMException& ex)
		{
			MessageBoxW(NULL, L"Critical Exception!", ex.what(), MB_SYSTEMMODAL);
			return false;
		}

		return true;
	}
}