#pragma once
#include <stack>
#include "FastRemoveVector.h"

template <typename T, size_t capacity, size_t elemSize, bool linkedList>
class IMemoryPool
{
private:
	IMemoryPool* nextPool;

	struct dummy
	{ char place[elemSize]; };

	FastRemoveArray<dummy, capacity> heap;
	
public:

	virtual IMemoryPool* create() = 0;

	T* alloc()
	{
		if (heap.isFull())
		{
			if (linkedList)
			{
				nextPool = create();
				std::cout << typeid(T).name() << " pool overflow!\n";
				return nextPool->alloc();
			}
			else
				throw std::exception("MemoryPool переполнен");
		}
		else
		{
			dummy d = dummy();
			size_t index = heap.push_back(d);
			return (T*)&heap[index];
		}
	}

	void free(T* elem)
	{
		if (elem == nullptr)
			return;

		size_t index = elem - (T*)&heap[0];
		if (index >= 0 && index < size)
			heap.erase(index);
		else nextPool->free(elem);
	}

	~IMemoryPool()
	{
		delete nextPool;
	}
};

template <typename R, size_t capacity, size_t elemSize, bool linkedList>
class FixedMemoryPool : public IMemoryPool<R, capacity, elemSzie, linkedList>
{
public:

	virtual FixedMemoryPool* create() override
	{
		return new FixedMemoryPool();
	}
};

template <typename R, size_t capacity, bool linkedList>
class MemoryPool : public IMemoryPool<R, capacity, sizeof(R), linkedList>
{
public:

	virtual MemoryPool* create() override
	{
		return new MemoryPool();
	}
};

template <typename R, size_t capacity>
class StackPool
{
	std::function<void(std::stack<R>&)> onExpand;

	StackPool(decltype(onExpand) onExpand)
		: onExpand(onExpand)
	{

	}

	std::stack<R> objects;

	R* alloc()
	{
		if (objects.size() == 0)
			onExpand();

		return objects.pop();
	}

	void free(R* value)
	{
		objects.push_back(value);
	}
};

template <typename T>
class StaticPool
{
public:
	static std::stack<T*> objects;

	template <typename T>
	static void clear(T* object) { }

	template <typename T>
	static void clear(std::vector<T>* object) { object->clear(); }

	template <typename T>
	static void clear(std::stack<T>* object) { while (st->size() > 0) st->pop(); }

	static T* alloc()
	{
		if (objects.size() == 0)
			objects.push(new T());

		T* object = objects.pop();
		pool_init(object);

		return /*std::move(*//*)*/object;
	}

	static void free(T* value)
	{
		clear(value);
		objects.push_back(/*std::move(*/value/*)*/);
	}
};

template <typename T>
std::stack<T*> StaticPool<T>::objects;

template <typename T>
struct PoolPointer
{
	T* pointer;

	PoolPointer()
	{
		pointer = StaticPool<T>::alloc();
	}

	PoolPointer(const PoolPointer& c) = delete;

	PoolPointer(PoolPointer&& m)
	{
		pointer = m.pointer;
		m.pointer = nullptr;
	}

	~PoolPointer()
	{
		if (pointer != nullptr)
			StaticPool<T>::free(pointer);
	}

	T* operator->()
	{
		return pointer;
	}

	T* operator*()
	{
		return pointer;
	}
};