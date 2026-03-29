#pragma once

namespace Nova
{
	class IApplication
	{
	public:
		virtual ~IApplication() = default;
		virtual void Run() = 0;
	};
}
