#include "stdafx.h"
#include "HyperLogLog.h"

double Hyper::CalcEstimator(std::vector<int> &M)
{
	double harmonicAvg = 0.0;
	for (int i = 0; i < M.size(); ++i)
	{
		harmonicAvg += std::pow(2.0, (double)(-1.0 * M[i]));
	}

	return M.size() * M.size() * 0.7 * (1.0 / harmonicAvg);
}

int Hyper::FindMostSignificantBitPosition(unsigned int n, unsigned int b)
{
	unsigned int offset = 1 << (31 - b);
	for (int bit = b + 1; bit <= 32 && offset != 0; ++bit)
	{
		if (n & offset)
			return (bit - b);
		offset >>= 1;
	}
	return 0;
}