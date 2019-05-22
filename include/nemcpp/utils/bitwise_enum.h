
#pragma once

#include <nemcpp/types.h>

/// Creates neccessary overloads in order to use enum values as binary flags.
#define NEM2_SDK_BITWISE_ENUM(ENUM_TYPE) \
	constexpr ENUM_TYPE operator|(ENUM_TYPE lhs, ENUM_TYPE rhs) \
	{ return static_cast<ENUM_TYPE>(to_underlying_type(lhs) | to_underlying_type(rhs)); } \
	\
	constexpr ENUM_TYPE& operator|=(ENUM_TYPE& lhs, ENUM_TYPE rhs) \
	{ return lhs = lhs | rhs; } \
	\
	constexpr ENUM_TYPE operator&(ENUM_TYPE lhs, ENUM_TYPE rhs) \
	{ return static_cast<ENUM_TYPE>(to_underlying_type(lhs) & to_underlying_type(rhs)); } \
	\
	constexpr ENUM_TYPE& operator&=(ENUM_TYPE& lhs, ENUM_TYPE rhs) \
	{ return lhs = lhs & rhs; } \
	\
	constexpr ENUM_TYPE operator~(ENUM_TYPE value) \
	{ \
		auto tmp = ~(to_underlying_type(value)); \
		return static_cast<ENUM_TYPE>(tmp & to_underlying_type(ENUM_TYPE::All)); \
	} \
	\
	constexpr bool CheckAll(ENUM_TYPE value, ENUM_TYPE flags) \
	{ return to_underlying_type(flags) == (to_underlying_type(value) & to_underlying_type(flags)); } \
	\
	constexpr bool CheckAny(ENUM_TYPE value, ENUM_TYPE flags) \
	{ return to_underlying_type(ENUM_TYPE::None) != (to_underlying_type(value) & to_underlying_type(flags)); }
