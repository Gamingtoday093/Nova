#include "novapch.h"
#include "StandardMaterial.h"
#include "Nova/Graphics/Bindables/Texture/Texture2D.h"

Nova::Graphics::StandardMaterial::StandardMaterial() : Material("Assets/CompiledShaders/Standard_ps.cso") { }

void Nova::Graphics::StandardMaterial::Bind() const
{
	Material::Bind();

	if (AlbedoTexture)
	{
		AlbedoTexture->Bind();
		m_AlbedoSampler.Bind();
	}
}
