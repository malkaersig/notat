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
class SHPTR
{
private:
	unsigned int* pcRef;

protected:
	PTR_TYPE ptr;
	virtual void DestroyPtr()
	{
		delete this->ptr;
	}

public:
	SHPTR() :
		ptr(nullptr),
		pcRef(new unsigned int(1))
	{}

	SHPTR(PTR_TYPE ptr) :
		ptr(ptr),
		pcRef(new unsigned int(1))
	{}

	SHPTR(PTR_TYPE& ptr) :
		ptr(ptr),
		pcRef(new unsigned int(1))
	{}

	SHPTR(const SHPTR& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{
		++*(this->pcRef);
	}

	SHPTR(SHPTR&& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{}

	~SHPTR()
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

	SHPTR operator=(const SHPTR& other)
	{
		this->ptr = other.ptr;
		this->pcRef = other.pcRef;
		++*(this->pcRef);
	}

	SHPTR operator=(const SHPTR&& other)
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
class ComPtrWrapper : public SHPTR<PTR_TYPE>
{
protected:
	void DestroyPtr() override
	{
		CoTaskMemFree(this->ptr);
	}
};

// SMART POINTER FOR ARRAY
template<typename PTR_TYPE>
class ARRSHPTR : public SHPTR<PTR_TYPE>
{
protected:
	void DestroyPtr() override
	{
		delete[] this->ptr;
	}
};

