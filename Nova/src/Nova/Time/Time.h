#pragma once

namespace Nova
{
	class Time
	{
	public:
		Time();
		~Time();

		void BeginFrame();

		static float GetDeltaTime();

	private:
		inline static Time& Get()
		{
			NOVA_ASSERT(m_Instance, "Time hasn't been Initialized");
			return *m_Instance;
		}

		std::chrono::high_resolution_clock::time_point m_LastFrameStart;
		float m_DeltaTime = 0;

		static Time* m_Instance;
	};
}

