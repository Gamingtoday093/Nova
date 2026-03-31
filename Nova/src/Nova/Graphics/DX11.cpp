#include "pch.h"
#include "DX11.h"
#include "Logging/HRAsserts.h"

Nova::Graphics::DX11* Nova::Graphics::DX11::m_Instance;

Nova::Graphics::DX11::DX11(const GraphicsContextParameters& contextParameters)
{
	NOVA_ASSERT(!m_Instance, "Creating multiple DX11 is not allowed");
	m_Instance = this;
	
	m_Width = contextParameters.Width;
	m_Height = contextParameters.Height;

	UINT deviceFlags = 0;
#ifdef DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	std::array<D3D_FEATURE_LEVEL, 2> featureLevels = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	ComPtr<ID3D11Device> baseDevice;
	ComPtr<ID3D11DeviceContext> baseContext;

	NOVA_HRASSERT(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceFlags,
		featureLevels.data(),
		UINT(featureLevels.size()),
		D3D11_SDK_VERSION,
		&baseDevice,
		&featureLevel,
		&baseContext), "Failed to get Create Base Device & Context");

	NOVA_HRASSERT(baseDevice.As(&m_Device), "Failed to get as ID3D11Device1");
	NOVA_HRASSERT(baseContext.As(&m_Context), "Failed to get as ID3D11DeviceContext1");

	ComPtr<IDXGIDevice> dxgiDevice;
	ComPtr<IDXGIAdapter> dxgiAdapter;
	ComPtr<IDXGIFactory2> dxgiFactory;

	NOVA_HRASSERT(m_Device.As(&dxgiDevice), "Failed to get as IDXGIDevice");
	NOVA_HRASSERT(dxgiDevice->GetAdapter(&dxgiAdapter), "Failed to get IDXGIAdapter");
	NOVA_HRASSERT(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)), "Failed to get IDXGIFactory2");

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc
	{
		.Width = m_Width,
		.Height = m_Height,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.Flags = 0
	};

	NOVA_HRASSERT(dxgiFactory->CreateSwapChainForHwnd(
		m_Device.Get(),
		contextParameters.HWND,
		&swapchainDesc,
		nullptr,
		nullptr,
		&m_SwapChain), "Failed to create SwapChain");

	CreateRenderTargetViews();
	CreateDepthStencilView();
	UpdateViewport();
}

Nova::Graphics::DX11::~DX11()
{
	m_Instance = nullptr;
}

void Nova::Graphics::DX11::Resize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	m_Context->OMSetRenderTargets(0, 0, 0);
	m_Context->Flush();

	// Release buffers
	m_SceneView = nullptr;
	m_DepthStencilView = nullptr;

	NOVA_HRASSERT(m_SwapChain->ResizeBuffers(0, m_Width, m_Height, DXGI_FORMAT_UNKNOWN, 0), "Resize Buffers");
	
	if (m_Width == 0 || m_Height == 0) return;

	CreateRenderTargetViews();
	CreateDepthStencilView();
	UpdateViewport();
}

void Nova::Graphics::DX11::BeginFrame(const float clearColour[4])
{
	m_Context->ClearRenderTargetView(m_SceneView.Get(), clearColour);
}

void Nova::Graphics::DX11::EndFrame()
{
	// Maybe use Present1() for ImGui Editor stuff as thats basically static
	m_SwapChain->Present(1, 0);
}

void Nova::Graphics::DX11::CreateRenderTargetViews()
{
	ComPtr<ID3D11Texture2D> backBuffer;
	NOVA_HRASSERT(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)), "Get BackBuffer");

	D3D11_RENDER_TARGET_VIEW_DESC sceneDesc
	{
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
		.Texture2D
		{
			.MipSlice = 0
		}
	};
	NOVA_HRASSERT(m_Device->CreateRenderTargetView(backBuffer.Get(), &sceneDesc, &m_SceneView), "Create SceneView");
}

void Nova::Graphics::DX11::CreateDepthStencilView()
{
	ComPtr<ID3D11Texture2D> depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthTextureDesc
	{
		.Width = m_Width,
		.Height = m_Height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_D32_FLOAT,
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL
	};
	NOVA_HRASSERT(m_Device->CreateTexture2D(&depthTextureDesc, nullptr, &depthStencilTexture), "Create DepthStencilTexture");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc
	{
		.Format = DXGI_FORMAT_D32_FLOAT,
		.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
		.Texture2D
		{
			.MipSlice = 0
		}
	};
	NOVA_HRASSERT(m_Device->CreateDepthStencilView(depthStencilTexture.Get(), &depthViewDesc, &m_DepthStencilView), "Create DepthStencilView");
}

void Nova::Graphics::DX11::UpdateViewport()
{
	D3D11_VIEWPORT viewport
	{
		.TopLeftX = 0.f,
		.TopLeftY = 0.f,
		.Width = float(m_Width),
		.Height = float(m_Height),
		.MinDepth = 0.f,
		.MaxDepth = 1.f
	};

	m_Context->RSSetViewports(1, &viewport);
}
