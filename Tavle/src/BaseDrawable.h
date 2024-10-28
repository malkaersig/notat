#pragma once

#include <map>
#include <memory>
#include <string>
#include "TavleInclude.h"

class DrawablesManager
{

	template<typename DRAWABLE_TYPE>
	inline ITEM_ID CreateDrawable()
	{
		return lookupTable->CreateItem<DRAWABLE_TYPE>();
	}

	template<typename DRAWABLE_TYPE>
	inline void DestroyDrawable(ITEM_ID id)
	{
		return lookupTable->DestroyItem<DRAWABLE_TYPE>(id);
	}

private:
	std::shared_ptr<LookupTable> lookupTable;
};

template<typename DRAWABLE_TYPE>
class DrawableHandler
{
public:
	DrawableHandler(std::shared_ptr<DrawablesManager> manager) :
		manager(manager)
	{
		id = manager->CreateDrawable<DRAWABLE_TYPE>();
	}
	~DrawableHandler()
	{
		manager->DestroyDrawable<DRAWABLE_TYPE>(id);
	}
	ITEM_ID id;
private:
	std::shared_ptr<DrawablesManager> manager;
public:
	std::vector<std::unique_ptr<DrawableHandler>> children;

};

class DrawableInterface
{

public:
	virtual void draw(int x, int y) = 0;

};
