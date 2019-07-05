
#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

namespace nem2_sdk { namespace internal {
	
	template<typename TName, typename TValue, typename TDescriptor> class Field;
	template<typename... TArgs> class VariadicStruct;
	
	template<typename> struct is_variadic_struct;
	template<typename TStruct> struct struct_size;
	template<typename TStruct> struct struct_parent;
	template<size_t I, typename TStruct> struct struct_field_by_index;
	template<uint64_t Id, typename TStruct> struct struct_field_by_id;
	template<typename TLiteral, typename TStruct> struct struct_field_by_name;

	// Compile-time FNV-1a hash.
	constexpr uint64_t FnvHash(const char *str, uint64_t hash = 14695981039346656037ull)
	{
		return *str ? FnvHash(str + 1, (hash ^ *str) * 109951162821ull) : hash;
	}

	// Compile-time string literal (used as variadic struct field name).
	template<char... chars>
	struct StringLiteral {
		static constexpr const char Value[sizeof...(chars) + 1] = {chars..., '\0'};
	};
	
	// Macros for defining compile-time string literal type from c-style string literal.
#if NEM2_USE_STRING_LITERAL_OPERATOR_TEMPLATE

	template<typename T, T... chars>
	constexpr StringLiteral<chars...> operator""_sl()
	{
		return {};
	}

#define STR_LITERAL(STR) decltype(STR##_sl)
#else
#define EXTRACT_CHAR(STR, IDX) (IDX < sizeof(STR)) ? STR[IDX] : '\0'
#define STR_LITERAL(STR) StringLiteral< \
	EXTRACT_CHAR(STR, 0),  EXTRACT_CHAR(STR, 1),  EXTRACT_CHAR(STR, 2),  EXTRACT_CHAR(STR, 3),  EXTRACT_CHAR(STR, 4),  \
	EXTRACT_CHAR(STR, 5),  EXTRACT_CHAR(STR, 6),  EXTRACT_CHAR(STR, 7),  EXTRACT_CHAR(STR, 8),  EXTRACT_CHAR(STR, 9),  \
	EXTRACT_CHAR(STR, 10), EXTRACT_CHAR(STR, 11), EXTRACT_CHAR(STR, 12), EXTRACT_CHAR(STR, 13), EXTRACT_CHAR(STR, 14), \
	EXTRACT_CHAR(STR, 15), EXTRACT_CHAR(STR, 16), EXTRACT_CHAR(STR, 17), EXTRACT_CHAR(STR, 18), EXTRACT_CHAR(STR, 19), \
	EXTRACT_CHAR(STR, 20), EXTRACT_CHAR(STR, 21), EXTRACT_CHAR(STR, 22), EXTRACT_CHAR(STR, 23), EXTRACT_CHAR(STR, 24), \
	EXTRACT_CHAR(STR, 25), EXTRACT_CHAR(STR, 26), EXTRACT_CHAR(STR, 27), EXTRACT_CHAR(STR, 28), EXTRACT_CHAR(STR, 29), \
	EXTRACT_CHAR(STR, 30), EXTRACT_CHAR(STR, 31), EXTRACT_CHAR(STR, 32), EXTRACT_CHAR(STR, 33), EXTRACT_CHAR(STR, 34), \
	EXTRACT_CHAR(STR, 35), EXTRACT_CHAR(STR, 36), EXTRACT_CHAR(STR, 37), EXTRACT_CHAR(STR, 38), EXTRACT_CHAR(STR, 39), \
	EXTRACT_CHAR(STR, 40), EXTRACT_CHAR(STR, 41), EXTRACT_CHAR(STR, 42), EXTRACT_CHAR(STR, 43), EXTRACT_CHAR(STR, 44), \
	EXTRACT_CHAR(STR, 45), EXTRACT_CHAR(STR, 46), EXTRACT_CHAR(STR, 47), EXTRACT_CHAR(STR, 48), EXTRACT_CHAR(STR, 49)  \
	>
#endif
	
	// Indicator that value is not initialized in variadic struct constructor.
	struct NullValueType { };
	constexpr NullValueType Null_Value{};
	
