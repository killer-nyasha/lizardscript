#pragma once
#include <vector>

template <typename T> 
inline size_t MedianOfThree(T &a, T &b, T &c, size_t ai, size_t bi, size_t ci)
{
	if (a >= b && a <= c)
		return ai;
	if (b >= a && b <= c)
		return bi;
	return ci;
}

template <typename T> 
inline size_t QuicksortPartition(std::vector<T> &A, size_t lo, size_t hi)
{
	size_t ci = (int)(0.5*(lo + hi) + 0.5);
	size_t pivotIndex = MedianOfThree(A[lo], A[ci], A[hi], lo, ci, hi);
	T pivot = A[pivotIndex];

	size_t i = lo;
	size_t j = hi;
	while (true)
	{
		while (A[i] <= pivot && i < pivotIndex) i++;
		while (A[j] >= pivot && j > pivotIndex) j--;
		if (i == pivotIndex && j == pivotIndex)
			return pivotIndex;

		T tmp = A[i];
		A[i] = A[j];
		A[j] = tmp;

		if (i == pivotIndex)
			pivotIndex = j;
		else if (j == pivotIndex)
			pivotIndex = i;
	}
}

template <typename T> 
inline void Quicksort(std::vector<T> &A, size_t lo, size_t hi)
{
	if (lo < hi)
	{
		size_t p = QuicksortPartition<T>(A, lo, hi);
		if (p > 0) //p is unsigned, so prevent negative
			Quicksort<T>(A, lo, p - 1);
		Quicksort<T>(A, p + 1, hi);
	}
}

template <typename T>
inline bool isSorted(const std::vector<T> &A)
{
	for (int i = 0; i < (int)A.size() - 1; i++)
		if (A[i] > A[i + 1])
			return false;
	return true;
}

template <typename T> 
inline void QuickSort(std::vector<T> &A)
{
	if (A.size() <= 1)
		return; 
	Quicksort(A, 0, A.size() - 1);
}

template <typename T>
inline int BinarySearch(const std::vector<T>& a, const T& x)
{
	if (a.size() == 0)
		return -1;

	int left = 0;
	int right = a.size() - 1;
	while (left <= right)
	{
		size_t middle = (left + right) / 2;
		if (a[middle] == x)
		{
			return (int)middle;
		}
		if (a[middle] < x)
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}

	return -1;
}