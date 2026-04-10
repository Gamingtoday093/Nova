#include "novapch.h"
#include "Time.h"

Nova::Time* Nova::Time::m_Instance;

Nova::Time::Time()
{
	NOVA_ASSERT(!m_Instance, "Creating multiple Time is not allowed!");
	m_Instance = this;

	m_LastFrameStart = std::chrono::high_resolution_clock::now();
}

Nova::Time::~Time()
{
	m_Instance = nullptr;
}

void Nova::Time::BeginFrame()
{
	auto lastFrameStart = m_LastFrameStart;
	m_LastFrameStart = std::chrono::high_resolution_clock::now();

	constexpr float nanoToSec = 1.0f / float(std::nano::den);
	m_DeltaTime = (m_LastFrameStart - lastFrameStart).count() * nanoToSec;
}

float Nova::Time::GetDeltaTime()
{
	return Time::Get().m_DeltaTime;
}
