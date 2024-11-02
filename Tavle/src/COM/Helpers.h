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
class ASHPTR
{
protected:
	PTR_TYPE ptr;
	unsigned int* pcRef;

public:
	virtual ~ASHPTR() = default;

	ASHPTR() :
		ptr(nullptr),
		pcRef(new unsigned int(1))
	{}

	ASHPTR(PTR_TYPE ptr) :
		ptr(ptr),
		pcRef(new unsigned int(1))
	{}

	ASHPTR(PTR_TYPE& ptr) :
		ptr(ptr),
		pcRef(new unsigned int(1))
	{}

	ASHPTR(const ASHPTR& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{
		++*(this->pcRef);
	}

	ASHPTR(ASHPTR&& other) :
		ptr(other.ptr),
		pcRef(other.pcRef)
	{}


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

	ASHPTR operator=(const ASHPTR& other)
	{
		this->ptr = other.ptr;
		this->pcRef = other.pcRef;
		++*(this->pcRef);
	}

	ASHPTR operator=(const ASHPTR&& other)
	{
		this->ptr = other.ptr;
		this->pcRef = other.pcRef;
	}

	operator PTR_TYPE() const
	{
		return this->ptr;
	}


};


template<typename PTR_TYPE>
class SHPTR : public ASHPTR<PTR_TYPE>
{
public:
	~SHPTR() override
	{
		--*(this->pcRef);
		if (*(this->pcRef) == 0)
		{
			delete this->ptr;
			delete this->pcRef;
		}
	}

};


// SMART POINTER / WRAPPER FOR COM
template<typename PTR_TYPE>
class ComPtrWrapper : public ASHPTR<PTR_TYPE>
{
public:
	~ComPtrWrapper() override
	{
		--*(this->pcRef);
		if (*(this->pcRef) == 0)
		{
			CoTaskMemFree(this->ptr);
			delete this->pcRef;
		}
	}

};

// SMART POINTER FOR ARRAY
template<typename PTR_TYPE>
class ARRSHPTR : public ASHPTR<PTR_TYPE>
{
public:
	~ARRSHPTR() override
	{
		--*(this->pcRef);
		if (*(this->pcRef) == 0)
		{
			delete[] this->ptr;
			delete this->pcRef;
		}
	}
};

