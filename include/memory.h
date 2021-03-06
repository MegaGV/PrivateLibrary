// memory.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_MEMORY_H_
#define _PRILIB_MEMORY_H_
#include "macro.h"
#include <utility>
#include <cstdint>
#include <cstdlib>
#include <cstring>

PRILIB_BEGIN
#if _ITERATOR_DEBUG_LEVEL != 0
PRILIB_END
#include <iterator>
PRILIB_BEGIN
template <typename T>
class pointer_iterator : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>
{
public:
	using _Unchecked_type = std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>;

	explicit pointer_iterator(T* p) : p(p) {}
	bool operator!=(const pointer_iterator& i) const {
		return p != i.p;
	}
	bool operator<(const pointer_iterator& i) const {
		return p < i.p;
	}
	T& operator*() const {
		return *p;
	}
	const pointer_iterator& operator++() {
		++p;
		return *this;
	}
	ptrdiff_t operator+(const pointer_iterator &i) const {
		return p + i.p;
	}
	ptrdiff_t operator-(const pointer_iterator &i) const {
		return p - i.p;
	}
	pointer_iterator operator+(ptrdiff_t i) const {
		return pointer_iterator(p + i);
	}
	pointer_iterator operator-(ptrdiff_t i) const {
		return pointer_iterator(p - i);
	}

	T* p;
};

template <typename T>
inline pointer_iterator<T> _PointerIterator(T* p) {
	return pointer_iterator<T>(p);
}
#else
template <typename T>
inline T* _PointerIterator(T* p) {
	return p;
}
#endif

using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;
using qword = uint64_t;

namespace Memory
{
	template <typename T>
	inline constexpr size_t get_size(size_t length) {
		return length * sizeof(T);
	}

	template <>
	inline constexpr size_t get_size<void>(size_t length) {
		return length;
	}

	// Alloc/Free
	template <typename T = byte>
	inline T* alloc(size_t length) {
		return (T*)std::malloc(length * sizeof(T));
	}
	template <typename T = byte>
	inline T* calloc(size_t length) {
		return (T*)std::calloc(length * sizeof(T), 1);
	}
	template <typename T = void>
	inline void free(T *p) {
		std::free(p);
	}
	// New/Delete
	template <typename T>
	inline T* new_(size_t length) {
		return new T[length]();
	}
	template <typename T>
	inline void new_n(T *data, size_t length) {
		for (size_t i = 0; i < length; i++) {
			new (data + i) T();
		}
	}
	template <typename T>
	inline void delete_(T *p) {
		delete[] p;
	}
	template <typename T>
	inline void delete_n(T *p, size_t n) {
		for (size_t i = 0; i < n; i++) {
			p[i]. ~T();
		}
		free(p);
	}

	template <typename T>
	inline void copy_n(T *data, T *begin, size_t length) {
		for (size_t i = 0; i < length; i++) {
			new (data + i) T(*(begin + i));
		}
	}
	template <typename T, typename Iter>
		inline void copy_n(T *data, Iter begin, size_t length) {
		for (size_t i = 0; i < length; i++) {
			new (data + i) T(*(begin + i));
		}
	}

	// Construct/Destruct
	template <typename T>
	inline void construct(T *data) {
		new (data) T();
	}
	template <typename T>
	inline void destruct(T *data) {
		data->~T();
	}
	// MCopy/NCopy
	inline void* mcopy(void* to, const void* from, size_t length) {
		return std::memcpy(to, from, length);
	}
	template <typename T>
	inline T& ncopy(T& to, const T& from) {
		return to = from;
	}

	template <typename T>
	inline T* clear(T* dat, size_t length) {
		std::memset((void*)dat, 0, get_size<T>(length));
		return dat;
	}

	template <typename T>
	inline T* copyTo(T* to, const T* from, size_t length) {
		return (T*)std::memcpy((void*)to, (void*)from, get_size<T>(length));
	}

	template <typename T>
	inline T* copyOf(const T* from, size_t length) {
		if (from == nullptr)
			return nullptr;
		else
			return copyTo(new_<T>(length), from, length);
	}
}
PRILIB_END

#endif
