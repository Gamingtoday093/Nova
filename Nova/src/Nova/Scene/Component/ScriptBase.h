#pragma once
#include "Nova/Scene/Entity.h"
#include "Nova/Tools/TypeNameFormat.hpp"

namespace Nova
{
	class ScriptBase
	{
	public:
		virtual ~ScriptBase() = default;

		bool GetEnabled() const;
		void SetEnabled(bool newEnabled);

		template<typename T>
		T& GetComponent() const;

		virtual const char* GetScriptName() const = 0;
		static const char* GetScriptName_s() { return "Script"; }
		
#define SCRIPT_NAME(scriptName)																				\
		const char* GetScriptName() const override { return GetScriptName_s(); }							\
		static const char* GetScriptName_s()																\
		{																									\
			static constexpr auto typeName = Nova::TypeNameFormat::FormatTypeName(#scriptName, "Script");	\
			return typeName.CStr();																			\
		}

	protected:
		virtual void Awake() { }
		virtual void Start() { }
		virtual void Update() { }
		virtual void OnEnable() { }
		virtual void OnDisable() { }
		
		Entity m_Entity;

	private:
		bool m_IsEnabled = false;
		bool m_HasStarted = false;

		Scripts::ScriptID m_ScriptID = {};

		friend class Scene;
		friend class Scripts::ScriptCollectionComponent;
	};
	
	template<typename T>
	T& ScriptBase::GetComponent() const
	{
		return m_Entity.GetComponent<T>();
	}
}
