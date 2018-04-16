#pragma once

#include <iostream>

typedef long LL;
typedef long double LD;

class Multiset
{
public:
	Multiset(LL n);
	Multiset();
	
	LL GetSize();
	void SetSize(LL n);
	LL GetNextValue();

private:
	LL m_lSize;
	LL m_lCurrentValue;
	LL m_lRemainingValues;
};