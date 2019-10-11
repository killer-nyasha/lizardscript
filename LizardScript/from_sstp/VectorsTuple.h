#pragma once
#include <vector>
#include <map>

template <typename... T>
class VectorsTuple
{
public:
	std::tuple <std::vector<T>...> vectors;

	template <typename G>
	std::vector<G>& get()
	{
		return std::get<std::vector<G>>(vectors);
	}

	template <typename G>
	const std::vector<G>& get() const
	{
		return std::get<std::vector<G>>(vectors);
	}

	template <typename G>
	void push_back(G& value)
	{
		std::get<std::vector<G>>(vectors).push_back(value);
	}

	template <typename G>
	void push_back(G&& value)
	{
		std::get<std::vector<G>>(vectors).push_back(value);
	}

	void sort()
	{
		size_t dummy[] = { (QuickSort(std::get<std::vector<T>>(vectors)), 0U)... };
	}
};

template <typename K, typename... T>
class MapsTuple
{
public:
	std::tuple <std::map<K, T>...> maps;

	template <typename G>
	void insert(K key, G& value)
	{
		std::get<std::map<K, G>>(maps).insert(std::make_pair(key, value));
	}
};