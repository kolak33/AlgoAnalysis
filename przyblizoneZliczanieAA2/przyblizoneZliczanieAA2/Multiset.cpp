#include "stdafx.h"
#include "Multiset.h"


Multiset::Multiset()
{
	m_lSize = m_lCurrentValue = m_lRemainingValues = 1;
}

Multiset::Multiset(LL n)
	: m_lSize(n),
m_lCurrentValue(1),
m_lRemainingValues(n)
{
}

LL Multiset::GetSize()
{
	return m_lSize;
}

void Multiset::SetSize(LL n)
{
	m_lSize = m_lRemainingValues = n;
}

LL Multiset::GetNextValue()
{
	--m_lRemainingValues;
	if (m_lCurrentValue == UINT32_MAX)
		m_lCurrentValue = 1;

	return m_lCurrentValue++;
}