/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/traits.h>

#include <cstdio>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace xpx_chain_sdk {
	
	/// Type-safe formatted string builder.
	class MakeString {
	public:
		/// Appends \a arg to the end of string.
		template<typename T>
		MakeString& operator<<(const T& arg)
		{
			stream_ << arg;
			return *this;
		}

		/// Returns builded string
		std::string str() const;

		/// Returns builded string.
		operator std::string() const;
		
	private:
		std::ostringstream stream_;
	};
	
	/// Returns formatted string replacing placeholders in \a format with \a args.
	template<typename... TArgs>
	std::string StringFormat(const char* format, TArgs... args)
	{
		constexpr size_t bufSize = 100;
		char buffer[bufSize];
		char* ptr = buffer;
		
		auto size = std::snprintf(nullptr, 0, format, args...);
		
		if (size < 0) {
			return std::string(format);
		}
		
		if (size + 1 > bufSize) {
			ptr = new char[size];
		}
		
		std::snprintf(ptr, size + 1, format, args...);
		
		if (ptr != buffer) {
			delete[] ptr;
		}
		
		return std::string(ptr, size);
	}
	
	/// Returns string containing hex-formatted \a value of integral type.
	template<typename T>
	auto ToHex(const T& value) -> std::enable_if_t<std::is_integral_v<T>, std::string>
	{
		const char* digits = "0123456789ABCDEF";
		size_t resultSize = sizeof(T) * 2;
		std::string result(resultSize, '0');
		
		for (size_t i = 0, j = (resultSize - 1) * 4; i < resultSize; ++i, j -= 4) {
			result[i] = digits[(value >> j) & 0x0F];
		}
		
		return result;
	}
	
	/// Returns string containing hex-formatted \a value of enum type.
	template<typename T>
	auto ToHex(const T& value) -> std::enable_if_t<std::is_enum_v<T>, std::string>
	{
		return ToHex(to_underlying_type(value));
	}
	
	/// Returns string containing hex-formatted \a value of iterable type (those with methods \c begin and \c end).
	template<typename T>
	auto ToHex(const T& value) -> std::enable_if_t<xpx_chain_sdk::is_iterable_v<T>, std::string>
	{
		std::string result;
		
		for (const auto& item: value) {
			result += ToHex(item);
		}
		
		return result;
	}
	
	/// Converts hex-formatted string to \a value of integral type and returns \c true on success.
	template<typename T>
	auto FromHex(std::string_view hexStr, T& value) -> std::enable_if_t<std::is_integral_v<T>, bool>
	{
		value = 0;
		
		if (hexStr.size() != sizeof(T) * 2) {
			return false;
		}
		
		for (auto ch: hexStr) {
			value <<= 4;
			
			if (ch >= '0' && ch <= '9') {
				value |= ch - '0';
			} else if (ch >= 'a' && ch <= 'f') {
				value |= ch - 'a' + 10;
			} else if (ch >= 'A' && ch <= 'F') {
				value |= ch - 'A' + 10;
			}
			else {
				return false;
			}
		}
		
		return true;
	}
	
	/// Converts hex-formatted string to \a value of enum type and returns \c true on success.
	template<typename T>
	auto FromHex(std::string_view hexStr, T& value) -> std::enable_if_t<std::is_enum_v<T>, bool>
	{
		std::underlying_type_t<T> rawValue{};
		bool result = FromHex(hexStr, rawValue);
		value = static_cast<T>(rawValue);
		return result;
	}
	
	/// Converts hex-formatted string to binary vector \a data and returns \c true on success.
	template<typename T>
	bool FromHex(std::string_view hexStr, std::vector<T>& data)
	{
			data.clear();
		size_t itemLen = sizeof(T) * 2;
		
		if (hexStr.empty()) {
			return true;
		} else if (hexStr.size() % itemLen) {
			return false;
		} else {
			data.resize(hexStr.size() / itemLen);
		}
		
		for (size_t i = 0; i < data.size(); ++i) {
			if (!FromHex(hexStr.substr(i * itemLen, itemLen), data[i])) {
				data.clear();
				return false;
			}
		}
		
		return true;
	}
}
