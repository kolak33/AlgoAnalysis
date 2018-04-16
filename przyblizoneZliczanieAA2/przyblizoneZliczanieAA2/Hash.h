#pragma once

#include <iostream>

class Hash
{
public:
	Hash();

	static uint32_t HashFNV(const void *data, size_t size);
	static uint32_t Hash_super_fast_hash(const void *data, size_t size);
	static uint32_t Hash_jenkins_one_at_time(const void *data, size_t size);
};

