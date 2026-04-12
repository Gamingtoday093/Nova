#pragma once
#include <concepts>

namespace Nova
{
	class ScriptBase;

	template<typename T>
	concept ScriptType = std::derived_from<T, ScriptBase>;

	namespace Scripts
	{
		using ScriptID = entt::id_type;

		template<ScriptType TScript>
		ScriptID GetScriptID() { return entt::type_hash<TScript>::value(); }

		class ScriptCollectionComponent;
	}
}
