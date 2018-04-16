#pragma once

#include <vector>
#include "Multiset.h"

#include <algorithm>

template <typename T>
void insertion_sort(std::vector<T> &data)
{
	for (unsigned int i = 1, j; i<data.size(); i++)
	{
		T tmp = data[i];

		for (j = i; j>0 && tmp < data[j - 1]; j--)
			data[j] = data[j - 1];
		data[j] = tmp;
	}
}

template <class hashFunc>
LD kMin(LL k, hashFunc hash, Multiset &multiset)
{
	std::vector<LD> M(k, (LD)1.0);

	for (LL iter = 0; iter < multiset.GetSize(); ++iter)
	{
		LL nextValue = multiset.GetNextValue();
		LD hashValue = hash((const void *)&nextValue, sizeof(LL));
		hashValue = hashValue / UINT32_MAX;

		if (hashValue < M.back() && !std::binary_search(M.begin(), M.end(), hashValue)/*std::find(M.begin(), M.end(), hashValue) == M.end()*/)
		{
			M[M.size() - 1] = hashValue;
			insertion_sort(M);
		}
	}

	if (M.back() == (LD)1.0)
	{
		return (M.size() - std::count(M.begin(), M.end(), (LD)1.0));
	}
	else
	{
		return (LD)(k - 1) / M.back();
	}
}