

#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>

#include <Windows.h>
#include <ShObjIdl.h>
#include <atlbase.h> // CComPtr
#include <comdef.h> // _com_error

#define ADD_CLEANUP(janitor, strName, cleanUp) janitor.add_cleanup(strName, [&]() { cleanUp });


// i will be using this for a cleaner class for the application class
// so the order which 
template<typename KEY_TYPE, typename VALUE_TYPE>
class VectorMap
{
public:
	// i am a little retarded so im not quite sure how avoid the move
	// constructor with my little shit brain.
	// i know i *can* pass the arguments, then instantiate the object in
	// the vector or whatever but i am to restarted for that rn
	// and this isnt something that would be called multiple times per
	// second so it doesnt really matter
	void EmplaceFront(KEY_TYPE key, VALUE_TYPE& obj);
	void EmplaceFront(KEY_TYPE key, VALUE_TYPE&& obj);
	VALUE_TYPE& Access(const KEY_TYPE& key);
	VALUE_TYPE& Index(const size_t index);
	size_t GetSize();
	void Remove(const KEY_TYPE& key);
	// fuck it's annoying idk how to make a iterator for a vector
	// maybe i will try to implement a vector class from scratch

private:
	std::vector<KEY_TYPE> vector;
	std::unordered_map<KEY_TYPE, VALUE_TYPE> map;

};

template<typename KEY_TYPE, typename VALUE_TYPE>
inline void VectorMap<KEY_TYPE, VALUE_TYPE>::EmplaceFront(KEY_TYPE key, VALUE_TYPE& obj)
{
	vector.emplace(vector.begin(), key);
	map[key] = obj;
}

template<typename KEY_TYPE, typename VALUE_TYPE>
inline void VectorMap<KEY_TYPE, VALUE_TYPE>::EmplaceFront(KEY_TYPE key, VALUE_TYPE&& obj)
{
	vector.emplace(vector.begin(), key);
	map[key] = obj;
}

template<typename KEY_TYPE, typename VALUE_TYPE>
inline VALUE_TYPE& VectorMap<KEY_TYPE, VALUE_TYPE>::Access(const KEY_TYPE& key)
{
	return map[key];
}

template<typename KEY_TYPE, typename VALUE_TYPE>
inline VALUE_TYPE& VectorMap<KEY_TYPE, VALUE_TYPE>::Index(const size_t index)
{
	return map[vector[index]];
}

template<typename KEY_TYPE, typename VALUE_TYPE>
inline size_t VectorMap<KEY_TYPE, VALUE_TYPE>::GetSize()
{
	return vector.size();
}

template<typename KEY_TYPE, typename VALUE_TYPE>
void VectorMap<KEY_TYPE, VALUE_TYPE>::Remove(const KEY_TYPE& key)
{
	vector.erase(std::find(vector.begin(), vector.end(), key));
	map.erase(key);
}

class Janitor
{
public:
	~Janitor()
	{
		exit();
	}
	void add_cleanup(const std::string& key, std::function<void(void)>&& lambda)
	{
		cleanupFunctions.EmplaceFront(key, lambda);
	}
	void call_cleanup(const std::string& key)
	{
		cleanupFunctions.Access(key)();
		cleanupFunctions.Remove(key);
	}
	void drop_cleanup(const std::string& key)
	{
		cleanupFunctions.Remove(key);
	}
	void exit()
	{
		// call cleanup functions in reverse order of added
		for (size_t i = 0; i < cleanupFunctions.GetSize(); ++i)
		{
			cleanupFunctions.Index(i)();
		}
	}

private:
	VectorMap<std::string, std::function<void(void)>> cleanupFunctions;
};

inline void throw_if_failed(HRESULT&& hr)
{
	if (FAILED(hr))
		throw _com_error(hr);
}

// SMART POINTER / WRAPPER FOR COM
template<class PTR_TYPE>
class ComPtrWrapper
{
private:
	PTR_TYPE ptr;
	unsigned int* pcRef;
public:
	ComPtrWrapper() :
		ptr(nullptr),
		pcRef(new unsigned int(1))
	{}

	ComPtrWrapper(PTR_TYPE ptr) :
		ptr(ptr),
		pcRef(new unsigned int(1))
	{}

	ComPtrWrapper(const ComPtrWrapper& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{
		++*(this->pcRef);
	}

	ComPtrWrapper(ComPtrWrapper&& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{}

	~ComPtrWrapper()
	{
		--*(this->pcRef);
		if (*(this->pcRef) == 0)
		{
			CoTaskMemFree(ptr);
			delete pcRef;
		}
	}

	PTR_TYPE* operator&()
	{
		return &(this->ptr);
	}

	PTR_TYPE operator->()
	{
		return this->ptr;
	}

	PTR_TYPE get_ptr()
	{
		return this->ptr;
	}

	void operator*() = delete;

	ComPtrWrapper operator=(const ComPtrWrapper& other)
	{
		this->ptr = other.ptr;
		this->pcRef = other.pcRef;
		++*(this->pcRef);
	}

	ComPtrWrapper operator=(const ComPtrWrapper&& other)
	{
		this->ptr = other.ptr;
		this->pcRef = other.pcRef;
	}

	operator PTR_TYPE() const
	{
		return this->ptr;
	}

};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE _prevHI, PWSTR pCmdLine, int nCmdShow)
{
	// JANITOR CLASS APPROACH
	//		made more sense before CComPtr, just the raw shit yk
	//		it just isnt the best solution, when writing this i
	//		also wasnt sure if hr would be used for anything else
	//		beyond just (FAILED(hr))
	/*
	{
		Janitor jan;

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		jan.add_cleanup("COM", [&]() { CoUninitialize(); });
		if (FAILED(hr))
			return 0;
		CComPtr<IFileOpenDialog> pFileOpen;
		hr = pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog));
		if (FAILED(hr))
			return 0;

		hr = pFileOpen->Show(NULL);
		if (FAILED(hr))
			return 0;

		IShellItem* pItem;
		hr = pFileOpen->GetResult(&pItem);
		jan.add_cleanup("SHELLITEM", [&]() { pItem->Release(); });
		if (FAILED(hr))
			return 0;

		PWSTR pszFilePath;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
		jan.add_cleanup("FILEPATH", [&]() { CoTaskMemFree(pszFilePath); });
		if (FAILED(hr))
			return 0;

		MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
	}
	*/


	// THROW ON FAIL APPROACH
	{
		try 
		{
			throw_if_failed(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

			CComPtr<IFileOpenDialog> pFileOpen;
			throw_if_failed(pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog)));
			
			throw_if_failed(pFileOpen->Show(NULL));
			
			CComPtr<IShellItem> pItem;
			throw_if_failed(pFileOpen->GetResult(&pItem));

			ComPtrWrapper<PWSTR> pszFilePath;
			throw_if_failed(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath));

			MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
		}
		catch (_com_error)
		{
			std::cerr << "_com_error (smd idk, go debug)";
			// handle error
		}
		CoUninitialize();
	}
}

