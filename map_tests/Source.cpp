#pragma once


#include <exception>
#include <unordered_map>
#include <iostream>

struct pos
{
	float x = 0;
	float y = 0;
};

struct pos_name : pos
{
	float z = 5;
};


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

template<class BASE_CLASS>
class LookupTable
{
public:
	LookupTable() = default;
	~LookupTable();

	template<class DERIVED_CLASS>
	ITEM_ID CreateItem();

	template<class DERIVED_CLASS>
	void DestroyItem(ITEM_ID id);

	template<typename DERIVED_CLASS>
	DERIVED_CLASS& get(ITEM_ID id);

	// returns vector of current ids in the lookup table
	std::vector<ITEM_ID> get_ids();

private:
	std::unordered_map<ITEM_ID, void*> map;
	// not current amount of ids, just amount created in total
	ITEM_ID id_counter = 0;
	int size;
};

template<class BASE_CLASS>
template<class DERIVED_CLASS>
inline ITEM_ID LookupTable<BASE_CLASS>::CreateItem()
{
	map.reserve(++id_counter);
	DERIVED_CLASS* drawable_ptr = new DERIVED_CLASS();
	map[id_counter] = static_cast<void*>(drawable_ptr);
	++size;
	return id_counter;
}

template<class BASE_CLASS>
template<class DERIVED_CLASS>
inline void LookupTable<BASE_CLASS>::DestroyItem(ITEM_ID id)
{
	DERIVED_CLASS* drawable_ptr = static_cast<DERIVED_CLASS*>(map[id]);
	map.erase(id);
	--size;
	delete drawable_ptr;
}

template<class BASE_CLASS>
template<class DERIVED_CLASS>
inline DERIVED_CLASS& LookupTable<BASE_CLASS>::get(ITEM_ID id)
{
	return *(static_cast<DERIVED_CLASS*>(map[id]));
}

template<class BASE_CLASS>
LookupTable<BASE_CLASS>::~LookupTable()
{
	for (auto item : map)
	{
		throw ErrMemoryLeak();
	}
}

template<class BASE_CLASS>
std::vector<ITEM_ID> LookupTable<BASE_CLASS>::get_ids()
{
	std::vector<ITEM_ID> ids;
	ids.reserve(size);
	for (auto item : map)
	{
		ids.emplace_back(item.first);
	}
	return ids;
}


int main()
{
	LookupTable<pos> polymMap;
	ITEM_ID id = polymMap.CreateItem<pos_name>();
	ITEM_ID id2 = polymMap.CreateItem<pos_name>();
	ITEM_ID id3 = polymMap.CreateItem<pos_name>();
	ITEM_ID id4 = polymMap.CreateItem<pos_name>();
	ITEM_ID id5 = polymMap.CreateItem<pos_name>();
	ITEM_ID id6 = polymMap.CreateItem<pos_name>();
	ITEM_ID id7 = polymMap.CreateItem<pos_name>();
	polymMap.DestroyItem<pos_name>(id3);

	std::vector<ITEM_ID> ids;
	ids = polymMap.get_ids();

	polymMap.get<pos_name>(id).x = 5;
	pos_name my_pos = polymMap.get<pos_name>(id);

	polymMap.DestroyItem<pos_name>(id);
	polymMap.DestroyItem<pos_name>(id2);
	polymMap.DestroyItem<pos_name>(id4);
	polymMap.DestroyItem<pos_name>(id5);
	polymMap.DestroyItem<pos_name>(id6);
	polymMap.DestroyItem<pos_name>(id7);


}


