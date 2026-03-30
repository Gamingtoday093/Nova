#pragma once
#include <chrono>

namespace Nova
{
	class Stopwatch
	{
	public:
		static Stopwatch StartNew();

		void Start();
		void Stop();
		void Reset();
		void Restart();

		float GetElapsedMilliseconds() const;

	private:
		std::chrono::high_resolution_clock::time_point m_StartTime;
		int64_t m_ElapsedNanoSeconds = 0;
		bool m_IsRunning = false;
	};
}
