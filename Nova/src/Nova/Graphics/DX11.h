#pragma once
#include <d3d11.h>
#include <d3d11_1.h>
#include <wrl.h>
#include "Nova/Logging/Asserts.h"

namespace Nova::Graphics
{
	class RenderTexture;
}

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

		DX11(const DX11&) = delete;
		DX11& operator=(const DX11&) = delete;

		void Resize(uint32_t width, uint32_t height);
		static void SetRenderTexture(const RenderTexture* renderTexture);

		void BeginFrame(const float clearColor[4]);
		void EndFrame();

		float GetAspectRatio() const;
		// TODO: Used by Camera to convert between ScreenToWorld. Not sure if I want to expose this property? this way
		// Maybe it's fine since it doesn't have any side effects
		static float GetAspectRatio_s();
		static uint32_t GetWidth();
		static uint32_t GetHeight();

		static ID3D11Device1* GetDevice() { return Get().m_Device.Get(); }
		static ID3D11DeviceContext1* GetContext() { return Get().m_Context.Get(); }
		static IDXGISwapChain1* GetSwapChain() { return Get().m_SwapChain.Get(); }

	private:
		inline static DX11& Get()
		{
			NOVA_ASSERT(m_Instance, "DX11 hasn't been Initialized");
			return *m_Instance;
		}

		void CreateBackBufferView();
		void CreateDepthStencilView();
		void UpdateViewport(uint32_t width, uint32_t height);

		uint32_t m_Width, m_Height;
		// Should probably use a Smart Pointer? Or own Wrapper?
		// Maybe move this into RenderTexture like Unity's RenderTexture.active?
		const RenderTexture* m_RenderTexture;

		ComPtr<ID3D11Device1> m_Device;
		ComPtr<ID3D11DeviceContext1> m_Context;
		ComPtr<IDXGISwapChain1> m_SwapChain;

		ComPtr<ID3D11RenderTargetView> m_BackBufferView;
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

		static DX11* m_Instance;
	};
}
