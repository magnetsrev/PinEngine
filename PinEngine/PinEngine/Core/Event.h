#pragma once
#include <vector>
namespace PinEngine
{
	enum EventHandlerType
	{
		OnUpdate = 1 << 0,
		OnMouseOver = 1 << 1,
		OnMouseExit = 1 << 2
	};
	template<class T>
	class Event
	{
	public:
		Event<T>& operator +=(void (*rhs)(T*))
		{
			this->callbacks.push_back(rhs);
			return *this;
		};

		std::vector<void (*)(T*)> callbacks;
	};
};