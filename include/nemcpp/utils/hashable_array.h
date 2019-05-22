
#pragma once

#include <array>
#include <cstring>
#include <tuple>
#include <type_traits>
#include <typeindex>

namespace nem2_sdk {
	
	/// Hashable array for which \c std::hash is specialized.
	/// \note Can be used as a key in STL hash-based containers.
	template<typename T, size_t N>
	class HashableArray: public std::array<T, N> { };
}

namespace std {
	
	template<typename T, size_t N>
	struct hash<nem2_sdk::HashableArray<T, N>> {
		size_t operator()(const nem2_sdk::HashableArray<T, N>& a) const
		{
			size_t hash = 0;
			
			// Using offset of 4 because some sdk hashable arrays (like address, for example) do not
			// have enough entropy at the beginning.
			constexpr size_t Offset = 4;
			static_assert(N >= sizeof(hash) + Offset, "invalid hashable array size");
			
			std::memcpy(&hash, a.data() + Offset, sizeof(hash));
			return hash;
		}
	};
	
	template<typename T, size_t N>
	class tuple_size<nem2_sdk::HashableArray<T, N>>: public integral_constant<size_t, N> { };
	
	template<size_t I, typename T, size_t N>
	struct tuple_element<I, nem2_sdk::HashableArray<T, N>> {
		using type = T;
	};
}
