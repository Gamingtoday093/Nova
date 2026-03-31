#pragma once

#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "BufferCommon.hpp"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include <cstdint>

namespace Nova::Graphics
{
	template<typename T>
	class ConstBuffer : public IBindable
	{
	public:
		ConstBuffer(EBindType bindType);

		void ApplyBuffer();
		void Bind() const override;
		void Bind(uint32_t slot) const;

		EBindType GetBindType() const;

		T Data{};

	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		EBindType m_BindType;
	};

	template<typename T>
	inline ConstBuffer<T>::ConstBuffer(EBindType bindType)
	{
		m_BindType = bindType;

		D3D11_BUFFER_DESC bufferDesc
		{
			.ByteWidth = sizeof(T),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
		};

		D3D11_SUBRESOURCE_DATA bufferData
		{
			.pSysMem = &Data
		};

		NOVA_HRASSERT(DX11::GetDevice()->CreateBuffer(&bufferDesc, &bufferData, m_Buffer.GetAddressOf()), "Create ConstBuffer Buffer");
	}

	template<typename T>
	inline void ConstBuffer<T>::ApplyBuffer()
	{
		D3D11_MAPPED_SUBRESOURCE bufferData{};

		NOVA_HRASSERT(DX11::GetContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData), "Map ConstBuffer Buffer");

		memcpy_s(bufferData.pData, sizeof(T), &Data, sizeof(T));

		DX11::GetContext()->Unmap(m_Buffer.Get(), 0);
	}

	template<typename T>
	inline void ConstBuffer<T>::Bind() const
	{
		Bind(0);
	}

	template<typename T>
	inline void ConstBuffer<T>::Bind(uint32_t slot) const
	{
		switch (m_BindType)
		{
		case EBindType::VertexShader:
			DX11::GetContext()->VSSetConstantBuffers(slot, 1, m_Buffer.GetAddressOf());
			break;
		case EBindType::PixelShader:
			DX11::GetContext()->PSSetConstantBuffers(slot, 1, m_Buffer.GetAddressOf());
			break;
		case EBindType::VertexAndPixelShader:
			DX11::GetContext()->VSSetConstantBuffers(slot, 1, m_Buffer.GetAddressOf());
			DX11::GetContext()->PSSetConstantBuffers(slot, 1, m_Buffer.GetAddressOf());
			break;
		default:
			NOVA_ASSERT(false, "Unsupported BindType");
			break;
		}
	}

	template<typename T>
	inline EBindType ConstBuffer<T>::GetBindType() const
	{
		return m_BindType;
	}
}