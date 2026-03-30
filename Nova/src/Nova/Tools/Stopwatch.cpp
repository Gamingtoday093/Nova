#include "pch.h"
#include "Stopwatch.h"

using namespace std::chrono;

Nova::Stopwatch Nova::Stopwatch::StartNew()
{
	Stopwatch sw;
	sw.Start();
	return sw;
}

void Nova::Stopwatch::Start()
{
	if (m_IsRunning) return;
	m_IsRunning = true;
	
	m_StartTime = high_resolution_clock::now();
}

void Nova::Stopwatch::Stop()
{
	if (!m_IsRunning) return;
	m_IsRunning = false;

	m_ElapsedNanoSeconds += (high_resolution_clock::now() - m_StartTime).count();
}

void Nova::Stopwatch::Reset()
{
	m_IsRunning = false;
	m_ElapsedNanoSeconds = 0;
}

void Nova::Stopwatch::Restart()
{
	Reset();
	Start();
}

float Nova::Stopwatch::GetElapsedMilliseconds() const
{
	int64_t totalNanoSeconds = m_ElapsedNanoSeconds;
	if (m_IsRunning)
		totalNanoSeconds += (high_resolution_clock::now() - m_StartTime).count();

	constexpr float nanoToMilli = 1.0f / float(std::nano::den / std::milli::den);
	return totalNanoSeconds * nanoToMilli;
}
