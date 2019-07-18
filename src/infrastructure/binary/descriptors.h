
#pragma once

#include "infrastructure/utils/variadic_struct.h"

#include <type_traits>

namespace nem2_sdk { namespace internal { namespace binary {
	
	namespace desc {
		
		// This namespace contains binary field descriptors.
		
		template<typename TName>
		struct VariableSize {
			using SizeFieldName = TName;

			template<typename... TFields>
			static size_t GetSize(const VariadicStruct<TFields...>& obj)
			{
				using Traits = struct_field_by_name<SizeFieldName, VariadicStruct<TFields...>>;
				return static_cast<size_t>(obj.template value<Traits::Id()>());
			}
		};
		
		template<typename TName>
		struct TrailingSize {
			using SizeFieldName = TName;

			template<typename... TFields>
			static size_t GetTotalSize(const VariadicStruct<TFields...>& obj)
			{
				using Traits = struct_field_by_name<SizeFieldName, VariadicStruct<TFields...>>;
				return static_cast<size_t>(obj.template value<Traits::Id()>());
			}
		};

		template<typename T>
		struct is_variable_size: public std::false_type { };
		
		template<typename TName>
		struct is_variable_size<VariableSize<TName>>: public std::true_type { };

		template<typename T>
		constexpr bool is_variable_size_v = is_variable_size<T>::value;

		template<typename T>
		struct is_trailing_size: public std::false_type { };

		template<typename TName>
		struct is_trailing_size<TrailingSize<TName>>: public std::true_type { };

		template<typename T>
		constexpr bool is_trailing_size_v = is_trailing_size<T>::value;
	}
}}}
