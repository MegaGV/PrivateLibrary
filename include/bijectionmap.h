// bijectionmap.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_BIJECTIONMAP_H_
#define _PRILIB_BIJECTIONMAP_H_
#include "macro.h"
#include <map>
#include <vector>
#include <deque>
#include <initializer_list>

PRILIB_BEGIN
template <typename _KTy, typename _VTy>
class BijectionMap
{
	using _PTy = std::pair<_KTy, _VTy>;
	using _CPTy = std::pair<const _KTy, const _VTy>;
public:
	using key_type = _KTy;
	using mapped_type = _VTy;
	using value_type = _CPTy;
private:
	template <typename _MTy>
	class iterator_base {
	public:
		iterator_base(_MTy *data, size_t count = 0)
			: data(data), count(count) {}

		using value_type = typename std::conditional<std::is_const<_MTy>::value, const _PTy, _PTy>::type;

		auto& operator*() {
			return get();
		}
		auto* operator->() {
			return &get();
		}
		auto& operator*() const {
			return get();
		}
		auto* operator->() const {
			return &get();
		}
		auto& operator[](std::ptrdiff_t diff) {
			return get(diff);
		}
		const auto& operator[](std::ptrdiff_t diff) const {
			return get(diff);
		}
		iterator_base& operator++() {
			++count;
			return *this;
		}
		iterator_base& operator--() {
			--count;
			return *this;
		}
		iterator_base operator+(std::ptrdiff_t diff) {
			return iterator_base(data, count + diff);
		}
		iterator_base operator-(std::ptrdiff_t diff) {
			return iterator_base(data, count - diff);
		}
		bool operator==(const iterator_base &iter) const {
			return data == iter.data && count == iter.count;
		}
		bool operator!=(const iterator_base &iter) const {
			return data != iter.data || count != iter.count;
		}
		bool operator>(const iterator_base &iter) const {
			return count > iter.count;
		}
		bool operator>=(const iterator_base &iter) const {
			return count >= iter.count;
		}
		bool operator<(const iterator_base &iter) const {
			return count < iter.count;
		}
		bool operator<=(const iterator_base &iter) const {
			return count <= iter.count;
		}

	private:
		_MTy *data;
		size_t count;
		auto& get(std::ptrdiff_t diff = 0) {
			return data->getData(count + diff);
		}
		const auto& get(std::ptrdiff_t diff = 0) const {
			return data->getData(count + diff);
		}
		friend class BijectionMap;
	};
public:
	using iterator = iterator_base<BijectionMap>;
	using const_iterator = iterator_base<const BijectionMap>;

public:
	BijectionMap() {}
	BijectionMap(const std::initializer_list<value_type> &il) {
		for (const value_type &e : il)
			insert(e.first, e.second);
	}

	iterator insert(const _KTy &key, const _VTy &val) {
		if (findKey(key) != size() || findValue(val) != size()) {
			return end();
		}

		iterator result(this);
		if (destroyable.empty()) {
			keymap[key] = currcount;
			valmap[val] = currcount;
			data.push_back(std::make_pair(key, val));
			result = iterator(this, currcount);
			currcount++;
		}
		else {
			size_t id = destroyable.front();
			keymap[key] = id;
			valmap[val] = id;
			data[id] = std::make_pair(key, val);
			result = iterator(this, id);
			destroyable.pop_front();
		}
		return result;
	}
	iterator insert_update(const _KTy &key, const _VTy &val) {
		size_t id;
		if ((id = findKey(key)) != size()) {
			getData(id).second = val;
			return begin() + id;
		}
		else if ((id = findValue(val)) != size()) {
			getData(id).first = key;
			return begin() + id;
		}
		else {
			return insert(key, val);
		}
	}
	size_t currentIndex() const {
		return destroyable.empty() ? currcount : destroyable.front();
	}
	bool erase(const _KTy &key) {
		size_t id = findKey(key);
		if (id == size())
			return false;

		const _VTy &val = getValue(id);
		_erase(key, val, id);
		return true;
	}
	bool eraseID(size_t id) {
		const _KTy &key = getKey(id);
		const _VTy &val = getValue(id);
		_erase(key, val, id);
		return true;
	}
	bool erase(const iterator &iter) {
		assert(iter.data == this);
		size_t id = iter.count;
		const _KTy &key = iter->first;
		const _VTy &val = iter->second;
		_erase(key, val, id);
		return true;
	}
	size_t findKey(const _KTy &key) const {
		return _find(keymap, key);
	}
	size_t findValue(const _VTy &val) const {
		return _find(valmap, val);
	}
	_PTy& getData(size_t id) {
		return data[id];
	}
	const _PTy& getData(size_t id) const {
		return data[id];
	}
	const _KTy& getKey(size_t id) const {
		return  data[id].first;
	}
	const _VTy& getValue(size_t id) const {
		return  data[id].second;
	}

