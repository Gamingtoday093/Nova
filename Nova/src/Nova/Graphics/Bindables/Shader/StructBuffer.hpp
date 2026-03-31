#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "BufferCommon.hpp"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include <cstdint>

namespace Nova::Graphics
{
	template<typename T>
	class StructBuffer : public IBindable
	{
	public:
		StructBuffer(EBindType bindType, uint32_t maxLength);

		void Resize(uint32_t newMaxLength);
		void ApplyBuffer();
		void Bind() const override;
		void Bind(uint32_t slot) const;

		EBindType GetBindType() const;
		uint32_t GetMaxLength() const;

		std::vector<T> Data;

	private:
		ComPtr<ID3D11Buffer> m_Buffer;
		ComPtr<ID3D11ShaderResourceView> m_BufferView;
		EBindType m_BindType;
		uint32_t m_BufferLength;
	};
	
	template<typename T>
	inline StructBuffer<T>::StructBuffer(EBindType bindType, uint32_t maxLength)
	{
		m_BindType = bindType;
		Resize(maxLength);
	}
	
	template<typename T>
	inline void StructBuffer<T>::Resize(uint32_t newMaxLength)
	{
		m_Buffer = nullptr;
		m_BufferView = nullptr;
		Data.reserve(newMaxLength);

		m_BufferLength = newMaxLength;

		D3D11_BUFFER_DESC bufferDesc
		{
			.ByteWidth = sizeof(T) * m_BufferLength,
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_SHADER_RESOURCE,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
			.StructureByteStride = sizeof(T)
		};

		D3D11_SUBRESOURCE_DATA bufferData
		{
			.pSysMem = std::data(Data)
		};

		NOVA_HRASSERT(DX11::GetDevice()->CreateBuffer(&bufferDesc, &bufferData, m_Buffer.GetAddressOf()), "Create StructBuffer Buffer");

		D3D11_SHADER_RESOURCE_VIEW_DESC bufferViewDesc
		{
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_BUFFER,
			.Buffer
			{
				.FirstElement = 0,
				.NumElements = m_BufferLength
			}
		};

		NOVA_HRASSERT(DX11::GetDevice()->CreateShaderResourceView(m_Buffer.Get(), &bufferViewDesc, m_BufferView.GetAddressOf()), "Create StructBuffer Buffer");
	}
	
	template<typename T>
	inline void StructBuffer<T>::ApplyBuffer()
	{
		NOVA_ASSERT(Data.size() <= m_BufferLength, "Failed to ApplyBuffer, Data is too long");

		D3D11_MAPPED_SUBRESOURCE bufferData{};

		NOVA_HRASSERT(DX11::GetContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData), "Map StructBuffer Buffer");

		memcpy_s(bufferData.pData, sizeof(T) * m_BufferLength, std::data(Data), sizeof(T) * Data.size());

		DX11::GetContext()->Unmap(m_Buffer.Get(), 0);
	}
	
	template<typename T>
	inline void StructBuffer<T>::Bind() const
	{
		Bind(0);
	}
	
	template<typename T>
	inline void StructBuffer<T>::Bind(uint32_t slot) const
	{
		switch (m_BindType)
		{
			case EBindType::VertexShader:
				DX11::GetContext()->VSSetShaderResources(slot, 1, m_BufferView.GetAddressOf());
				break;
			case EBindType::PixelShader:
				DX11::GetContext()->PSSetShaderResources(slot, 1, m_BufferView.GetAddressOf());
				break;
			case EBindType::VertexAndPixelShader:
				DX11::GetContext()->VSSetShaderResources(slot, 1, m_BufferView.GetAddressOf());
				DX11::GetContext()->PSSetShaderResources(slot, 1, m_BufferView.GetAddressOf());
				break;
			default:
				NOVA_ASSERT(false, "Unsupported BindType");
				break;
		}
	}
	
	template<typename T>
	inline EBindType StructBuffer<T>::GetBindType() const
	{
		return m_BindType;
	}
	
	template<typename T>
	inline uint32_t StructBuffer<T>::GetMaxLength() const
	{
		return m_BufferLength;
	}
}