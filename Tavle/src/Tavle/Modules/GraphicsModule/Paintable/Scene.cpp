#include "Scene.h"

void Scene::AddChild(std::shared_ptr<IPaintable>&& child)
{
	children.emplace_back(child);
}

HRESULT Scene::CreateOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::Create, pRenderTarget, pDWriteFactory, pFactory);
}

HRESULT Scene::SizeOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::SizeOverride);
}

HRESULT Scene::PaintOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::PaintOverride);
}

HRESULT Scene::DiscardOverride()
{
	return CallMemberFunctionOnChildren(&IPaintable::DiscardOverride);
}
