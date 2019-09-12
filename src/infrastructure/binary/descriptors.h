/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include "infrastructure/utils/variadic_struct.h"

#include <type_traits>

namespace xpx_chain_sdk { namespace internal { namespace binary {
	
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
