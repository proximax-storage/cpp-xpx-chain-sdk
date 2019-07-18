
#pragma once

#include <nemcpp/utils/hashable_array.h>

#include <type_traits>
#include <utility>

namespace nem2_sdk {
	
	/// Returns enumeration type \a value converted to underlying type.
	template<typename TEnum>
	constexpr typename std::underlying_type<TEnum>::type to_underlying_type(TEnum value)
	{
		return static_cast<std::underlying_type_t<TEnum>>(value);
	}
	
	/// Determines whether \c T is specialization of template type \c U.
	template<typename T, template<typename...> typename U>
	struct is_specialization: std::false_type { };

	template<template<typename...> typename U, typename... TArgs>
	struct is_specialization<U<TArgs...>, U>: std::true_type { };

	template<typename T, template<typename...> typename U>
	constexpr bool is_specialization_v = is_specialization<T, U>::value;

	/// Determines whether \c T represents some container (type that has methods \c data and \c size).
	template<typename T>
	struct is_container {
		template<
			typename U,
			typename = typename std::enable_if<std::is_pointer_v<decltype(std::declval<U&>().data())>>::type,
			typename = decltype(std::declval<U&>().size())>
		static std::true_type test(int);
		
		template<typename U> static std::false_type test(...);
		
		static constexpr bool value = decltype(test<std::decay_t<T>>(0))::value;
	};
	
	template<typename T>
	constexpr bool is_container_v = is_container<T>::value;
	
	/// Determines whether \c T is iterable (has methods \c begin and \c end).
	template<typename T>
	struct is_iterable {
		template<
			typename U,
			typename = decltype(std::begin(std::declval<U&>())),
			typename = decltype(std::end(std::declval<U&>()))>
			static std::true_type test(int);

		template<typename U> static std::false_type test(...);

		static constexpr bool value = decltype(test<std::remove_cv_t<std::remove_reference_t<T>>>(0))::value;
	};

	template<typename T>
	constexpr bool is_iterable_v = is_iterable<T>::value;

	/// Determines whether \c T is an \c std::array or \c HashableArray.
	template<typename T>
	struct is_array_ext: public std::false_type { };

	template<typename TItem, size_t N>
	struct is_array_ext<std::array<TItem, N>>: public std::true_type { };

	template<typename TItem, size_t N>
	struct is_array_ext<HashableArray<TItem, N>>: public std::true_type { };

	template<typename T>
	constexpr bool is_array_ext_v = is_array_ext<T>::value;
}
