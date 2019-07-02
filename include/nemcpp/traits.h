
#pragma once

#include <type_traits>
#include <utility>

namespace nem2_sdk {
	
	/// Returns enumeration type \a value converted to underlying type.
	template<typename TEnum>
	constexpr typename std::underlying_type<TEnum>::type to_underlying_type(TEnum value)
	{
		return static_cast<std::underlying_type_t<TEnum>>(value);
	}
	
	/// Determines whether \c T represents some container (type that has methods \c data and \c size).
	template<typename T>
	struct is_container {
		template<
			typename U,
			typename = typename std::enable_if<std::is_pointer_v<decltype(std::declval<U*>()->data())>>::type,
			typename = decltype(std::declval<U*>()->size())>
		static std::true_type test(int);
		
		template<typename U> static std::false_type test(...);
		
		static constexpr bool value = decltype(test<typename std::decay<T>::type>(0))::value;
	};
	
	template<typename T>
	constexpr bool is_container_v = is_container<T>::value;
	
	/// Determines whether \c T is iterable (has methods \c begin and \c end).
	template<typename T>
	struct is_iterable {
		template<
			typename U,
			typename = decltype(std::declval<U*>()->begin()),
			typename = decltype(std::declval<U*>()->end())>
		static std::true_type test(int);
		
		template<typename U> static std::false_type test(...);
		
		static constexpr bool value = decltype(test<typename std::decay<T>::type>(0))::value;
	};
	
	template<typename T>
	constexpr bool is_iterable_v = is_iterable<T>::value;
}
