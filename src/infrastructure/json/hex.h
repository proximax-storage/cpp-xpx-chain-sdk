/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <utility>

namespace xpx_chain_sdk { namespace internal { namespace json {

	template<typename T>
	class Hex {
	public:
		using WrappedType = T;
	
	public:
		Hex() = default;

		Hex(const WrappedType& data) : data_(data)
		{ }

		Hex(WrappedType&& data) : data_(std::move(data))
		{ }

		WrappedType& unwrap() &
		{ return data_;	}

		const WrappedType& unwrap() const &
		{ return data_; }

		WrappedType&& unwrap() &&
		{ return std::move(data_); }

		const WrappedType&& unwrap() const &&
		{ return std::move(data_); }

		operator WrappedType&() &
		{ return unwrap(); }

		operator const WrappedType&() const &
		{ return unwrap(); }

		operator WrappedType&&() &&
		{ return std::move(*this).unwrap(); }

		operator const WrappedType&&() const &&
		{ return std::move(*this).unwrap(); }

		Hex& operator=(const WrappedType& data)
		{
			data_ = data;
			return *this;
		}

		Hex& operator=(WrappedType&& data)
		{
			data_ = std::move(data);
			return *this;
		}

	private:
		WrappedType data_;
	};
}}}
