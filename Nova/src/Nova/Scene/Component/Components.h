#pragma once
#include "ScriptBase.h"
#include "Nova/Graphics/Resources/Transform.h"
#include "Nova/Graphics/Resources/Bounds.h"

namespace Nova::Graphics
{
	class Mesh;
	class Material;
}

namespace Nova
{
	struct TagComponent
	{
		TagComponent(const std::string& name);

		std::string Name;
	};

	struct TransformComponent
	{
		Graphics::Transform Transform;
	};

	struct MeshRendererComponent
	{
		MeshRendererComponent() = default;
		MeshRendererComponent(std::shared_ptr<Graphics::Mesh> mesh);
		MeshRendererComponent(std::shared_ptr<Graphics::Mesh> mesh, std::shared_ptr<Graphics::Material> material);

		Graphics::Bounds GetBounds(const Graphics::Transform& transform) const;

		// Should probably store reference to Assets in some way
		// Using std::variant<Mesh, MeshAsset> was a little too slow imo. How often do you need to asset? Maybe the ID is enough? Even if it's 16 bytes
		std::shared_ptr<Graphics::Mesh> Mesh;
		// TODO: Support Multiple Materials
		std::shared_ptr<Graphics::Material> Material;
	};

	namespace Scripts
	{
		class ScriptCollectionComponent
		{
		public:
			void Release();

			template<ScriptType TScript>
			TScript& AddScript(Entity& entity);
			ScriptBase& AddScript(Entity& entity, Scripts::ScriptID scriptID, ScriptBase* script);

			template<ScriptType TScript>
			bool HasScript() const;
			bool HasScript(Scripts::ScriptID scriptID) const;

			template<ScriptType TScript>
			TScript* TryGetScript() const;
			ScriptBase* TryGetScript(Scripts::ScriptID scriptID) const;

			template<ScriptType TScript>
			bool RemoveScript(Entity& entity);
			bool RemoveScript(Entity& entity, Scripts::ScriptID scriptID);

			void Update();

			std::vector<ScriptBase*>::iterator begin();
			std::vector<ScriptBase*>::const_iterator begin() const;
			std::vector<ScriptBase*>::iterator end();
			std::vector<ScriptBase*>::const_iterator end() const;

		private:
			std::vector<ScriptBase*> m_Scripts;
		};

		template<ScriptType TScript>
		TScript& ScriptCollectionComponent::AddScript(Entity& entity)
		{
			TScript* existingScript = TryGetScript<TScript>();
			if (existingScript)
			{
				TagComponent* tag = entity.TryGetComponent<TagComponent>();
				NOVA_CORE_ERROR("Entity {} already has {}", tag ? tag->Name : "(Missing Tag Component)", TScript::GetScriptName_s());
				return *existingScript;
			}

			return static_cast<TScript&>(AddScript(entity, Scripts::GetScriptID<TScript>(), new TScript()));
		}

		template<ScriptType TScript>
		bool ScriptCollectionComponent::HasScript() const
		{
			return TryGetScript<TScript>() != nullptr;
		}

		template<ScriptType TScript>
		TScript* ScriptCollectionComponent::TryGetScript() const
		{
			return static_cast<TScript*>(TryGetScript(Scripts::GetScriptID<TScript>()));
		}

		template<ScriptType TScript>
		bool ScriptCollectionComponent::RemoveScript(Entity& entity)
		{
			return RemoveScript(entity, Scripts::GetScriptID<TScript>());
		}
	}
}