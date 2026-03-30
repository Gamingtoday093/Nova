#include "pch.h"
#include "DX11.h"
#include "Logging/HRAsserts.h"

Nova::Graphics::DX11* Nova::Graphics::DX11::m_Instance;

Nova::Graphics::DX11::DX11(HWND hwnd, uint32_t width, uint32_t height)
{
	NOVA_ASSERT(!m_Instance, "Creating multiple DX11 is not allowed");
	m_Instance = this;
	
	m_Width = width;
	m_Height = height;

	DXGI_SWAP_CHAIN_DESC swapchainDesc
	{
		.BufferDesc
		{
			.Width = 0,
			.Height = 0,
			.RefreshRate
			{
				.Numerator = 0,
				.Denominator = 0
			},
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			.Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
		},
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 1,
		.OutputWindow = hwnd,
		.Windowed = TRUE,
		.SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
		.Flags = 0
	};

	NOVA_HRASSERT(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&m_SwapChain,
		&m_Device,
		nullptr,
		&m_Context), "Failed to Initialize DX11");
}

Nova::Graphics::DX11::~DX11()
{
	m_Instance = nullptr;
}

void Nova::Graphics::DX11::Resize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;


}

void Nova::Graphics::DX11::BeginFrame(const float clearColour[4])
{

}

void Nova::Graphics::DX11::EndFrame()
{

}
