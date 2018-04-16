#include "stdafx.h"
#include "Hash.h"


Hash::Hash()
{
}

uint32_t Hash::HashFNV(const void *data, size_t size)
{
	const uint32_t fnv_prime = 0x811C9DC5;
	uint32_t hash = 0;
	size_t i;
	const uint8_t *key = (const uint8_t *)data;

	if (data == NULL || size == 0)
		return 0;

	for (i = 0; i < size; ++i)
	{
		hash *= fnv_prime;
		hash ^= key[i];
	}

	return hash;
}

uint32_t Hash::Hash_super_fast_hash(const void *data, size_t size)
{
	const uint32_t mask1 = 0x0000FFFF;
	const uint32_t mask2 = 0xFFFF0000;
	uint32_t hash = (uint32_t)size;
	uint32_t temp;

	size_t i = 0;
	const size_t rem_by_4 = size & 0x3;
	const unsigned char *key = (const unsigned char *)data;
	uint32_t acc;


	if (data == NULL || size == 0)
		return 0;

	/* 4 bytes at 1 time */
	size >>= 2;
	for (i = 0; i < size; i += 4)
	{
		acc = *(uint32_t *)&key[i];
		hash += acc & mask1;
		temp = ((acc & mask2) << 11) ^ hash;
		hash = (hash << 16) ^ temp;
		hash += hash >> 11;
	}

	key = &key[i];

	/* the last part */
	switch (rem_by_4)
	{
	case 3:
	{
		hash += *(uint16_t *)key;
		hash ^= hash << 16;
		hash ^= (uint32_t)key[2] << 18;
		hash += hash >> 11;
		break;
	}
	case 2:
	{
		hash += *(uint16_t *)key;
		hash ^= hash << 11;
		hash += hash >> 17;
		break;
	}
	case 1:
	{
		hash += key[0];
		hash ^= hash << 10;
		hash += hash >> 1;
		break;
	}
	case 0:
	{
		break;
	}
	default:
	{
		break;
	}
	}

	/* Final step */
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}

uint32_t Hash::Hash_jenkins_one_at_time(const void *data, size_t size)
{
	size_t i;
	uint32_t hash = 0;
	const uint8_t *key = (const uint8_t *)data;

	if (data == NULL || size == 0)
		return 0;

	for (i = 0; i < size; ++i)
	{
		hash += key[i];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}
