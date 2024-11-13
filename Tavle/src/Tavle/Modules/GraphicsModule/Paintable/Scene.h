#pragma once

#include "Paintable.h"

struct Scene : IPaintable
{
private:

	std::vector<std::shared_ptr<IPaintable>> children;

	template <typename RETURN_TYPE, typename... ARGS >
	HRESULT CallMemberFunctionOnChildren(RETURN_TYPE(IPaintable::* func)(ARGS...), ARGS&&... args);

public:

	void AddChild(std::shared_ptr<IPaintable>&& child);

	HRESULT CreateOverride() override;
	HRESULT SizeOverride() override;
	HRESULT PaintOverride() override;
	HRESULT DiscardOverride() override;

};

template<typename RETURN_TYPE, typename ...ARGS>
HRESULT Scene::CallMemberFunctionOnChildren(RETURN_TYPE(IPaintable::* func)(ARGS...), ARGS && ...args)
{
	try
	{
		for (auto child : children)
		{
			(*(child).*func)(std::forward<ARGS>(args)...);
		}
	}
	catch (_com_error)
	{
		return E_FAIL;
	}
	return S_OK;
}
