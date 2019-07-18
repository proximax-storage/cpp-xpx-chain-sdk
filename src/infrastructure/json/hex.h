#pragma once

#include <utility>

namespace nem2_sdk { namespace internal { namespace json { 

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
