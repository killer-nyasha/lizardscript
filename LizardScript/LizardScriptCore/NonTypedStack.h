/*!
\file NonTypedStack.h
\brief Stores elements with different sizes and types.
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include "interpreter_util.h"
#include "TypeInfo.h"

//!All genius is simple ^_^
//struct Dynamic
//{
//	TypeInfo type;
//	void* value;
//
//	Dynamic(TypeInfo type, void* value) : type(type), value(value)
//	{
//
//	}
//};

//!Each element has different size, but stack saves its type metadata so we still can push and pop.
//todo: I should write functions with move-semantic
//class NonTypedStack
//{
//	//!Size of alocated memory.
//	size_t capacity;
//
//	//!Pointer to allocated memory.
//	unsigned char* data;
//
//	//!Count of bytes which we are using at the moment.
//	size_t size = 0;
//
//public:
//
//	//!Count of objects in a stack. It's easier than compute count of elements. and O(1).
//	size_t count = 0;
//
//	NonTypedStack(size_t capacity = 1024)
//	{
//		this->capacity = capacity;
//		if (capacity == 0)
//			data = nullptr;
//		else data = (unsigned char*)malloc(capacity);
//	}
//
//	//!\warning I'm to lazy to implement this right now.
//	NonTypedStack(const NonTypedStack& other) = delete;
//
//	NonTypedStack(NonTypedStack&& other)
//	{
//		data = other.data;
//		capacity = other.capacity;
//		size = other.size;
//		other.data = nullptr;
//	}
//
//	~NonTypedStack()
//	{
//		free(data);
//	}
//
//	bool isEmpty()
//	{
//		return size == 0;
//	}
//
//	//!checks if stack has enough space to push element of this type 
//	template <typename T>
//	bool check(const T& value)
//	{
//		return size + sizeof(TypeInfo) + typeInfo<T>().size() < capacity;
//	}
//
//	template <typename T>
//	void push(const T& value)
//	{
////		if (!check(value))
////		{
////			capacity *= 2;
////			realloc(data, capacity);
////		}
////
////		TypeInfo info = TYPEINFO(T);
////		*reinterpret_cast<T*>(data[size]) = value;
////		size += info.size();
////		*reinterpret_cast<TypeInfo*>(data[size]) = info;
////		size += sizeof(TypeInfo);
////		count++;
//	}
//
//	//!\warning Dynamic instanse become invalid when you use push
//	Dynamic pop()
//	{
//		size -= sizeof(TypeInfo);
//		TypeInfo info = *reinterpret_cast<TypeInfo*>(data[size]);
//		size -= info.size();
//		void* ptr = static_cast<void*>(&data[size]);
//		count--;
//		return Dynamic(info, ptr);
//	}
//};
