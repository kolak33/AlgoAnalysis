#pragma once


#include "Multiset.h"
#include <algorithm>
#include <vector>


class Hyper
{
public:
	Hyper()
	{
	}

	template <class hashFunc>
	static LD HyperLogLog(unsigned int b, hashFunc hash, Multiset &multiset);

protected:
	static int FindMostSignificantBitPosition(unsigned int n, unsigned int b);
	static double CalcEstimator(std::vector<int> &M);
};

template <class hashFunc>
LD Hyper::HyperLogLog(unsigned int b, hashFunc hash, Multiset &multiset)
{
	int m = (int)std::pow(2, b);
	std::vector<int> M(m, 0);

	unsigned int offsetSubstream = (1 << b) - 1;
	unsigned int offsetCutHash = (1 << (32 - b)) - 1;

	std::vector<int> substrCnt(m, 0);

	for (LL iter = 0; iter < multiset.GetSize(); ++iter)
	{
		LL nextValue = multiset.GetNextValue();
		unsigned int hashValue = (unsigned int)hash((const void *)&nextValue, sizeof(LL));

		int substreamNumber = (hashValue & (offsetSubstream << (32 - b))) >> (32 - b);
		int cutHashValue = hashValue & offsetCutHash;

		int msb = FindMostSignificantBitPosition(cutHashValue, b);
		M[substreamNumber] = std::max(M[substreamNumber], msb);
	}

	double estimator = CalcEstimator(M);

	if (estimator <= 2.5 * m) /* fix for small numbers */
	{
		int v = std::count(M.begin(), M.end(), 0);
		if (v != 0)
			estimator = -1.0 * m * std::log((double)v / (double)m);
	}
	else if (estimator > (1.0 / 7.5) * std::pow(2, 30)) /* fix for large numbers */
	{
		estimator = -1.0 * std::pow(2.0, 30.0) * std::log(1.0 - (estimator / std::pow(2.0, 30.0) / 4.0)) * 4.0;
	}

	return estimator;
}
