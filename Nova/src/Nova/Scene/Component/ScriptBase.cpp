#include "novapch.h"
#include "ScriptBase.h"

bool Nova::ScriptBase::GetEnabled() const
{
	return m_IsEnabled;
}

void Nova::ScriptBase::SetEnabled(bool newEnabled)
{
	bool isNewEnabled = m_IsEnabled != newEnabled;
	m_IsEnabled = newEnabled;

	if (isNewEnabled)
	{
		if (m_IsEnabled) OnEnable();
		else OnDisable();
	}
}
