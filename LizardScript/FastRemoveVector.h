#pragma once
#include <array>
#include <vector>

template <typename T, size_t size>
class FastRemoveArray
{
private:
	//!Вектор, содержащий неупорядоченные объекты.
	std::array<T, size> data;

	int nextIndex;

	//!Вектор, содержащий свободные индексы.
	//!Когда требуется удалить объект, вместо того, чтобы сдвигать на 1 шаг назад все стоящие после него, индекс просто
	//!добавляется в данный вектор, и следующий добавленный объект займёт место удалённого.
	std::vector<size_t> freeIndex;

public:

	size_t push_back(T& object)
	{
		size_t index = 0;
		if (freeIndex.size() == 0)
		{
			index = nextIndex;
			data[nextIndex++] = object;
		}
		else
		{
			index = freeIndex[freeIndex.size() - 1];
			data[index] = object;
			freeIndex.pop_back();
		}
		return index;
	}

	void erase(size_t index)
	{
		if (index == nextIndex - 1)
			nextIndex--;
		else
			freeIndex.push_back(index);
	}

	T& operator[](size_t index)
	{
		return data[index];
	}

	bool isFull()
	{
		return (freeIndex.size() == 0) && (nextIndex == size);
	}

	bool isEmpty()
	{
		return nextIndex - freeIndex.size() == 0;
	}
};

template <typename T>
class FastRemoveVector
{
private:
	//!Вектор, содержащий неупорядоченные объекты.
	std::vector<T> data;

	//!Вектор, содержащий свободные индексы.
	//!Когда требуется удалить объект, вместо того, чтобы сдвигать на 1 шаг назад все стоящие после него, индекс просто
	//!добавляется в данный вектор, и следующий добавленный объект займёт место удалённого.
	std::vector<size_t> freeIndex;

public:

	size_t push_back(T& object)
	{
		size_t index = 0;
		if (freeIndex.size() == 0)
		{
			index = data.size();
			data.push_back(object);
		}
		else
		{
			index = freeIndex[freeIndex.size() - 1];
			data[index] = object;
			freeIndex.pop_back();
		}
		return index;
	}

	void erase(size_t index)
	{
		freeIndex.push_back(index);
	}

	T& operator[](size_t index)
	{
		return data[index];
	}
};