#include "novapch.h"
#include "Material.h"
#include "Nova/Graphics/Bindables/Shader/PixelShader.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Assets/AssetFormats/ShaderAsset.h"

Nova::Graphics::Material::Material(std::shared_ptr<PixelShader> shader)
{
	NOVA_ASSERT(shader, "Material Requires a Shader");
	m_Shader = std::move(shader);
}

Nova::Graphics::Material::~Material() = default;

void Nova::Graphics::Material::Bind() const
{
	m_Shader->Bind();
}

Nova::Graphics::Material::Material(const std::filesystem::path& shaderPath)
{
	auto shaderAsset = AssetManager::GetAsset<ShaderAsset>(shaderPath);
	NOVA_ASSERT(shaderAsset, "Failed to find Built-in Shader");

	m_Shader = shaderAsset->GetShader();
}
