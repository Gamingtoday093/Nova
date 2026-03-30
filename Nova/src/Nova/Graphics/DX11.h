#pragma once
#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class DX11
	{
	public:
		DX11(HWND hwnd, uint32_t width, uint32_t height);
		~DX11();

		void Resize(uint32_t width, uint32_t height);

		void BeginFrame(const float clearColour[4]);
		void EndFrame();

		static ID3D11Device* GetDevice() { return Get().m_Device.Get(); }
		static ID3D11DeviceContext* GetContext() { return Get().m_Context.Get(); }
		static IDXGISwapChain* GetSwapChain() { return Get().m_SwapChain.Get(); }

	private:
		inline static DX11& Get()
		{
			NOVA_ASSERT(m_Instance, "DX11 hasn't been Initialized");
			return *m_Instance;
		}

		uint32_t m_Width, m_Height;

		ComPtr<ID3D11Device> m_Device;
		ComPtr<ID3D11DeviceContext> m_Context;
		ComPtr<IDXGISwapChain> m_SwapChain;
		ComPtr<ID3D11RenderTargetView> m_BackBuffer;
		ComPtr<ID3D11DepthStencilView> m_DepthBuffer;

		static DX11* m_Instance;
	};
}
