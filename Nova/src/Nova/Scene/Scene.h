#pragma once
#include "Camera/FreeLookCamera.h"

namespace Nova
{
	class Entity;
}

namespace Nova::Graphics
{
	class Renderer;
}

namespace Nova
{
	class Scene
	{
	public:
		Scene();

		Entity CreateEntity(const std::string& name);

		void RenderEntities(Graphics::Renderer& renderer);

		const Camera& GetCamera() const;
		FreeLookCamera m_FreeLookCamera;

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}

