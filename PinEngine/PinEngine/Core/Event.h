#pragma once
#include <vector>
namespace PinEngine
{
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