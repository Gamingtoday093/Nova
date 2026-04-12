#include "novapch.h"
#include "ScriptBase.h"

bool Nova::ScriptBase::GetEnabled() const
{
	return m_IsEnabled;
}

void Nova::ScriptBase::SetEnabled(bool newEnabled)
{
	m_IsEnabled = newEnabled;
	m_HasStarted = m_HasStarted && m_IsEnabled;
}
