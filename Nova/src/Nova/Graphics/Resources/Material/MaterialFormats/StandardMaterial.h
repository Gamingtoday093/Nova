#pragma once
#include "Nova/Graphics/Resources/Material/Material.h"
#include "Nova/Graphics/Bindables/Shader/Sampler.h"

namespace Nova::Graphics
{
	class Texture2D;
}

namespace Nova::Graphics
{
	class StandardMaterial : public Material
	{
	public:
		StandardMaterial();

		void Bind() const override;

		std::shared_ptr<Texture2D> AlbedoTexture;

	private:
		Sampler m_AlbedoSampler;
	};
}
