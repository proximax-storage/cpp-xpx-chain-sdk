
#pragma once

#include <cstdio>
#include <sstream>
#include <string>

namespace nem2_sdk {
	
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
}
