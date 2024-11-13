#pragma once

#include "Paintable.h"

struct Scene : IPaintable
{
private:

	std::vector<std::weak_ptr<IPaintable>> children;

	template <typename RETURN_TYPE, typename... ARGS >
	HRESULT CallMemberFunctionOnChildren(RETURN_TYPE(IPaintable::* func)(ARGS...), ARGS&&... args);

public:

	void AddChild(std::weak_ptr<IPaintable>&& child);

	inline HRESULT CreateOverride() override;
	inline HRESULT SizeOverride() override;
	inline HRESULT PaintOverride() override;
	inline HRESULT DiscardOverride() override;

};

template<typename RETURN_TYPE, typename ...ARGS>
inline HRESULT Scene::CallMemberFunctionOnChildren(RETURN_TYPE(IPaintable::* func)(ARGS...), ARGS && ...args)
{
	try
	{
		for (auto weakChildPtr : children)
		{
			if (auto childPtr = weakChildPtr.lock())
			{
				(*(childPtr).*func)(std::forward<ARGS>(args)...);
			}
		}
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}
