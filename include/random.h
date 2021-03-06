// random.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_RANDOM_H_
#define _PRILIB_RANDOM_H_
#include "macro.h"
#include <cstdlib>

PRILIB_BEGIN
inline int random_base(int n, int (*rand)(void))
{
	int rand_result;
	int rand_max = RAND_MAX / n * n;
	do {
		rand_result = rand();
	} while (rand_result >= rand_max);
	return rand_result % n;
}
inline int random(int n)
{
	return random_base(n, std::rand);
}
inline int random(int begin, int end)
{
	return random(end - begin) + begin;
}
PRILIB_END

#endif