	// Indicator that value is default initialized in variadic struct constructor.
	struct DefaultValueType { };
	constexpr DefaultValueType Default_Value{};
	
	// Indicator that all struct fields should be initialized to default values.
	struct DefaultInitializedStructType { };
	constexpr DefaultInitializedStructType Default_Initialized{};
	
	// Variadic struct field traits.
	template<typename TName, typename TValue, typename TDescriptor>
	struct FieldTraits {
		using NameType = TName;
		using ValueType = TValue;
		using DescriptorType = TDescriptor;
		
		static constexpr const char* Name()
		{ return NameType::Value; }
		
		static constexpr uint64_t Id()
		{ return FnvHash(NameType::Value); }
	};
	
	// Variadic struct field.
	template<typename TName, typename TValue, typename TDescriptor = void>
	class Field {
	public:
		using Traits = FieldTraits<TName, TValue, TDescriptor>;
		
	public:
		constexpr Field():
			value_{},
			isSet_(false)
		{ }
		
		constexpr Field(const TValue& value):
			value_(value),
			isSet_(true)
		{ }
		
		constexpr Field(TValue&& value):
			value_(std::move(value)),
			isSet_(true)
		{ }
		
		constexpr Field(NullValueType): Field{} { }
		constexpr Field(DefaultValueType): Field(GetDefaultInitializedValue()) { }
		
		template<
			uint64_t FieldId,
			typename = typename std::enable_if<FieldId == Traits::Id()>::type>
		constexpr const TValue& value() const&
		{ return value_; }
		
		template<
			uint64_t FieldId,
			typename = typename std::enable_if<FieldId == Traits::Id()>::type>
		constexpr TValue& value() &
		{ return value_; }
		
		template<
			uint64_t FieldId,
			typename = typename std::enable_if<FieldId == Traits::Id()>::type>
		constexpr const TValue&& value() const&&
		{ return std::move(value_); }
		
		template<
			uint64_t FieldId,
			typename = typename std::enable_if<FieldId == Traits::Id()>::type>
		constexpr TValue&& value() &&
		{ return std::move(value_); }
		
		template<
			uint64_t FieldId,
			typename TArg,
			typename = typename std::enable_if<FieldId == Traits::Id()>::type>
		constexpr bool set(TArg&& value)
		{
			if constexpr (!std::is_convertible_v<TArg, TValue>) {
				static_assert(sizeof(TArg) == 0, "argument can't be converted to field value");
			}
			
			using Type = typename std::decay<TArg>::type;
			bool result = true;
			
			if constexpr (std::is_arithmetic_v<Type> && std::is_arithmetic_v<TValue>) {
				if constexpr (std::is_integral_v<Type> && std::is_integral_v<TValue>) {
					if constexpr (std::is_signed_v<Type> != std::is_signed_v<TValue>) {
						result = value >= 0 && value <= std::numeric_limits<TValue>::max();
					} else if constexpr (sizeof(Type) >= sizeof(TValue)) {
						result = value >= std::numeric_limits<TValue>::min() && value <= std::numeric_limits<TValue>::max();
					}
				} else if constexpr (std::is_floating_point_v<Type> != std::is_floating_point_v<TValue> ||
				                     sizeof(Type) > sizeof(TValue)) {
					static_assert(sizeof(TArg) == 0, "dangerous argument conversion to field value");
				}
				
				value_ = result ? static_cast<TValue>(value) : TValue{};
			} else {
				value_ = std::forward<TArg>(value);
			}
			
			isSet_ = result;
			return result;
		}
		
		template<
			uint64_t FieldId,
			typename = typename std::enable_if<FieldId == Traits::Id()>::type>
		constexpr const bool& isSet() const
		{
			return isSet_;
		}
		
		template<
			uint64_t FieldId,
			typename = typename std::enable_if<FieldId == Traits::Id()>::type>
		constexpr bool& isSet()
		{
			return isSet_;
		}
		
	private:
		static constexpr TValue GetDefaultInitializedValue()
		{
			if constexpr (is_variadic_struct<TValue>::value) {
				return TValue(Default_Initialized);
			} else {
				return TValue{};
			}
		}
		
