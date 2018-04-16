#pragma once

#include <vector>
#include "Multiset.h"
#include "KMin.h"
#include "HyperLogLog.h"

class ExperimentHandler
{
public:
	ExperimentHandler();
	
	void RunExp1CalcRatio();
	unsigned int RunExp2CalcKWithChernoff(unsigned int n = 10000);
	void RunExp3CheckKValid(int k);

	template <class HashAlg>
	void RunExp4ShadyResults(HashAlg hashAlg);

	void RunExp5CalcDelta();

	void RunHyperLogLogDifferentM();
	void RunCompareHyperloglogAndKMinForSameAmountOfMemory();

protected:
	double CalcChernoff(double x, unsigned int k);
	bool CheckProbabilityRange(double rangePercent, double prob, unsigned int k);
	bool CheckIfCorrectEpsilon(double epsilon, std::vector<double> kMinEstymatorVec, int n, double probability, Multiset &multiset);
	double CalcChernoffEpsilon(unsigned int k, double probability);
	double CalcCzybyszewEps(unsigned int k, unsigned int n, double probability);

	template <class HashAlg>
	bool CheckIfCorrectRatio(Multiset &multiset, unsigned int k, unsigned int n, double probability, HashAlg hashAlg);
};

template <class HashAlg>
bool ExperimentHandler::CheckIfCorrectRatio(Multiset &multiset, unsigned int k, unsigned int n, double probability, HashAlg hashAlg)
{
	LD nEstymator = kMin(k, hashAlg, multiset);
	LD ratio = nEstymator / (LD)multiset.GetSize();
	std::cout << "ratio: " << ratio << "\n";
	return (std::abs(ratio - (LD)1.0) < (LD)probability);
}

// dla jakiego n sa zaburzone wyniki, paradoks urodzinowy, powinno byc dla 2lg n = B, gdzie B = 32, wiec n = 65536
template <class HashAlg>
void ExperimentHandler::RunExp4ShadyResults(HashAlg hashAlg)
{
	double probability = 0.1;
	unsigned int optimalK = 752;
	unsigned int n = 1;
	n <<= 6; //64

	Multiset multiset;
	multiset.SetSize(n);

	std::cout << "check results for n = " << n << ", 2^" << std::log2(n) << "\n";
	while (CheckIfCorrectRatio(multiset, optimalK, n, probability, hashAlg))
	{
		n <<= 1;
		multiset.SetSize(n);
		std::cout << "check results for n = " << n << ", 2^" << std::log2(n) << "\n";
	}

	std::cout << "Shady results for n = " << n << ", 2^" << std::log2(n) << "\n";
}
