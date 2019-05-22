
#pragma once

#include "infrastructure/utils/variadic_struct.h"

#include <type_traits>

namespace nem2_sdk { namespace internal { namespace binary {
	
	namespace desc {
		
		// This namespace contains binary field descriptors.
		
		template<typename TName>
		struct VariableSize {
			template<typename... TFields>
			static size_t GetSize(const VariadicStruct<TFields...>& obj)
			{
				using Traits = struct_field_by_name<TName, VariadicStruct<TFields...>>;
				
				if constexpr (std::is_integral_v<typename Traits::ValueType>) {
					return static_cast<size_t>(obj.template value<Traits::Id()>());
				} else {
					static_assert(sizeof(TName) == 0, "invalid size field type");
				}
			}
		};
		
		template<typename TName>
		struct TrailingSize {
			template<typename... TFields>
			static size_t GetTotalSize(const VariadicStruct<TFields...>& obj)
			{
				using Traits = struct_field_by_name<TName, VariadicStruct<TFields...>>;
				
				if constexpr (std::is_integral_v<typename Traits::ValueType>) {
					return static_cast<size_t>(obj.template value<Traits::Id()>());
				} else {
					static_assert(sizeof(TName) == 0, "invalid total size field type");
				}
			}
		};
		
		template<typename>
		constexpr bool is_variable_size_v = false;
		
		template<typename TName>
		constexpr bool is_variable_size_v<VariableSize<TName>> = true;
		
		template<typename>
		constexpr bool is_trailing_size_v = false;
		
		template<typename TName>
		constexpr bool is_trailing_size_v<TrailingSize<TName>> = true;
	}
}}}
