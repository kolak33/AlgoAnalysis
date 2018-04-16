#include "stdafx.h"
#include "ExperimentHandler.h"

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "Hash.h"

ExperimentHandler::ExperimentHandler()
{
}

void ExperimentHandler::RunExp1CalcRatio()
{
	std::clock_t start = std::clock();
	std::vector<unsigned int> kValues = { 2, 3, 10, 100, 400 };
	int n = 10000;
	Multiset multiset;
	for (int k : kValues)
	{
		std::cout << "start experymentu dla k = " << k << std::endl;
		std::string fileName = "k=" + std::to_string(k) + ".txt";
		std::ofstream file(fileName.c_str());

		if (!file.good())
		{
			std::cout << "blad otwarcia pliku " << fileName << std::endl;
			return;
		}

		for (int i = 1; i <= n; ++i)
		{
			multiset.SetSize(i);

			LD nEstymator = kMin(k, Hash::HashFNV, multiset);
			LD ratio = nEstymator / (LD)multiset.GetSize();

			file << (i) << "\t" << ratio << "\n";
		}

		file.close();
	}
	std::cout << "zajelo: " << (double)(clock() - start) / (double)CLOCKS_PER_SEC;
}

double ExperimentHandler::CalcChernoff(double x, unsigned int k)
{
	return std::exp(x*k) * std::pow((1.0 - x), k);
}

bool ExperimentHandler::CheckProbabilityRange(double rangePercent, double prob, unsigned int k/*, Multiset &multiset*/)
{
	double epsilon1 = (double)1.0 / (1.0 - (rangePercent / (double)100.0)) - 1.0;
	double epsilon2 = 1.0 - (double)1.0 / (1.0 + (rangePercent / 100.0));

	double calcProbability = 1.0 - CalcChernoff(epsilon2, k) - CalcChernoff(-epsilon1, k);

	return (calcProbability > prob);
	//LD nEstymator = kMin(k, hashFNV, multiset);
	//LD ratio = nEstymator / (LD)multiset.GetSize();
	//return (std::abs(ratio - 1.0) < rangePercent / (double)100.0);
}

unsigned int ExperimentHandler::RunExp2CalcKWithChernoff(unsigned int n)
{
	std::string fileName = "CalculatedBestK.txt";
	std::ofstream file(fileName.c_str());

	if (!file.good())
	{
		std::cout << "blad otwarcia pliku " << fileName << std::endl;
		return -1;
	}

	file << "best k:\n";
	//int n = 10000;
	double rangePercent = 10.0;
	double successProbability = 0.95;
	//Multiset multiset;
	//multiset.SetSize(n);

	unsigned int foundK = 0;
	for (unsigned int k = 1; k <= n; ++k)
	{
		if (CheckProbabilityRange(rangePercent, successProbability, k/*, multiset*/))
		{
			file << k << "\n";
			foundK = k;
			break;
		}
	}

	file.close();
	return foundK;
}

void ExperimentHandler::RunExp3CheckKValid(int k)
{
	int n = 10000;
	double probability = 0.1;

	Multiset multiset;

	int correct = 0;
	std::cout << "checking for k = " << k << std::endl;
	for (int i = 1; i <= n; ++i)
	{
		multiset.SetSize(i);

		LD nEstymator = kMin(k, Hash::HashFNV, multiset);
		LD ratio = nEstymator / (LD)multiset.GetSize();
		if (std::abs(ratio - 1.0) < probability)
			++correct;
	}

	std::cout << "k is valid " << (double)correct * 100 / (double)n << " % times\n";
}

bool ExperimentHandler::CheckIfCorrectEpsilon(double epsilon, std::vector<double> kMinEstymatorVec, int n, double probability, Multiset &multiset)
{
	int correct = 0;

	for (int i = 1; i <= n; ++i)
	{
		multiset.SetSize(i);

		LD ratio = kMinEstymatorVec[i - 1];
		if (std::abs(ratio - 1.0) < epsilon) // np. ratio < 0.1 (=epsilon) (10%)
			++correct;
	}

	return ((double)correct / (double)n >= probability);
}

double ExperimentHandler::CalcChernoffEpsilon(unsigned int k, double probability)
{
	double delta = 0.001;
	double decr = 0.001;
	double temp = 0.0;

	while (temp < probability)
	{
		double epsilon1 = (1.0 / (1.0 - delta)) - 1.0;
		double epsilon2 = 1.0 - (1.0 / (1.0 + delta));
		temp = 1.0 - CalcChernoff(epsilon2, k) - CalcChernoff(-epsilon1, k);
		delta += decr;
	}

	return (delta - decr);
}