	private:
		TValue value_;
		bool isSet_;
	};
	
	// Overloaded field name literal.
	constexpr uint64_t operator""_(const char* fieldName, size_t)
	{
		return FnvHash(fieldName);
	}
	
	// Tuple-like variadic struct (no parent).
	template<typename... TFields>
	class VariadicStruct: private TFields... {
	public:
		using SelfType = VariadicStruct<TFields...>;
		using ParentType = VariadicStruct<>;
		
		using TFields::value...;
		using TFields::set...;
		using TFields::isSet...;
		
	public:
		VariadicStruct() = default;
		
		constexpr VariadicStruct(DefaultInitializedStructType): TFields(Default_Value)...
		{ }
		
		template<
			typename... TArgs,
			typename = typename std::enable_if<sizeof...(TArgs) <= sizeof...(TFields)>::type,
			typename = typename std::enable_if<sizeof...(TArgs) != 1 || // should not hide default copy and move!
			                                   !(std::is_same_v<typename std::decay<TArgs>::type, SelfType> && ...)>::type>
		constexpr VariadicStruct(TArgs&&... args):
			VariadicStruct(std::tuple_cat(std::forward_as_tuple(std::forward<TArgs>(args)...),
			                              make_tail(std::make_index_sequence<sizeof...(TFields) - sizeof...(TArgs)>{})),
			               std::make_index_sequence<sizeof...(TFields)>{})
		{ }
		
	private:
		template<typename... TArgs, size_t... Idx>
		constexpr VariadicStruct(std::tuple<TArgs...> args, std::index_sequence<Idx...>):
			TFields(std::forward<TArgs>(std::get<Idx>(args)))...
		{ }
		
	private:
		template<size_t I>
		using TailType = NullValueType;
		
		template<size_t... Idx>
		static constexpr auto make_tail(std::index_sequence<Idx...>)
		{
			return std::forward_as_tuple(TailType<Idx>{}...);
		}
	};
	
	// Tuple-like variadic struct which inherits from another variadic struct.
	template<typename... TParentFields, typename... TFields>
	class VariadicStruct<VariadicStruct<TParentFields...>, TFields...>: public VariadicStruct<TParentFields...>,
	                                                                    private TFields... {
	public:
		using SelfType = VariadicStruct<VariadicStruct<TParentFields...>, TFields...>;
		using ParentType = VariadicStruct<TParentFields...>;
		
		using ParentType::value;
		using ParentType::set;
		using ParentType::isSet;
		
		using TFields::value...;
		using TFields::set...;
		using TFields::isSet...;
		
	public:
		VariadicStruct() = default;
		
		constexpr VariadicStruct(DefaultInitializedStructType):
			ParentType(Default_Initialized),
			TFields(Default_Value)...
		{ }
		
		template<
			typename... TArgs,
			typename = typename std::enable_if<sizeof...(TArgs) <= struct_size<ParentType>::value + sizeof...(TFields)>::type,
			typename = typename std::enable_if<sizeof...(TArgs) != 1 || // should not hide default copy and move!
			                                   !(std::is_same_v<typename std::decay<TArgs>::type, SelfType> && ...)>::type>
		constexpr VariadicStruct(TArgs&&... args):
			VariadicStruct(std::tuple_cat(std::forward_as_tuple(std::forward<TArgs>(args)...),
			                              make_tail(std::make_index_sequence<struct_size<ParentType>::value +
			                                                                 sizeof...(TFields) - sizeof...(TArgs)>{})),
			               std::make_index_sequence<struct_size<ParentType>::value>{},
			               std::make_index_sequence<sizeof...(TFields)>{})
		{ }
		
	private:
		template<typename... TArgs, size_t... ParentIdx, size_t... FieldIdx>
		constexpr VariadicStruct(std::tuple<TArgs...> args,
		                         std::index_sequence<ParentIdx...> parentIdx,
		                         std::index_sequence<FieldIdx...> fieldIdx):
			VariadicStruct(splitArgs<0>(args, parentIdx), parentIdx,
			               splitArgs<sizeof...(ParentIdx)>(args, fieldIdx), fieldIdx)
		{ }
		
		template<typename... TParentArgs, size_t... ParentIdx,
		         typename... TFieldArgs,  size_t... FieldIdx>
		VariadicStruct(std::tuple<TParentArgs...> parentArgs, std::index_sequence<ParentIdx...>,
		               std::tuple<TFieldArgs...>  fieldArgs,  std::index_sequence<FieldIdx...>):
			ParentType(std::forward<TParentArgs>(std::get<ParentIdx>(parentArgs))...),
			TFields(std::forward<TFieldArgs>(std::get<FieldIdx>(fieldArgs)))...
		{ }
		
	private:
		template<size_t I>
		using TailType = NullValueType;
		
		template<size_t... Idx>
		static constexpr auto make_tail(std::index_sequence<Idx...>)
		{
			return std::forward_as_tuple(TailType<Idx>{}...);
		}
		
		template<size_t Start, typename... TArgs, size_t... Idx>
		static constexpr auto splitArgs(const std::tuple<TArgs...>& args, std::index_sequence<Idx...>)
		{
			return std::forward_as_tuple(std::forward<typename std::tuple_element<Start + Idx, std::tuple<TArgs...>>::type>(
				std::get<Start + Idx>(args))...);
		}
	};
	
