#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "Nova/Graphics/RenderData/Line.h"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class LineBuffer : public IBindable
	{
	public:
		static constexpr uint32_t MAX_LINES = USHORT_MAX / 2;
		
		LineBuffer(uint32_t maxLines = MAX_LINES);

		void Resize(uint32_t newMaxLines);
		void ApplyChanges();

		void Bind() const override;

		uint32_t GetMaxLines() const;

		std::vector<Line> Lines;

	private:
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		ComPtr<ID3D11Buffer> m_IndexBuffer;

		uint32_t m_MaxLines;
	};
}
