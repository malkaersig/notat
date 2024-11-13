#include "Scene.h"

inline void Scene::AddChild(std::weak_ptr<IPaintable>&& child)
{
	children.emplace_back(child);
}

inline HRESULT Scene::CreateOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::Create, pRenderTarget, pDWriteFactory, pFactory);
}

inline HRESULT Scene::SizeOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::SizeOverride);
}

inline HRESULT Scene::PaintOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::PaintOverride);
}

inline HRESULT Scene::DiscardOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::DiscardOverride);
}
