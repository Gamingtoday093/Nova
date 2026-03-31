#pragma once
#include <d3d11.h>
#include <d3d11_1.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	struct GraphicsContextParameters
	{
		GraphicsContextParameters(HWND hwnd)
		{
			HWND = hwnd;

			RECT rect;
			GetClientRect(HWND, &rect);

			Width = rect.right - rect.left;
			Height = rect.bottom - rect.top;
		}

		HWND HWND = 0;
		uint32_t Width = 0, Height = 0;
	};

	class DX11
	{
	public:
		DX11(const GraphicsContextParameters& contextParameters);
		~DX11();

		void Resize(uint32_t width, uint32_t height);

		void BeginFrame(const float clearColor[4]);
		void EndFrame();

		static ID3D11Device1* GetDevice() { return Get().m_Device.Get(); }
		static ID3D11DeviceContext1* GetContext() { return Get().m_Context.Get(); }
		static IDXGISwapChain1* GetSwapChain() { return Get().m_SwapChain.Get(); }

	private:
		inline static DX11& Get()
		{
			NOVA_ASSERT(m_Instance, "DX11 hasn't been Initialized");
			return *m_Instance;
		}

		void CreateRenderTargetViews();
		void CreateDepthStencilView();
		void UpdateViewport();

		uint32_t m_Width, m_Height;

		ComPtr<ID3D11Device1> m_Device;
		ComPtr<ID3D11DeviceContext1> m_Context;
		ComPtr<IDXGISwapChain1> m_SwapChain;

		ComPtr<ID3D11RenderTargetView> m_SceneView;
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

		static DX11* m_Instance;
	};
}
