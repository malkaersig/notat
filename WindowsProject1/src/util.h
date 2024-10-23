#pragma once

template<typename T, typename... Args>
auto bind_member(T* obj, void (T::* func)(Args...))
{
	return [obj, func](Args... args)
		{
			(obj->*func)(args...);
		};
}