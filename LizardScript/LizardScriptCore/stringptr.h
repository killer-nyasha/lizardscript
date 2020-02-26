/*!
\file stringptr.h
\brief Simple shared pointer to std::string
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <iostream>
#include <string>

//!shared pointer with comparison by value, not by pointer
template <typename T>
class byval_ptr
{
protected:
	//!shared pointer data
	struct byval_ptr_obj
	{
		size_t refcount;
		T object;
	};

	byval_ptr_obj* pointer = nullptr;

public:

	//!init with nullptr
	byval_ptr() { }
	byval_ptr(std::nullptr_t) { }

	operator bool()
	{
		return *this != nullptr;
	}

	//byval_ptr(T* ptr) { pointer = new byval_ptr_obj; pointer->refcount = 1; pointer->object = ptr; }
	
	~byval_ptr()
	{
		if (pointer != nullptr)
		{
			pointer->refcount--;
			if (pointer->refcount == 0)
				delete pointer;
		}
	}

	byval_ptr(T&& t)
	{
		pointer = new byval_ptr_obj{ 1, std::move(t) };
	}

	byval_ptr(const byval_ptr& t)
	{
		pointer = t.pointer;
		if (t.pointer != nullptr)
			t.pointer->refcount++;
	}
	byval_ptr(byval_ptr&& t)
	{
		pointer = t.pointer;
		t.pointer = nullptr;
	}

	T& operator*() const { return pointer->object; }
	T* operator->() const { return &pointer->object; }

	//!if there are more then one references to object, it makes its copy and sets this pointer to it.
	//!it's useful when you want to modify string but don't want it to change in other places.
	void make_unique()
	{
		if (this->pointer->refcount > 1)
		{
			byval_ptr_obj* newobj = new byval_ptr_obj (*this->pointer);
			newobj->refcount = 1;
			this->pointer = newobj;
		}
	}

	bool operator ==(std::nullptr_t) const
	{
		return pointer == nullptr;
	}
	bool operator !=(std::nullptr_t) const
	{
		return pointer != nullptr;
	}

	bool operator ==(const byval_ptr& s) const
	{
		return pointer->object == s.pointer->object;
	}
	bool operator !=(const byval_ptr& s) const
	{
		return pointer->object != s.pointer->object;
	}
	bool operator <(const byval_ptr& s) const
	{
		return pointer->object < s.pointer->object;
	}
	bool operator <=(const byval_ptr& s) const
	{
		return pointer->object <= s.pointer->object;
	}
	bool operator >(const byval_ptr& s) const
	{
		return pointer->object > s.pointer->object;
	}
	bool operator >=(const byval_ptr& s) const
	{
		return pointer->object >= s.pointer->object;
	}

	friend std::ostream& operator<<(std::ostream& stream, const byval_ptr& str)
	{
		if (str.pointer != nullptr)
			stream << str.pointer->object;
		else stream << "(nullptr)";
		return stream;
	}
};

//!we can add there our own string functions
struct stringptr : public byval_ptr<std::string>
{
	using byval_ptr<std::string>::byval_ptr;

	auto& operator[](size_t index)
	{
		return (*this)->operator[](index);
	}

	bool hasEnding(const stringptr ending) const
	{
		if ((*this)->length() >= ending->length())
		{
			return (0 == (*this)->compare((*this)->length() - ending->length(),
				ending->length(), *ending));
		}
		else
		{
			return false;
		}
	}

	stringptr tokenBetween(const stringptr leftSeparator,
		const stringptr rightSeparator) const
	{
		auto start = (*this)->find_last_of(*leftSeparator) + 1;
		auto end = (*this)->find_last_of(*rightSeparator) - 3;
		return stringptr{ (*this)->substr(start, end - start) };
	}

	//why do we need this?
	stringptr& operator+(const char* cString)
	{
		**this += cString;
		return *this;
	}

	//!copies char* content to new std::string
	stringptr(const char* data) : byval_ptr(std::string(data))
	{
		
	}

	//!\warning returns temporary pointer
	operator const char* ()
	{
		return pointer->object.c_str();
	}
};

//!stringptr literal
inline stringptr operator "" _sp(const char* text, size_t size)
{
	return stringptr(std::string(text));
}
