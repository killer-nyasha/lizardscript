#pragma once
#include "interpreter_util.h"

//!all genius is simple ^__^
struct Dynamic
{
	TypeInfo type;
	void* value;

	Dynamic(TypeInfo type, void* value) : type(type), value(value)
	{

	}
};

//!Each element has different size, but stack saves its type metadata so we still can push and pop.
class NonTypedStack
{
	//!Size of alocated memory.
	size_t capacity;

	//!Pointer to allocated memory.
	unsigned char* data;

	//!Count of bytes which we are using at the moment.
	size_t size = 0;

	//!Count of objects in a stack. It's easier than compute count of elements. and O(1).
	size_t count = 0;

public:

	NonTypedStack(size_t capacity = 1024)
	{
		this->capacity = capacity;
		if (capacity == 0)
			data = nullptr;
		else data = new unsigned char[capacity];
	}

	NonTypedStack(const NonTypedStack& other) = delete;

	NonTypedStack(NonTypedStack&& other)
	{
		data = other.data;
		capacity = other.capacity;
		size = other.size;
		other.data = nullptr;
	}

	~NonTypedStack()
	{
		delete[] data;
	}

	bool isEmpty()
	{
		return size == 0;
	}

	size_t count()
	{

	}

	template <typename T>
	bool check(const T& value)
	{
		return size + sizeof(TypeInfo) + TYPEINFO(T).size() < capacity;
	}

	template <typename T>
	void push(const T& value)
	{
		TypeInfo info = TYPEINFO(T);
		*reinterpret_cast<T*>(data[size]) = value;
		size += info.size();
		*reinterpret_cast<TypeInfo*>(data[size]) = info;
		size += sizeof(TypeInfo);
		count++;
	}

	//template <typename T>
	//void push(T&& value)
	//{
	//	*reinterpret_cast<T*>(data[size]) = std::move(value);
	//}

	Dynamic pop()
	{
		size -= sizeof(TypeInfo);
		TypeInfo info = *reinterpret_cast<TypeInfo*>(data[size]);
		size -= info.size();
		void* ptr = static_cast<void*>(&data[size]);
		count--;
		return Dynamic(info, ptr);
	}
};