double ExperimentHandler::CalcCzybyszewEps(unsigned int k, unsigned int n, double probability)
{
	double variance = (1.0 / (double)(k - 2)) + 1.0 / (double)n;

	return std::sqrt(variance / (1.0 - probability));
}

void ExperimentHandler::RunExp5CalcDelta()
{
	using namespace std;
	vector<double> probVec = { 0.95, 0.99, 0.995 };
	unsigned int k = 400;
	unsigned int n = 10000;
	Multiset multiset;
	std::string strFileName = "deltaEstymationForProb=";

	std::cout << "calculating kMin vector\n";
	std::vector<double> ratioVec;
	for (unsigned int i = 1; i <= n; ++i)
	{
		multiset.SetSize(i);
		LD nEstymator = kMin(k, Hash::HashFNV, multiset);
		LD ratio = nEstymator / (LD)multiset.GetSize();
		ratioVec.push_back(ratio);
	}

	for (double probability : probVec)
	{
		std::string strFileName = "deltaEstymationForProb=";
		strFileName += std::to_string(probability) + ".txt";
		std::ofstream file(strFileName);
		if (!file.good())
		{
			std::cout << "blad otwarcia pliku\n";
			return;
		}
		std::cout << "start experiment for probability: " << probability << std::endl;

		double chernoffEps = CalcChernoffEpsilon(k, probability);

		for (unsigned int i = 1; i <= n; ++i)
		{
			double czybyszewEps = CalcCzybyszewEps(k, i, probability);
			double normalEps = 0.0;
			//std::cout << "czybyszew: " << czybyszewEps << std::endl;

			double left = 0.0;
			double right = 1.0;
			double margin = 0.001;

			while (right - left > margin)
			{
				normalEps = left + (right - left) / 2.0;
				if (!CheckIfCorrectEpsilon(normalEps, ratioVec, i, probability, multiset))
					left = normalEps + margin;
				else
					right = normalEps;
			}

			file << i << "\t" << ratioVec[i - 1] << "\t" << 1.0 - normalEps << "\t" << 1.0 + normalEps << "\t"
				<< 1.0 - chernoffEps << "\t" << 1.0 + chernoffEps << "\t"
				<< 1.0 - czybyszewEps << "\t" << 1.0 + czybyszewEps << "\n";

		}

		file.close();
	}
}

void ExperimentHandler::RunHyperLogLogDifferentM()
{
	std::vector<unsigned int> bValues = { 4, 8, 12, 16 };
	int n = 10000;
	Multiset multiset;
	for (int b : bValues)
	{
		std::cout << "start experymentu dla b = " << b << std::endl;
		std::string fileName = "HashComparisonForHyperloglog//HyperHash=FNVb=" + std::to_string(b) + ".tsv";
		std::ofstream file(fileName.c_str());

		if (!file.good())
		{
			std::cout << "blad otwarcia pliku " << fileName << std::endl;
			return;
		}

		for (int i = 1; i <= n; ++i)
		{
			multiset.SetSize(i);

			LD nEstymator = Hyper::HyperLogLog(b, Hash::HashFNV, multiset);
			LD ratio = nEstymator / (LD)multiset.GetSize();

			file << (i) << "\t" << ratio << "\n";
		}

		file.close();
	}
}

void ExperimentHandler::RunCompareHyperloglogAndKMinForSameAmountOfMemory()
{

	std::vector<unsigned int> bValues = { 4, 8, 10, 12, 13, 14, 15, 16 };
	int n = 10000;
	Multiset multiset;
	for (int b : bValues)
	{
		int k = std::pow(2.0, b) * 5.0 / 32.0; //5bits for Hyper register and 32bits for KMin register
		std::cout << "start experymentu dla b = " << b << ", k = " << k << std::endl;
		std::string fileNameHyper = "KMinVsHyperloglog//CompHyperb=" + std::to_string(b) + "k=" + std::to_string(k) + ".tsv";
		std::ofstream fileHyper(fileNameHyper.c_str());

		if (!fileHyper.good())
		{
			std::cout << "blad otwarcia pliku " << std::endl;
			return;
		}	

		for (int i = 1; i <= n; ++i)
		{
			multiset.SetSize(i);

			LD nEstymator = Hyper::HyperLogLog(b, Hash::Hash_jenkins_one_at_time, multiset);
			LD ratio = nEstymator / (LD)multiset.GetSize();
			fileHyper << (i) << "\t" << ratio;

			nEstymator = kMin(k, Hash::Hash_jenkins_one_at_time, multiset);
			ratio = nEstymator / (LD)multiset.GetSize();
			fileHyper << "\t" << ratio << "\n";
		}

		fileHyper.close();
	}
}