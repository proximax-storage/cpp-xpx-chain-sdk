
#pragma once

#include <type_traits>
#include <typeindex>

namespace nem2_sdk {
	
	/// Calculates hash for data member of user-defined type.
	/// \tparam TType User-defined type.
	/// \tparam TDataMember Type of data member from user-defined type.
	/// \tparam data Pointer to data member.
	/// \tparam THasher Template type for performing hashing of values (accepts arguments of type
	///         which is a \c TDataMember with removed reference and cv-qualifiers).
	template<typename TType, typename TDataMember, TDataMember TType::*data,
	         template<typename> class THasher = std::hash>
	class data_hasher {
	public:
		using type = TType;
		using value_type = std::remove_cv_t<std::remove_reference_t<TDataMember>>;
		using hash_type = THasher<value_type>;
		
		size_t operator()(const type& obj) const
		{
			return hash_type{}(obj.*data);
		}
	};
	
	/// Calculates hash for return value of method of user-defined type.
	/// \tparam TType User-defined type.
	/// \tparam TReturnValue Type of method return value.
	/// \tparam method Pointer to method.
	/// \tparam THasher Template type for performing hashing of values (accepts arguments of type
	///         which is a \c TReturnValue with removed reference and cv-qualifiers).
	/// \note Method should be \c const and have empty parameter list.
	template<typename TType, typename TReturnValue, TReturnValue (TType::*method)() const,
	         template<typename> class THasher = std::hash>
	class method_hasher {
	public:
		using type = TType;
		using value_type = std::remove_cv_t<std::remove_reference_t<TReturnValue>>;
		using hash_type = THasher<value_type>;
		
		size_t operator()(const type& obj) const
		{
			return hash_type{}((obj.*method)());
		}
	};
	
	/// Combines hashes of arbitrary number of user-defined type data members and methods return values.
	/// \tparam TMemberHasher Either \c data_hasher or \c member_hasher.
	template<typename TMemberHasher, typename... TMemberHashers>
	class compound_hasher: public compound_hasher<TMemberHashers...> {
	public:
		using type = typename TMemberHasher::type;
		using hash_type = TMemberHasher;
		using base_type = compound_hasher<TMemberHashers...>;
		static_assert(std::is_same_v<type, typename base_type::type>,
		              "different user-defined types in member comparators");
		
	public:
		size_t operator()(const type& obj) const
		{
			return combine(obj, 0);
		}
		
	protected:
		using base_type::combine_impl;
		
		size_t combine(const type& obj, size_t seed) const
		{
			auto hash = hash_type{}(obj);
			seed = combine_impl(hash, seed);
			return base_type::combine(obj, seed);
		}
	};
	
	/// Combines hashes of arbitrary number of user-defined type data members and methods return values.
	/// \tparam TMemberHasher Either \c data_hasher or \c member_hasher.
	template<typename TMemberHasher>
	class compound_hasher<TMemberHasher> {
	public:
		using type = typename TMemberHasher::type;
		using hash_type = TMemberHasher;
		
	public:
		size_t operator()(const type& obj) const
		{
			return combine(obj, 0);
		}
		
	protected:
		size_t combine(const type& obj, size_t seed) const
		{
			auto hash = hash_type{}(obj);
			return combine_impl(hash, seed);
		}
		
		size_t combine_impl(size_t hash, size_t seed) const
		{
			// Implementation is taken from Boost hash_combine 
			const uint64_t m = 0xc6a4a7935bd1e995ull;
			const int r = 47;
			
			hash *= m;
			hash ^= hash >> r;
			hash *= m;
			
			seed ^= hash;
			seed *= m;
			seed += 0xe6546b64;
			
			return seed;
		}
	};
}
