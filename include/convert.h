// convert.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_CONVERT_H_
#define _PRILIB_CONVERT_H_
#include "macro.h"
#include <string>
#include <functional>
#include <limits>

PRILIB_BEGIN
namespace Convert
{
	// Format
	template <typename T> struct Format;
	template <> struct Format<int> { static constexpr const char *text = "%d"; };
	template <> struct Format<long> { static constexpr const char *text = "%ld"; };
	template <> struct Format<long long> { static constexpr const char *text = "%lld"; };
	template <> struct Format<unsigned> { static constexpr const char *text = "%u"; };
	template <> struct Format<unsigned long> { static constexpr const char *text = "%lu"; };
	template <> struct Format<unsigned long long> { static constexpr const char *text = "%llu"; };
	template <> struct Format<float> { static constexpr const char *text = "%f"; };
	template <> struct Format<double> { static constexpr const char *text = "%f"; };
	template <> struct Format<long double> { static constexpr const char *text = "%Lf"; };
	template <> struct Format<char> { static constexpr const char *text = "%c"; };

	template <typename T> inline constexpr const char* format() { return Format<T>::text; }

	// To String
	//   String & const char*
	inline const std::string& to_string(const std::string &str)
	{
		return str;
	}
	inline std::string to_string(const char *s)
	{
		return std::string(s);
	}
	//   Value to Template
	inline std::string to_string(std::nullptr_t null)
	{
		return std::string("Null");
	}
	inline std::string to_string(bool b)
	{
		return std::string((b == true) ? "T" : "F");
	}
	inline std::string to_string(char c)
	{
		std::string str;
		str.push_back(c);
		return str;
	}
	inline std::string to_string(const void *p)
	{
		size_t size = sizeof(void*) * 2 + 3;
		char *str = new char[size];
		snprintf(str, size, "0x%p", p);
		return str;
	}
	template <char B = '\0', char E = '\0', char D = '\0', typename Iter, typename Func>
	std::string to_string(Iter begin, Iter end, Func func) {
		std::string str;
		if (B != '\0')
			str.push_back(B);
		if (begin != end) {
			str.append(func(*begin++));
			while (begin != end) {
				if (D != '\0')
					str.push_back(D);
				if (D != '\n')
					str.append(" ");
				str.append(func(*begin++));
			}
		}
		if (E != '\0')
			str.push_back(E);
		return str;
	}
	template <char B = '\0', char E = '\0', char D = '\0', typename Iter>
	inline std::string to_string(Iter begin, Iter end) {
		using std::to_string;
		using Convert::to_string;

		return to_string<B, E, D>(begin, end, [](const auto &e) { return to_string(e); });
	}
}

namespace Convert
{
	// To Hex
	template <typename T>
	std::string to_hex(T value, const char *fmt)
	{
		const size_t len = sizeof(T) * 2 + 1;
		char data[len];
#if defined(_MSC_VER)
		sprintf_s(data, len, fmt, value);
#else
		sprintf(data, fmt, value);
#endif
		return to_string(data);
	}

	inline std::string to_hex(int8_t i) { return to_hex(i, "%02x"); }
	inline std::string to_hex(uint8_t i) { return to_hex(i, "%02x"); }
	inline std::string to_hex(int16_t i) { return to_hex(i, "%04x"); }
	inline std::string to_hex(uint16_t i) { return to_hex(i, "%04x"); }
	inline std::string to_hex(int32_t i) { return to_hex(i, "%08x"); }
	inline std::string to_hex(uint32_t i) { return to_hex(i, "%08x"); }
	inline std::string to_hex(int64_t i) { return to_hex(i, "%016llx"); }
	inline std::string to_hex(uint64_t i) { return to_hex(i, "%016llx"); }
}

namespace Convert
{
	bool to_ll(const std::string &str, long long &result, int base = 10);
	bool to_ull(const std::string &str, unsigned long long &result, int base = 10);

	template <typename T>
	bool to_integer(const std::string &str, T &result, int base = 10) {
		T min = std::numeric_limits<T>::min();
		T max = std::numeric_limits<T>::max();
		if (std::numeric_limits<T>::is_signed) {
			long long res;
			if (!to_ll(str, res, base))
				return false;
			if (res > static_cast<long long>(max) || res < static_cast<long long>(min))
				return false;
			result = static_cast<T>(res);
		}
		else {
			unsigned long long res;
			if (!to_ull(str, res, base))
				return false;
			if (res > static_cast<unsigned long long>(max))
				return false;
			result = static_cast<T>(res);
		}
		return true;
	}

	template <typename T>
	T to_integer(const std::string &str, std::function<void()> errfunc, int base = 10) {
		T result;
		if (to_integer(str, result, base)) {
			return result;
		}
		else {
			errfunc();
			return 0;
		}
	}
}

namespace Convert
{
	void split(const std::string &token, const std::string &delimit, std::function<void(const char*)> yield);
}
PRILIB_END

#endif
