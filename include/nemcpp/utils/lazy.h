
#pragma once

#include <functional>
#include <optional>
#include <type_traits>

namespace nem2_sdk {
	
	/// Wrapper for lazy-initialized data member of user-defined type.
	/// \tparam TMemberType Data member type.
	/// \tparam TType User-defined type.
	/// \tparam initialize Method to be called for data member initialization.
	/// \note Because \c initialize should be bound to concrete class instance, copy/move constructors
	///       are deleted (we can't obtain surrounding class instance in them). Also we do not copy/move
	///       \c initialize in assignment operations.
	template<typename TMemberType, typename TType, void (TType::*initialize)(std::optional<TMemberType>& value)>
	class Lazy {
	public:
		/// User-defined type.
		using ClassType = TType;
		
		/// User-defined type data member type.
		using MemberType = TMemberType;
		
	public:
		/// Creates wrapper for lazy-initialized data member of \a obj using \a value as initial value.
		explicit Lazy(ClassType* obj, const std::optional<MemberType>& value = std::nullopt):
			initialize_(std::bind(initialize, obj, std::placeholders::_1)),
			value_(value)
		{ }
		
		/// Creates wrapper for lazy-initialized data member of \a obj using \a value as initial value.
		explicit Lazy(ClassType* obj, std::optional<MemberType>&& value):
			initialize_(std::bind(initialize, obj, std::placeholders::_1)),
			value_(std::move(value))
		{ }
		
		/// Deleted copy/move constructors.
		Lazy(const Lazy& rhs) = delete;
		Lazy(Lazy&& rhs) = delete;
		
		/// Copies wrapped value from \a rhs.
		Lazy& operator=(const Lazy& rhs)
		{
			value_ = rhs.value_;
			return *this;
		}
		
		/// Moves wrapped value from \a rhs.
		Lazy& operator=(Lazy&& rhs) noexcept(std::is_nothrow_move_assignable<std::optional<MemberType>>::value)
		{
			value_ = std::move(rhs.value_);
			return *this;
		}
		
		/// Sets wrapped value.
		template<
			typename T,
			typename = typename std::enable_if<std::is_assignable_v<MemberType, T&&>>::type>
		Lazy& operator=(T&& value)
		{
			value_ = std::forward<T>(value);
			return *this;
		}
		
		/// Sets wrapped value in-place.
		template<typename... TArgs>
		MemberType& emplace(TArgs&&... args)
		{
			value_.emplace(args...);
			return *value_;
		}
		
		/// Returns wrapped value initializing it if necessary.
		MemberType& value() &
		{
			if (!value_) {
				initialize_(value_);
			}
			
			return *value_;
		}
		
		/// Returns wrapped value initializing it if necessary.
		const MemberType& value() const&
		{
			if (!value_) {
				initialize_(value_);
			}
			
			return *value_;
		}

		/// Returns wrapped value initializing it if necessary.
		MemberType&& value() &&
		{
			if (!value_) {
				initialize_(value_);
			}

			return *std::move(value_);
		}

		/// Returns wrapped value initializing it if necessary.
		const MemberType&& value() const&&
		{
			if (!value_) {
				initialize_(value_);
			}

			return *std::move(value_);
		}

		/// Returns wrapped value initializing it if necessary.
		MemberType& operator*() &
		{
			return value();
		}
		
		/// Returns wrapped value initializing it if necessary.
		const MemberType& operator*() const&
		{
			return value();
		}

		/// Returns wrapped value initializing it if necessary.
		MemberType&& operator*() &&
		{
			return std::move(*this).value();
		}

		/// Returns wrapped value initializing it if necessary.
		const MemberType&& operator*() const&&
		{
			return std::move(*this).value();
		}

		/// Returns wrapped value initializing it if necessary.
		MemberType* operator->()
		{
			return &value();
		}
		
		/// Returns wrapped value initializing it if necessary.
		const MemberType* operator->() const
		{
			return &value();
		}
		
		/// Returns \c true if wrapped value is initialized.
		explicit operator bool() const
		{
			return static_cast<bool>(value_);
		}
		
	public:
		std::function<void (std::optional<MemberType>& value)> initialize_;
		mutable std::optional<MemberType> value_;
	};
}