	// Empty variadic struct.
	using NullStruct = VariadicStruct<>;
	
	// Variadic struct traits.
	//==========================================================================
	
	template<typename>
	struct is_variadic_struct: public std::false_type { };
	
	template<typename... TArgs>
	struct is_variadic_struct<VariadicStruct<TArgs...>>: public std::true_type { };
	
	template<typename T>
	constexpr bool is_variadic_struct_v = is_variadic_struct<T>::value;
	
	template<typename... TFields>
	struct struct_size<VariadicStruct<TFields...>>: public std::integral_constant<size_t, sizeof...(TFields)>
	{ };
	
	template<typename... TParentFields, typename... TFields>
	struct struct_size<VariadicStruct<VariadicStruct<TParentFields...>, TFields...>>:
		public std::integral_constant<size_t, struct_size<VariadicStruct<TParentFields...>>::value + sizeof...(TFields)>
	{ };
	
	template<typename... TFields>
	struct struct_parent<VariadicStruct<TFields...>> {
		using type = typename VariadicStruct<TFields...>::ParentType;
	};
	
	template<size_t I, typename TName, typename TValue, typename TDescriptor, typename... TFields>
	struct struct_field_by_index<I, VariadicStruct<Field<TName, TValue, TDescriptor>, TFields...>>:
		public struct_field_by_index<I - 1, VariadicStruct<TFields...>>
	{ };
	
	template<typename TName, typename TValue, typename TDescriptor, typename... TFields>
	struct struct_field_by_index<0, VariadicStruct<Field<TName, TValue, TDescriptor>, TFields...>>:
		public FieldTraits<TName, TValue, TDescriptor>
	{ };
	
	template<size_t I, typename... TParentFields, typename... TFields>
	struct struct_field_by_index<I, VariadicStruct<VariadicStruct<TParentFields...>, TFields...>>:
		public std::conditional<
			I < struct_size<VariadicStruct<TParentFields...>>::value,
			struct_field_by_index<I, VariadicStruct<TParentFields...>>,
			struct_field_by_index<I - struct_size<VariadicStruct<TParentFields...>>::value, VariadicStruct<TFields...>>>::type
	{ };
	
	template<size_t I> struct struct_field_by_index<I, NullStruct> {
		static_assert(sizeof(I) == 0, "variadic struct field index is out of range");
	};
	
	template<uint64_t Id, typename TName, typename TValue, typename TDescriptor, typename... TFields>
	struct struct_field_by_id<Id, VariadicStruct<Field<TName, TValue, TDescriptor>, TFields...>>:
		public std::conditional<Id == FnvHash(TName::Value),
		                        FieldTraits<TName, TValue, TDescriptor>,
		                        struct_field_by_id<Id, VariadicStruct<TFields...>>>::type
	{ };
	
