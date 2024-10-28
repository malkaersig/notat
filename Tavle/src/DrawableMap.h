#pragma once

#include "BaseDrawable.h"
#include <map>
#include "COM/Helpers.h"
#include <type_traits>
#include <stdexcept>

using ITEM_ID = unsigned int;

class PolymMap
{
public:
	template<class DRAWABLE_TYPE>
	ITEM_ID CreateItem();

	template<class DRAWABLE_TYPE>
	void DestroyItem(ITEM_ID id);
	
	template<typename DRAWABLE_TYPE>
	DRAWABLE_TYPE& operator[](ITEM_ID id);

private:
	std::unordered_map<ITEM_ID, void*> map;
};

template<class DRAWABLE_TYPE>
inline ITEM_ID PolymMap::CreateItem()
{
	if (!std::is_base_of<DrawableInterface, DRAWABLE_TYPE>)
		throw std::bad_typeid();
	DRAWABLE_TYPE* drawable_ptr = new DRAWABLE_TYPE();
	return ITEM_ID();
}

template<class DRAWABLE_TYPE>
inline void PolymMap::DestroyItem(ITEM_ID id)
{
	DRAWABLE_TYPE* drawable_ptr = static_cast<DRAWABLE_TYPE>(map[id]);
	map.erase(id);
	delete drawable_ptr;
}

template<typename DRAWABLE_TYPE>
inline DRAWABLE_TYPE& PolymMap::operator[](ITEM_ID id)
{
	return *map[id];
}
