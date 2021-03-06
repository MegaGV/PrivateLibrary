// range.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_RANGE_H_
#define _PRILIB_RANGE_H_
#include "macro.h"
#include <string>
#include "prints.h"

PRILIB_BEGIN
template <typename T, typename Iter>
class RangeBase
{
public:
	using type = T;
	using iterator = Iter;
public:
	RangeBase(const T &bg, const T &ed) : _begin(bg), _end(ed) {}
	// Data Method
	Iter begin() const {
		return _begin;
	}
	Iter end() const {
		return _end;
	}
	bool operator==(const RangeBase &r) const {
		return _begin == r._begin && _end == r._end;
	}

	// Calculation
	size_t size() const {
		return _end - _begin;
	}

	bool include(const T &n) const {
		return n >= _begin && n < _end;
	}
	bool include(const RangeBase &r) const {
		return _begin <= r._begin && _end >= r._end;
	}

	template <typename R, typename F> friend std::string to_string(const RangeBase<R, F> &r);

protected:
	T _begin, _end;
};

template <typename T, typename Iter>
class RangeBaseReverse
{
public:
	using type = T;
	using iterator = Iter;
public:
	RangeBaseReverse(const T &bg, const T &ed) : _begin(bg), _end(ed) {}
	// Data Method
	Iter begin() const {
		return _begin;
	}
	Iter end() const {
		return _end;
	}
	bool operator==(const RangeBaseReverse &r) const {
		return _begin == r._begin && _end == r._end;
	}

	// Calculation
	size_t size() const {
		return _begin - _end;
	}

	bool include(const T &n) const {
		return n <= _begin && n > _end;
	}
	bool include(const RangeBaseReverse &r) const {
		return _begin >= r._begin && _end <= r._end;
	}

	template <typename R, typename F> friend std::string to_string(const RangeBase<R, F> &r);

protected:
	T _begin, _end;
};

template <typename T>
class RIterator
{
public:
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T * ;
	using reference = T & ;
	using iterator_category = std::random_access_iterator_tag;

public:
	RIterator(T value) : value(value) {}
	operator T() const {
		return value;
	}
	T operator*() const {
		return value;
	}
	bool operator!=(const RIterator& i) const {
		return value != i.value;
	}
	const RIterator& operator++() {
		++value;
		return *this;
	}

private:
	T value;
};

template <typename T>
class RIteratorReverse
{
public:
	RIteratorReverse(T value) : value(value) {}
	operator T() const {
		return value;
	}
	T operator*() const {
		return value;
	}
	bool operator!=(const RIteratorReverse& i) const {
		RIteratorReverse rir(i);
		++rir;
		return value != rir.value;
	}
	const RIteratorReverse& operator++() {
		--value;
		return *this;
	}

private:
	T value;
};

template <typename T>
class Range : public RangeBase<T, RIterator<T>>
{
public:
	Range(const T &bg, const T &ed) : RangeBase<T, RIterator<T>>(bg, ed) {}

	T operator[](size_t i) const {
		return this->_begin + i;
	}
};

template <typename T>
class RangeReverse : public RangeBaseReverse<T, RIteratorReverse<T>>
{
public:
	RangeReverse(const T &bg, const T &ed) : RangeBaseReverse<T, RIteratorReverse<T>>(bg, ed) {}

	T operator[](size_t i) const {
		return this->_end + i;
	}
};

template <typename T>
class RangeIterator : public RangeBase<T, T>
{
public:
	RangeIterator(const T &bg, const T &ed) : RangeBase<T, T>(bg, ed) {
		if (bg >= ed)
			this->_begin = this->_end;
	}

	auto operator[](size_t i) const {
		return *(this->_begin + i);
	}
};

template <typename T>
inline Range<T> range(const T &a, const T &b)
{
	return Range<T>(a, b);
}
inline Range<size_t> range(int a, size_t b)
{
	return Range<size_t>(a, b);
}
inline Range<size_t> range(size_t a, int b)
{
	return Range<size_t>(a, b);
}
inline Range<size_t> range(size_t e)
{
	return Range<size_t>(0, e);
}

template <typename T>
inline RangeReverse<T> ranger(const T &a, const T &b)
{
	return RangeReverse<T>(a, b);
}
inline RangeReverse<size_t> ranger(int a, size_t b)
{
	return RangeReverse<size_t>(a, b);
}
inline RangeReverse<size_t> ranger(size_t a, int b)
{
	return RangeReverse<size_t>(a, b);
}

template <typename T>
inline RangeIterator<T> rangei(T a, T b)
{
	return RangeIterator<T>(a, b);
}

template <typename T1, typename T2>
inline std::string to_string(const RangeBase<T1, T2> &r)
{
	using Convert::to_string;
	using std::to_string;

	return "range(" + to_string(r._begin) + "," + to_string(r._end) + ")";
}
PRILIB_END

#endif
