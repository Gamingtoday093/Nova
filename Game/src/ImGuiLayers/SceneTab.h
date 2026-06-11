#pragma once
#include <Nova/ImGui/IImGuiLayer.hpp>
#include <Nova/Graphics/Bindables/Texture/RenderTexture.h>

class SceneTab : public Nova::IImGuiLayer
{
public:
	~SceneTab();

	void BeginFrame() override;
	void Render() override;
	void EndFrame() override;

private:
	uint32_t m_Width = 0, m_Height = 0;

	Nova::Graphics::RenderTexture m_RenderTexture;
};

