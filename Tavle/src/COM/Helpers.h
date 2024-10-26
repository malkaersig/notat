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


template<typename PTR_TYPE>
class SHRPTR
{
private:
	PTR_TYPE ptr;
	unsigned int* pcRef;

protected:
	virtual void DestroyPtr()
	{
		delete ptr;
	}

public:
	SHRPTR() :
		ptr(nullptr),
		pcRef(new unsigned int(1))
	{}

	SHRPTR(PTR_TYPE ptr) :
		ptr(ptr),
		pcRef(new unsigned int(1))
	{}

	SHRPTR(const SHRPTR& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{
		++*(this->pcRef);
	}

	SHRPTR(SHRPTR&& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{}

	~SHRPTR()
	{
		--*(this->pcRef);
		if (*(this->pcRef) == 0)
		{
			DestroyPtr();
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

	PTR_TYPE GetPtr()
	{
		return this->ptr;
	}

	void operator*() = delete;

	SHRPTR operator=(const SHRPTR& other)
	{
		this->ptr = other.ptr;
		this->pcRef = other.pcRef;
		++*(this->pcRef);
	}

	SHRPTR operator=(const SHRPTR&& other)
	{
		this->ptr = other.ptr;
		this->pcRef = other.pcRef;
	}

	operator PTR_TYPE() const
	{
		return this->ptr;
	}

};


// SMART POINTER / WRAPPER FOR COM
template<typename PTR_TYPE>
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

	PTR_TYPE GetPtr()
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

