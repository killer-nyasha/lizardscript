#pragma once
#ifndef SSTP_INCLUDED
#include <iostream>
#include <string>

//реализовать всё это так, чтобы нормально работало
template <typename T>
struct byval_ptr
{
	struct byval_ptr_obj
	{
		size_t refcount;
		T object;
	};

	byval_ptr_obj* pointer;

	//или же создать новый экземпляр
	byval_ptr() { pointer = nullptr; }
	//byval_ptr(T* ptr) { pointer = new byval_ptr_obj; pointer->refcount = 1; pointer->object = ptr; }
	byval_ptr(std::nullptr_t) { pointer = nullptr; }
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
		if (t.pointer != nullptr)
			t.pointer->refcount++;
	}

	void operator=(const byval_ptr& t)
	{
		if (pointer == t.pointer)
			return;

		this->~byval_ptr();
		pointer = t.pointer;
		if (t.pointer != nullptr)
			t.pointer->refcount++;
	}

	void operator=(byval_ptr&& t)
	{
		if (pointer == t.pointer)
			return;

		this->~byval_ptr();
		pointer = t.pointer;
		if (t.pointer != nullptr)
			t.pointer->refcount++;
	}

	T& operator*() const { return pointer->object; }
	T* operator->() const { return &pointer->object; }


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

	bool isNullptr() { return pointer == nullptr; }

	friend std::ostream& operator<<(std::ostream& stream, byval_ptr& str)
	{
		if (str.pointer != nullptr)
			stream << str.pointer->object;
		else stream << "(nullptr)";
		return stream;
	}
};

struct stringptr : public byval_ptr<std::string>
{
	using byval_ptr<std::string>::byval_ptr;

	auto& operator[] (int index)
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

	std::string operator+(const char * cString)
	{
		**(this) + cString;
		return **(this);
	}
};

inline stringptr operator ""  _sp(const char* text, size_t size)
{
	return stringptr(std::string(text));
}

#endif