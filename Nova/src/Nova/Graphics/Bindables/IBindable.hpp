#pragma once

namespace Nova::Graphics
{
	class IBindable
	{
	public:
		virtual ~IBindable() = default;

		virtual void Bind() const = 0;
	};
}
