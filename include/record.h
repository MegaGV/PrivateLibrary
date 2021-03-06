// record.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_RECORD_H_
#define _PRILIB_RECORD_H_
#include "macro.h"
#include <utility>

PRILIB_BEGIN
template <typename T>
class Record
{
public:
	explicit Record(T &ref)
		: _record(std::move(ref)), _data(ref) {}

	explicit Record(T &ref, T &&set)
		: Record(ref) {
		_data = std::move(set);
	}

	~Record() {
		_data = std::move(_record);
	}

	T _record;
	T &_data;
};
PRILIB_END

#endif
