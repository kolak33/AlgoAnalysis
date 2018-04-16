// przyblizoneZliczanieAA2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "ExperimentHandler.h"
#include "Hash.h"

#include "HyperLogLog.h"
int main()
{
	ExperimentHandler exp;

	//exp.RunExp1CalcRatio(); //1b
	//exp.RunExp2CalcKWithChernoff(); //1c

	//exp.RunExp3CheckKValid(752); //1c verify
	//double probability = 0.95;
	//unsigned int k = 400;
	//std::cout << exp.CalcChernoffEpsilon(k, probability) << std::endl;
	//exp.RunExp4ShadyResults(Hash::HashFNV); //2a

	//exp.RunExp4ShadyResults(Hash::Hash_super_fast_hash); //2b
	//exp.RunExp4ShadyResults(Hash::Hash_jenkins_one_at_time); //2b

	//exp.RunExp5CalcDelta(); //3


	//exp.RunHyperLogLogDifferentM();
	exp.RunCompareHyperloglogAndKMinForSameAmountOfMemory();

    return 0;
}

