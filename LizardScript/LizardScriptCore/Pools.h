/*!
\file Pools.h
\brief 1000 ways to use ObjectPool pattern for any case
\author killer-nyasha
\version 0.2
\date 26.00.2020
*/
#pragma once
#include <stack>
#include <functional>

#include "FastRemoving.h"
#include "Exception.h"

//!pool which operates with fixed-size blocks of memory
template <typename T, size_t capacity, size_t elemSize, bool linkedList>
class MemoryPool
{
private:
	//!pointer to next pool, if there aren't free place in this.
	//!works as linked list.
	MemoryPool* nextPool;

	//!just some bytes
	struct dummy
	{ char place[elemSize]; };

	//!storage
	FastRemoving<ArrayAsVectorWrapper<dummy, capacity>> heap;
	
public:

	//!allocate object in pool
	T* alloc()
	{
		if (heap.isFull())
		{
			if (linkedList)
			{
				nextPool = new MemoryPool();//create();
				std::cout << typeid(T).name() << " pool overflow!\n";
				return nextPool->alloc();
			}
			else
				throw std::exception("MemoryPool overflow");
		}
		else
		{
			dummy d = dummy();
			size_t index = heap.push_back(d);
			return (T*)&heap[index];
		}
	}

	//!free object
	void free(T* elem)
	{
		if (elem == nullptr)
			return;

		size_t index = elem - (T*)&heap[0];
		if (index >= 0 && index < capacity)
			heap.erase(index);
		else if (nextPool) nextPool->free(elem);
		else throw Exception("are you sure that it's an object from pool?");
	}

	//!recursive deletion of pools
	~MemoryPool()
	{
		delete nextPool;
	}
};

//!affects only speed and memory requirements
#define SIMPLE_MEMORY_POOL_DEFAULT_SIZE 4096

template <typename T, bool linkedList>
using SimpleMemoryPool = MemoryPool<T, SIMPLE_MEMORY_POOL_DEFAULT_SIZE / sizeof(T), sizeof(T), true>;

//!pool which is static and unique for each type.
//!\warning it creates objects only using default constructor
template <typename T>
class StaticPool
{
public:
	//!storage
	static std::stack<T*> objects;

	//!does nothing
	template <typename T>
	static void clear(T* object) { }

	//!clears vector after reuse
	template <typename T>
	static void clear(std::vector<T>* object) { object->clear(); }

	//!clears stack after reuse
	template <typename T>
	static void clear(std::stack<T>* object) { while (objects.size() > 0) objects.pop(); }

	//!get object from pool or create (in heap) with default constructor
	//!\warning it returns a pointer so you must return it to pool manually
	static T* alloc()
	{
		if (objects.size() == 0)
			objects.push(new T());

		T* object = objects.top();
		objects.pop();
		//pool_init(object);

		return /*std::move(*//*)*/object;
	}

	//!clear object and return it to the pool 
	static void free(T* value)
	{
		clear(value);
		objects.push(/*std::move(*/value/*)*/);
	}
};

//!static pool storage implementation
template <typename T>
std::stack<T*> StaticPool<T>::objects;

//!smart pointer to an object from pool
//!it automatically gets object from pool on creation and returns it on destruction
//!haven't move constructor so it's a bit like std::unique_ptr
template <typename T>
struct PoolPointer
{
private:
	//!raw pointer
	T* pointer;

public:
	//!get object from the pool
	PoolPointer()
	{
		pointer = StaticPool<T>::alloc();
	}

	PoolPointer(const PoolPointer& c) = delete;

	//!move semantic
	PoolPointer(PoolPointer&& m)
	{
		pointer = m.pointer;
		m.pointer = nullptr;
	}

	PoolPointer& operator=(PoolPointer&& ptr)
	{
		this->~PoolPointer();
		this->PoolPointer::PoolPointer(std::forward<PoolPointer<T>>(ptr));
		return *this;
	}

	//!return object to the pool
	~PoolPointer()
	{
		if (pointer != nullptr)
			StaticPool<T>::free(pointer);
	}

	T* operator->()
	{
		return pointer;
	}

	const T* operator->() const
	{
		return pointer;
	}

	T& operator*()
	{
		return *pointer;
	}

	const T& operator*() const
	{
		return *pointer;
	}
};