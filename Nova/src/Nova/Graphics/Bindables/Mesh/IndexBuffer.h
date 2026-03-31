#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class IndexBuffer : public IBindable
	{
	public:
		void Create(const std::vector<uint16_t>& indices);
		void Bind() const override;

		uint32_t Length() const;

	private:
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		uint32_t m_IndexBufferLength = 0;
	};
}