	template<uint64_t Id, typename... TParentFields, typename... TFields>
	struct struct_field_by_id<Id, VariadicStruct<VariadicStruct<TParentFields...>, TFields...>>:
		public struct_field_by_id<Id, VariadicStruct<TParentFields...>>,
		public struct_field_by_id<Id, VariadicStruct<TFields...>>
	{ };
	
	template<uint64_t Id>
	struct struct_field_by_id<Id, NullStruct>
	{ };
	
	template<char... chars, typename... TFields>
	struct struct_field_by_name<StringLiteral<chars...>, VariadicStruct<TFields...>>:
		public struct_field_by_id<FnvHash(StringLiteral<chars...>::Value), VariadicStruct<TFields...>>
	{ };
	
	// For strutured binding ADL lookup.
	template<size_t I, typename... TFields>
	constexpr auto get(const VariadicStruct<TFields...>& obj) ->
		typename std::tuple_element<I, VariadicStruct<TFields...>>::type const&
	{
		return obj.template value<struct_field_by_index<I, VariadicStruct<TFields...>>::Id()>();
	}
	
	template<size_t I, typename... TFields>
	constexpr auto get(VariadicStruct<TFields...>& obj) ->
		typename std::tuple_element<I, VariadicStruct<TFields...>>::type&
	{
		return obj.template value<struct_field_by_index<I, VariadicStruct<TFields...>>::Id()>();
	}
	
	template<size_t I, typename... TFields>
	constexpr auto get(const VariadicStruct<TFields...>&& obj) ->
		typename std::tuple_element<I, VariadicStruct<TFields...>>::type const&&
	{
		return (std::move(obj)).template value<struct_field_by_index<I, VariadicStruct<TFields...>>::Id()>();
	}
	
	template<size_t I, typename... TFields>
	constexpr auto get(VariadicStruct<TFields...>&& obj) ->
		typename std::tuple_element<I, VariadicStruct<TFields...>>::type&&
	{
		return (std::move(obj)).template value<struct_field_by_index<I, VariadicStruct<TFields...>>::Id()>();
	}
	
	// Variadic struct template variables
	//==========================================================================
	
	template<size_t I, typename TStruct>
	constexpr uint64_t Field_Id = struct_field_by_index<I, TStruct>::Id();
	
	template<size_t I, typename TStruct>
	constexpr const char* Field_Name = struct_field_by_index<I, TStruct>::Name();
	
	// Variadic struct comparison operators
	//==========================================================================
	
	template<typename... TFields, size_t... Idx>
	bool AreEqual(const VariadicStruct<TFields...>& lhs, const VariadicStruct<TFields...>& rhs, std::index_sequence<Idx...>)
	{
		using Type = VariadicStruct<TFields...>;
		return
			(
				(
					( lhs.template isSet<Field_Id<Idx, Type>>() && rhs.template isSet<Field_Id<Idx, Type>>() &&
					  lhs.template value<Field_Id<Idx, Type>>() == rhs.template value<Field_Id<Idx, Type>>() )
					||
					( !lhs.template isSet<Field_Id<Idx, Type>>() && !rhs.template isSet<Field_Id<Idx, Type>>() )
				) && ...
			);
	}
	
	template<typename... TFields>
	bool operator==(const VariadicStruct<TFields...>& lhs, const VariadicStruct<TFields...>& rhs)
	{
		return AreEqual(lhs, rhs, std::make_index_sequence<struct_size<VariadicStruct<TFields...>>::value>{});
	}
	
	template<typename... TFields>
	bool operator!=(const VariadicStruct<TFields...>& lhs, const VariadicStruct<TFields...>& rhs)
	{
		return !(lhs == rhs);
	}
}}

namespace std {
	
	template<typename... TFields>
	class tuple_size<nem2_sdk::internal::VariadicStruct<TFields...>>:
		public std::integral_constant<size_t, nem2_sdk::internal::struct_size<nem2_sdk::internal::VariadicStruct<TFields...>>::value>
	{ };
	
	template<size_t I, typename... TFields>
	struct tuple_element<I, nem2_sdk::internal::VariadicStruct<TFields...>> {
		using type = typename nem2_sdk::internal::struct_field_by_index<I, nem2_sdk::internal::VariadicStruct<TFields...>>::ValueType;
	};
}
