#pragma once

namespace Nova::Graphics
{
	class PixelShader;
}

namespace Nova::Graphics
{
	class Material
	{
	public:
		Material(std::shared_ptr<PixelShader> shader);
		virtual ~Material();

		virtual void Bind() const;

	protected:
		Material(const std::filesystem::path& shaderPath);

	private:
		std::shared_ptr<PixelShader> m_Shader;
	};

	template<typename T>
	concept MaterialType = std::derived_from<T, Material>;
}
