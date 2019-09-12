
#pragma once

#include <functional>
#include <type_traits>

namespace xpx_sdk {
	
	/// Compares two data members of user-defined type.
	/// \tparam TType User-defined type.
	/// \tparam TDataMember Type of data member from \c TType.
	/// \tparam data Pointer to data member.
	/// \tparam TCompare Template type for performing comparison of values (instantiated with
	///         type which is a \c TDataMember without reference and cv-qualifiers).
	template<typename TType, typename TDataMember, TDataMember TType::*data,
	         template<typename> class TCompare = std::less>
	class data_comparator {
	public:
		using type = TType;
		using value_type = std::remove_cv_t<std::remove_reference_t<TDataMember>>;
		using compare_type = TCompare<value_type>;
		using is_transparent = std::true_type;
		
		constexpr bool operator()(const type& lhs, const type& rhs) const
		{
			return compare_type{}(lhs.*data, rhs.*data);
		}
		
		constexpr bool operator()(const type& lhs, const value_type& rhs) const
		{
			return compare_type{}(lhs.*data, rhs);
		}
		
		constexpr bool operator()(const value_type& lhs, const type& rhs) const
		{
			return compare_type{}(lhs, rhs.*data);
		}
		
		static constexpr TDataMember get_value(const type& obj)
		{
			return obj.*data;
		}
	};
	
	/// Compares return values of two methods of user-defined type.
	/// \tparam TType User-defined type.
	/// \tparam TReturnValue Type of return value of method from \c TType.
	/// \tparam method Pointer to method.
	/// \tparam TCompare Template type for performing comparison of values (accepts arguments of type
	///         which is a \c TReturnValue with removed reference and cv-qualifiers).
	/// \note Method should be \c const and have empty parameter list.
	template<typename TType, typename TReturnValue, TReturnValue (TType::*method)() const,
	         template <typename> class TCompare = std::less>
	class method_comparator {
	public:
		using type = TType;
		using value_type = std::remove_cv_t<std::remove_reference_t<TReturnValue>>;
		using compare_type = TCompare<value_type>;
		using is_transparent = std::true_type;
		
		constexpr bool operator()(const type& lhs, const type& rhs) const
		{
			return compare_type{}((lhs.*method)(), (rhs.*method)());
		}
		
		constexpr bool operator()(const type& lhs, const value_type& rhs) const
		{
			return compare_type{}((lhs.*method)(), rhs);
		}
		
		constexpr bool operator()(const value_type& lhs, const type& rhs) const
		{
			return compare_type{}(lhs, (rhs.*method)());
		}
		
		static constexpr TReturnValue get_value(const type& obj)
		{
			return (obj.*method)();
		}
	};
	
	/// Compares arbitrary number of user-defined type data members and methods return values.
	/// \tparam TMemberComparator Either \c data_comparator or \c member_comparator.
	/// \note Data members and methods return values are compared in order of \c TMemberComparators.
	template<typename TMemberComparator, typename... TMemberComparators>
	class compound_comparator: public compound_comparator<TMemberComparators...> {
	public:
		using type = typename TMemberComparator::type;
		using compare_type = TMemberComparator;
		using base_type = compound_comparator<TMemberComparators...>;
		static_assert(std::is_same_v<type, typename base_type::type>,
		              "different user-defined types in member comparators");
		
		constexpr bool operator()(const type& lhs, const type& rhs) const
		{
			if (compare_type::get_value(lhs) == compare_type::get_value(rhs)) {
				return base_type::operator()(lhs, rhs);
			}
			
			return compare_type{}(lhs, rhs);
		}
	};
	
	/// Compares user-defined type data members or methods return values.
	/// \tparam TMemberComparator Either \c data_comparator or \c member_comparator.
	template<typename TMemberComparator>
	class compound_comparator<TMemberComparator> {
	public:
		using type = typename TMemberComparator::type;
		using compare_type = TMemberComparator;
		
		constexpr bool operator()(const type& lhs, const type& rhs) const
		{
			return compare_type{}(lhs, rhs);
		}
	};
}
