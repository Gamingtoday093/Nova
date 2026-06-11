#pragma once

namespace Nova
{
	class IImGuiLayer
	{
	public:
		virtual ~IImGuiLayer() = default;

		virtual void BeginFrame() { };
		virtual void Render() = 0;
		virtual void EndFrame() { };
	};
}