	size_t size() const {
		return data.size();
	}
	bool empty() const {
		return data.empty();
	}
	// Iterator
	iterator begin() {
		return iterator(this, 0);
	}
	iterator end() {
		return iterator(this, size());
	}
	const_iterator begin() const {
		return cbegin();
	}
	const_iterator end() const {
		return cend();
	}
	const_iterator cbegin() {
		return const_cast<const BijectionMap *>(this)->cbegin();
	}
	const_iterator cend() {
		return const_cast<const BijectionMap *>(this)->cend();
	}
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	const_iterator cend() const {
		return const_iterator(this, size());
	}

private:
	size_t currcount = 0;
	std::map<_KTy, size_t> keymap;
	std::map<_VTy, size_t> valmap;
	std::vector<_PTy> data;
	std::deque<size_t> destroyable;

	template <typename T>
	size_t _find(const std::map<T, size_t> &map, const T &v) const {
		auto iter = map.find(v);
		if (iter != map.end())
			return iter->second;
		else
			return this->size();
	}
	void _erase(const _KTy &key, const _VTy &val, size_t id) {
		keymap.erase(key);
		valmap.erase(val);
		destroyable.push_back(id);
	}
};

template <typename _KTy, typename _VTy>
class BijectionKVMap
{
	using _PTy = std::pair<_KTy, _VTy>;
	using _CPTy = std::pair<const _KTy, const _VTy>;
public:
	using key_type = _KTy;
	using mapped_type = _VTy;
	using value_type = _CPTy;
public:
	BijectionKVMap() {}
	BijectionKVMap(const std::initializer_list<value_type> &il) {
		for (const value_type &e : il)
			data.insert(e.first, e.second);
	}
	bool insert(const _KTy &key, const _VTy &val) {
		return data.insert(key, val) != data.end();
	}
	bool eraseKey(const _KTy &key) {
		return data.erase(key);
	}
	bool eraseValue(const _VTy &val) {
		return data.eraseID(data.findValue(val));
	}
	size_t findKey(const _KTy &key) const {
		return data.findKey(key);
	}
	size_t findValue(const _VTy &val) const {
		return data.findValue(val);
	}
	const _PTy& getData(size_t id) const {
		return data.getData(id);
	}
	const _KTy& getKey(const _VTy &val) const {
		return  data.getKey(data.findValue(val));
	}
	const _VTy& getValue(const _KTy &key) const {
		return  data.getValue(data.findKey(key));
	}
	size_t size() const {
		return data.size();
	}
	bool empty() const {
		return data.empty();
	}

private:
	BijectionMap<_KTy, _VTy> data;
};

template <typename _KTy>
class SerialBijectionMap
{
	using _VTy = size_t;
	using _PTy = std::pair<_KTy, _VTy>;
	using _CPTy = std::pair<const _KTy, const _VTy>;
public:
	using key_type = _KTy;
	using mapped_type = _VTy;
	using value_type = _CPTy;
public:
	explicit SerialBijectionMap() {}
	explicit SerialBijectionMap(const std::initializer_list<_KTy> &keylist) {
		for (auto &key : keylist) {
			insert(key);
		}
	}

	bool insert(const _KTy &key) {
		if (findKey(key) != size()) {
			return false;
		}
		else {
			_insert(key);
			return true;
		}
	}
	size_t insertRepeat(const _KTy &key) {
		size_t index = findKey(key);
		if (index == size()) {
			return _insert(key);
		}
		else {
			return index;
		}
	}
	size_t currentIndex() const {
		return destroyable.empty() ? currcount : destroyable.front();
	}
	bool erase(const _KTy &key) {
		size_t id = findKey(key);
		if (id == size())
			return false;
		_erase(key, id);
		return true;
	}
	bool eraseID(size_t id) {
		const _KTy &key = getKey(id);
		_erase(key, id);
		return true;
	}
	size_t findKey(const _KTy &key) const {
		return _find(keymap, key);
	}
	const _KTy& getKey(size_t id) const {
		return data[id];
	}
	size_t operator[](const _KTy &key) {
		size_t id = findKey(key);
		if (id == size()) {
			id = currentIndex();
			insert(key);
		}
		return id;
	}

	size_t size() const {
		return data.size();
	}
	bool empty() const {
		return data.empty();
	}

private:
	size_t currcount = 0;
	std::map<_KTy, size_t> keymap;
	std::vector<_KTy> data;
	std::deque<size_t> destroyable;

	template <typename T>
	size_t _find(const std::map<T, size_t> &map, const T &v) const {
		auto iter = map.find(v);
		if (iter != map.end())
			return iter->second;
		else
			return this->size();
	}
	void _erase(const _KTy &key, size_t id) {
		keymap.erase(key);
		destroyable.push_back(id);
	}
	size_t _insert(const _KTy &key) {
		if (destroyable.empty()) {
			keymap[key] = currcount;
			data.push_back(key);
			currcount++;
			return currcount - 1;
		}
		else {
			size_t id = destroyable.front();
			keymap[key] = id;
			data[id] = key;
			destroyable.pop_front();
			return id;
		}
	}
};
PRILIB_END

#endif
