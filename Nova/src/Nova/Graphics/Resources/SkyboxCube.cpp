#include "novapch.h"
#include "SkyboxCube.h"
#include "Nova/Graphics/DX11.h"

Nova::Graphics::SkyboxCube::SkyboxCube()
{
	constexpr float side = 1.0f / 2.0f;

	std::vector<SkyboxVertex> vertices
	{
		{ -side, -side, -side },
		{ side, -side, -side },
		{ -side, side, -side },
		{ side, side, -side },
		{ -side, -side, side },
		{ side, -side, side },
		{ -side, side, side },
		{ side, side, side }
	};

	m_VertexBuffer.Create(vertices);

	m_IndexBuffer.Create({ 0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4 });
}

void Nova::Graphics::SkyboxCube::Bind() const
{
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();
}

void Nova::Graphics::SkyboxCube::DrawIndexed() const
{
	DX11::GetContext()->DrawIndexed(m_IndexBuffer.Length(), 0, 0);
}
