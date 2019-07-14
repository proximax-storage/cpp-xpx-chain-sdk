
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
