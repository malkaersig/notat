#pragma once

#include "TavleInclude.h"

// ERROR CLASS FOR WHEN FORGETTING THE DESTROY ALL ITEMS IN LOOKUP TABLE;
class ErrMemoryLeak : public std::exception
{
public:
	ErrMemoryLeak() = default;
	const char* what() const noexcept override
	{
		return "Error: Instance going out of scoping before destroying every element.";
	}
};

using ITEM_ID = unsigned int;

class LookupTable
{
public:
	LookupTable() = default;
	~LookupTable();

	// create item of class DERIVED_CLASS and return its id
	template<typename ITEM_TYPE>
	ITEM_ID CreateItem();

	// destroy item of class DERIVED_CLASS at given id
	template<typename ITEM_TYPE>
	void DestroyItem(ITEM_ID id);

	// access item of class DERIVED_CLASS at given id
	template<typename ITEM_TYPE>
	ITEM_TYPE& get(ITEM_ID id);

	// returns vector of current ids in the lookup table
	std::vector<ITEM_ID> get_ids();

private:
	std::unordered_map<ITEM_ID, void*> map;
	// not current amount of ids, just amount created in total
	ITEM_ID id_counter = 0;
	int size;
};

template<typename ITEM_TYPE>
inline ITEM_ID LookupTable::CreateItem()
{
	map.reserve(++id_counter);
	ITEM_TYPE* derived_ptr = new ITEM_TYPE();
	map[id_counter] = static_cast<void*>(derived_ptr);
	++size;
	return id_counter;
}

template<typename ITEM_TYPE>
inline void LookupTable::DestroyItem(ITEM_ID id)
{
	ITEM_TYPE* derived_ptr = static_cast<ITEM_TYPE*>(map[id]);
	map.erase(id);
	--size;
	delete derived_ptr;
}

template<typename ITEM_TYPE>
inline ITEM_TYPE& LookupTable::get(ITEM_ID id)
{
	return *(static_cast<ITEM_TYPE*>(map[id]));
}

// throw an error if an item wasnt deleted
inline LookupTable::~LookupTable()
{
	for (auto item : map)
	{
		throw ErrMemoryLeak();
	}
}

inline std::vector<ITEM_ID> LookupTable::get_ids()
{
	std::vector<ITEM_ID> ids;
	ids.reserve(size);
	for (auto item : map)
	{
		ids.emplace_back(item.first);
	}
	return ids;
}