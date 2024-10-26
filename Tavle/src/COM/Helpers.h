#pragma once

#include "TavleInclude.h"

template<class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

inline void throw_if_failed(HRESULT& hr)
{
	if (FAILED(hr))
		throw _com_error(hr);
}

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