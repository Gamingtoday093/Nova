#pragma once
#include "Scene.h"
#include "Component/ScriptID.h"
#include "Nova/Logging/Asserts.h"

namespace Nova
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template<ScriptType TScript>
		TScript& AddComponent();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<ScriptType TScript>
		TScript& GetOrAddComponent();

		template<typename T, typename... Args>
		T& GetOrAddComponent(Args&&... args);

		template<ScriptType TScript>
		bool HasComponent() const;

		template<typename T>
		bool HasComponent() const;

		template<ScriptType TScript>
		TScript& GetComponent() const;

		template<typename T>
		T& GetComponent() const;
		
		template<ScriptType TScript>
		TScript* TryGetComponent() const;

		template<typename T>
		T* TryGetComponent() const;

		template<ScriptType TScript>
		bool RemoveComponent();

		template<typename T>
		bool RemoveComponent();

		entt::entity GetEntityHandle() const;
		Scene* GetScene() const;

		bool operator ==(const Entity& entity) const;

		bool IsValid() const;
		operator bool() const;

	private:
		ScriptBase& AddScript(Scripts::ScriptCollectionComponent& scriptCollection, Scripts::ScriptID scriptID, ScriptBase* script);

		ScriptBase* TryGetScript(const Scripts::ScriptCollectionComponent& scriptCollection, Scripts::ScriptID scriptID) const;

		bool RemoveScript(Scripts::ScriptCollectionComponent& scriptCollection, Scripts::ScriptID scriptID);

		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};

	template<ScriptType TScript>
	TScript& Entity::AddComponent()
	{
		auto& scriptCollection = GetOrAddComponent<Scripts::ScriptCollectionComponent>();
		Scripts::ScriptID scriptID = Scripts::GetScriptID<TScript>();
		auto* script = TryGetScript(scriptCollection, scriptID);
		if (script != nullptr)
		{
			NOVA_CORE_ERROR("Entity already has {}", TScript::GetScriptName_s());
			return static_cast<TScript&>(*script);
		}

		return static_cast<TScript&>(AddScript(scriptCollection, scriptID, new TScript()));
	}

	template<typename T, typename... Args>
	T& Entity::AddComponent(Args&&... args)
	{
		T* existing = m_Scene->m_Registry.try_get<T>(m_Handle);
		if (existing)
		{
			NOVA_CORE_WARN("Entity already has Component");
			return *existing;
		}
		return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
	}

	template<ScriptType TScript>
	TScript& Entity::GetOrAddComponent()
	{
		auto& scriptCollection = GetOrAddComponent<Scripts::ScriptCollectionComponent>();
		Scripts::ScriptID scriptID = Scripts::GetScriptID<TScript>();
		auto* script = TryGetScript(scriptCollection, scriptID);
		if (script != nullptr) static_cast<TScript&>(*script);

		return static_cast<TScript&>(AddScript(scriptCollection, scriptID, new TScript()));
	}

	template<typename T, typename... Args>
	T& Entity::GetOrAddComponent(Args&&... args)
	{
		return m_Scene->m_Registry.get_or_emplace<T>(m_Handle, std::forward<Args>(args)...);
	}

	template<ScriptType TScript>
	bool Entity::HasComponent() const
	{
		return TryGetComponent<TScript>() != nullptr;
	}

	template<typename T>
	bool Entity::HasComponent() const
	{
		return m_Scene->m_Registry.any_of<T>(m_Handle);
	}

	template<ScriptType TScript>
	TScript& Entity::GetComponent() const
	{
		TScript* script = TryGetComponent<TScript>();
		NOVA_ASSERT(script, "Entity doesn't have Script {}", TScript::GetScriptName_s());
		return *script;
	}

	template<typename T>
	T& Entity::GetComponent() const
	{
		NOVA_ASSERT(HasComponent<T>(), "Entity doesn't have Component");
		return m_Scene->m_Registry.get<T>(m_Handle);
	}

	template<ScriptType TScript>
	TScript* Entity::TryGetComponent() const
	{
		auto* scriptCollection = TryGetComponent<Scripts::ScriptCollectionComponent>();
		if (scriptCollection == nullptr) return nullptr;
		return static_cast<TScript*>(TryGetScript(*scriptCollection, Scripts::GetScriptID<TScript>()));
	}

	template<typename T>
	T* Entity::TryGetComponent() const
	{
		return m_Scene->m_Registry.try_get<T>(m_Handle);
	}

	template<ScriptType TScript>
	bool Entity::RemoveComponent()
	{
		auto* scriptCollection = TryGetComponent<Scripts::ScriptCollectionComponent>();
		if (scriptCollection == nullptr) return false;
		return RemoveScript(*scriptCollection, Scripts::GetScriptID<TScript>());
	}

	template<typename T>
	bool Entity::RemoveComponent()
	{
		if (!HasComponent<T>()) return false;

		m_Scene->m_Registry.erase<T>(m_Handle);
		return true;
	}
}
