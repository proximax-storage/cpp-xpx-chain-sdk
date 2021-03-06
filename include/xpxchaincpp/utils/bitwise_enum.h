/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>

/// Creates neccessary overloads in order to use enum values as binary flags.
#define XPX_CHAIN_SDK_BITWISE_ENUM(ENUM_TYPE) \
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
