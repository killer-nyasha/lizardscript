/*!
\file VectorsTuple.h
\brief tuple of vectors or maps
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <vector>
#include <map>

//!contains some vectors of different types T...
//!we can add element of any type from T...
//!and it will be added to vector of its type automatically
template <typename... T>
class VectorsTuple
{
public:
	std::tuple <std::vector<T>...> vectors;

	//!get vector of some type
	//!\returns reference to vector of G
	template <typename G>
	std::vector<G>& get()
	{
		return std::get<std::vector<G>>(vectors);
	}

	//!get const vector of some type
	//!\returns const reference to vector of G
	template <typename G>
	const std::vector<G>& get() const
	{
		return std::get<std::vector<G>>(vectors);
	}

	//!push element
	//!\warning T... must contain G
	template <typename G>
	void push_back(G& value)
	{
		get<G>().push_back(value);
	}

	//!push element
	//!\warning T... must contain G
	template <typename G>
	void push_back(G&& value)
	{
		get<G>().push_back(value);
	}

	//!sort all vectors using quicksort
	void sort()
	{
		size_t dummy[] = { (quickSort(get<T>()), 0U)... };
	}
};

//!contains some maps of types T... 
//!all maps have key's type K
//!we can add element of any type from T...
//!and it will be added to map of its type automatically
template <typename K, typename... T>
class MapsTuple
{
public:
	std::tuple <std::map<K, T>...> maps;

	//!get map of some type
	//!\returns reference to map of G
	template <typename G>
	std::map<K, G>& get()
	{
		return std::get<std::map<K, G>>(maps);
	}

	//!get const map of some type
	//!\returns const reference to map of G
	template <typename G>
	const std::map<K, G>& get() const
	{
		return std::get<std::map<K, G>>(maps);
	}

	//!insert key and value
	//!\warning T... must contain G
	template <typename G>
	void insert(K key, G& value)
	{
		get<G>().insert(std::make_pair(key, value));
	}
};