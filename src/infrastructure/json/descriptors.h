
#pragma once

#include <nemcpp/utils/bitwise_enum.h>

#include <type_traits>

namespace nem2_sdk { namespace internal { namespace json {
	
	namespace desc {
		
		// This namespace contains json field descriptors.
		
		enum class FieldFlags: uint8_t {
			None = 0x00,
			Optional = 0x01,
			All = 0x01
		};

		NEM2_SDK_BITWISE_ENUM(FieldFlags);

		template<FieldFlags Value>
		using Flags = std::integral_constant<FieldFlags, Value>;

		template<typename T>
		struct is_field_flags: public std::false_type { };

		template<FieldFlags Value>
		struct is_field_flags<std::integral_constant<FieldFlags, Value>>: public std::true_type { };

		template<typename T>
		constexpr bool is_field_flags_v = is_field_flags<T>::value;

		template<typename T>
		constexpr FieldFlags GetFieldFlags()
		{
			if constexpr (is_field_flags_v<T>) {
				return T::value;
			} else {
				return FieldFlags::None;
			}
		}

		template<typename T>
		constexpr bool Is_Optional = CheckAll(GetFieldFlags<T>(), FieldFlags::Optional);
	}
}}}
