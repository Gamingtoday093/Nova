#pragma once

namespace Nova
{
	class IImGuiLayer
	{
	public:
		virtual ~IImGuiLayer() = default;

		virtual void Render() = 0;
		virtual void OnAttach() { };
		virtual void OnDetach() { };
	};
}
