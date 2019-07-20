
#pragma once

#include "infrastructure/json/descriptors.h"
#include "infrastructure/json/hex.h"
#include "infrastructure/json/uint64.h"
#include "infrastructure/utils/variadic_struct.h"

#include <nemcpp/utils/format.h>
#include <nemcpp/utils/hashable_array.h>
#include <nemcpp/utils/noncopyable.h>

#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <stdio.h>
namespace nem2_sdk { namespace internal { namespace json {

	// Json DTOs should contain only following types:
	//
	// 1. other DTOs
	// 2. arithmetic (int, double (but no float!) etc), enums and special type Uint64 which is represented
	//    in json as 2 item array holding low and high 32bit parts of 64bit value
	// 3. std::vector, std::array or nem2_sdk::HashableArray with elements of any supported type
	// 4. std::string
	// 5. special template type 'Hex<T>' which marks wrapped type as hex-formatted in json; type T can be
	//    integral, enum or any of the container type from 3 if their value type is integral or enum.
	//
	// Also Json DTO can contain special field with name STR_LITERAL("__data") and type std::string. During read
	// operation this field is initialized with raw json object data. When writing DTO containing initialized "__data"
	// field, other DTOs fields are merged into it, adding/updating fields in raw json object data.

	constexpr const char* Object_Data_Field_Name = "__data";

	enum class OutputMode: uint8_t {
		Default = 0,
		Pretty
	};
	
	class ParseResult {
	public:
		ParseResult(bool result, std::string_view invalidField = {});
		
		const std::string& invalidField() const; // returns RFC6901 pointer
		operator bool() const;
		
	private:
		bool result_;
		std::string invalidField_;
	};
	
	class Parser: private NonCopyable {
	public:
		Parser() = default;
		
		bool setData(std::string_view data);
		std::string getData(OutputMode mode = OutputMode::Default) const;
		
		template<typename T>
		ParseResult read(T& value, const char* jsonPtr = nullptr) const
		{
			if (!jsonPtr) {
				jsonPtr = "";
			}
			
			rapidjson::Pointer ptr(jsonPtr);
			
			if (!ptr.IsValid()) {
				return { false, "" };
			}
			
			const rapidjson::Value* jsonValue = rapidjson::GetValueByPointer(document_, ptr);
			return Impl<T>::Read(value, jsonValue, jsonPtr);
		}
		
		template<typename T>
		ParseResult write(const T& value, const char* jsonPtr = nullptr)
		{
			if (!jsonPtr) {
				jsonPtr = "";
			}
			
			rapidjson::Pointer ptr(jsonPtr);
			
			if (!ptr.IsValid()) {
				return { false, "" };
			}
			
			rapidjson::Value& jsonValue = rapidjson::CreateValueByPointer(document_, ptr);
			return Impl<T>::Write(value, jsonValue, jsonPtr, document_);
		}
		
	public:
		
		template<typename T>
		static ParseResult Read(T& obj, std::string_view data, const char* jsonPtr = nullptr)
		{
			Parser parser;
			
			if (!parser.setData(data)) {
				return { false, "" };
			}
			
			return parser.read(obj, jsonPtr);
		}
		
		template<typename T>
		static ParseResult Write(const T& obj, std::string& data, const char* jsonPtr = nullptr)
		{
			return Write(obj, data, jsonPtr, OutputMode::Default);
		}
		
		template<typename T>
		static ParseResult PrettyWrite(const T& obj, std::string& data, const char* jsonPtr = nullptr)
		{
			return Write(obj, data, jsonPtr, OutputMode::Pretty);
		}
		
	private:
		static std::string ValueToString(const rapidjson::Value* jsonValue, OutputMode mode = OutputMode::Default);

		template<typename T>
		static ParseResult Write(const T& obj, std::string& data, const char* jsonPtr, OutputMode mode)
		{
			Parser parser;
			auto result = parser.write(obj, jsonPtr);
			
			if (result) {
				data = parser.getData(mode);
			}
			
			return result;
		}

	private:
		template<typename TName, typename TStruct, typename TField, typename TDescriptor = void>
		static constexpr bool IsValidField() // bool return value to force compiler to instantiate this function template early
		{
			if constexpr (!desc::is_field_flags_v<TDescriptor> && !std::is_same_v<TDescriptor, void>) {
				static_assert(sizeof(TField) == 0, "invalid field descriptor (should be 'void' or 'Flags')");
			}

			if  constexpr (FnvHash(TName::Value) == FnvHash(Object_Data_Field_Name) && !std::is_same_v<TField, std::string>) {
				static_assert(sizeof(TField) == 0, "invalid '__data' field type (should be 'std::string')");
			}

			if constexpr ( (std::is_arithmetic_v<TField> && !std::is_same_v<TField, float>) ||
			               std::is_enum_v<TField> ||
			               std::is_same_v<TField, std::string> ||
			               std::is_same_v<TField, Uint64> ||
			               is_variadic_struct_v<TField>) {
				//do nothing
			} else if constexpr (is_specialization_v<TField, std::vector> || is_array_ext_v<TField>) {
				return IsValidField<TName, TStruct, typename TField::value_type>();
			} else if constexpr (is_specialization_v<TField, Hex>) {
				if constexpr (std::is_integral_v<typename TField::WrappedType> || std::is_enum_v<typename TField::WrappedType>) {
					//do nothing
				} else if constexpr (is_specialization_v<typename TField::WrappedType, std::vector> ||
				                     is_array_ext_v<typename TField::WrappedType>) {
					if constexpr  (!std::is_integral_v<typename TField::WrappedType::value_type> &&
					               !std::is_enum_v<typename TField::WrappedType::value_type>) {
						static_assert(sizeof(TField) == 0, "invalid wrapped type for hex-formatted field");
					}
				} else {
					static_assert(sizeof(TField) == 0, "invalid wrapped type for hex-formatted field");
				}
			} else {
				static_assert(sizeof(TField) == 0, "invalid field type");
			}

			return true;
		}
		
	private:
		template<typename TField>
		class Impl {
		public:
			static ParseResult Read(TField& value, const rapidjson::Value* jsonValue, const char* jsonPtr)
			{
				if (!jsonValue || jsonValue->IsNull()) {
					value = TField{};
					return true;
				}
				
				if constexpr (std::is_enum_v<TField>) {
					std::underlying_type_t<TField> rawValue{};
					auto result = Impl<std::underlying_type_t<TField>>::Read(rawValue, jsonValue, jsonPtr);

					if (result) {
						value = static_cast<TField>(rawValue);
					}
					
					return result;
				} else if constexpr (std::is_integral_v<TField>) {
					if constexpr (std::is_same_v<TField, bool>) {
						if (jsonValue->IsBool()) {
							value = jsonValue->GetBool();
							return true;
						}
					} else if constexpr (std::is_unsigned_v<TField>) {
						if (jsonValue->IsUint64()) {
							auto v = jsonValue->GetUint64();
							
							if (v >= std::numeric_limits<TField>::min() && v <= std::numeric_limits<TField>::max()) {
								value = static_cast<TField>(v);
								return true;
							}
						}
					} else {
						if (jsonValue->IsInt64()) {
							auto v = jsonValue->GetInt64();
							
							if (v >= std::numeric_limits<TField>::min() && v <= std::numeric_limits<TField>::max()) {
								value = static_cast<TField>(v);
								return true;
							}
						}
					}
				} else if constexpr (std::is_same_v<TField, double>) {
					if (jsonValue->IsNumber()) {
						value = jsonValue->GetDouble();
						return true;
					}
				} else if constexpr (std::is_same_v<TField, std::string>) {
					if (jsonValue->IsString()) {
						value = jsonValue->GetString();
						return true;
					}
				} else {
					static_assert(sizeof(TField) == 0, "field type is not supported by json parser");
				}
				
				return { false, jsonPtr };
			}
			
			static ParseResult Write(const TField& value,
			                         rapidjson::Value& jsonValue,
			                         const char* jsonPtr,
			                         rapidjson::Document& document)
			{
				if constexpr (std::is_enum_v<TField>) {
					return Impl<std::underlying_type_t<TField>>::Write(to_underlying_type(value), jsonValue, jsonPtr, document);
				} else if constexpr (std::is_integral_v<TField>) {
					if constexpr (std::is_same_v<TField, bool>) {
						jsonValue.SetBool(value);
					} else if constexpr (std::is_unsigned_v<TField>) {
						jsonValue.SetUint64(value);
					} else {
						jsonValue.SetInt64(value);
					}
				} else if constexpr (std::is_same_v<TField, double>) {
					jsonValue.SetDouble(value);
				} else if constexpr (std::is_same_v<TField, std::string>) {
					jsonValue.SetString(value.c_str(), document.GetAllocator());
				} else {
					static_assert(sizeof(TField) == 0, "field type is not supported by json parser");
				}
				
				return true;
			}
		};
		
	private:
		mutable rapidjson::Document document_;
	};
	
	// Implementation specializations for complex types

	template<typename TItem>
	class Parser::Impl<std::vector<TItem>> {
	public:
		static ParseResult Read(std::vector<TItem>& value, const rapidjson::Value* jsonValue, const char* jsonPtr)
		{
			if (!jsonValue || jsonValue->IsNull()) {
				value.clear();
				return true;
			} else if (!jsonValue->IsArray()) {
				return { false, jsonPtr };
			}
			
			value.clear();
			auto size = jsonValue->Size();
			
			for (decltype(size) i = 0; i < size; ++i) {
				std::string itemPtr = MakeString{} << jsonPtr << "/" << i;
				TItem item{};
				
				auto result = Impl<TItem>::Read(item, &(*jsonValue)[i], itemPtr.c_str());
				
				if (result) {
					value.push_back(std::move(item));
				} else {
					return result;
				}
			}
			
			return true;
		}
		
		static ParseResult Write(const std::vector<TItem>& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document)
		{
			jsonValue.SetArray();
			
			for (size_t i = 0; i< value.size(); ++i) {
				std::string itemPtr = MakeString{} << jsonPtr << "/" << i;
				rapidjson::Value itemValue;
				
				auto result = Impl<TItem>::Write(value[i], itemValue, itemPtr.c_str(), document);
				
				if (result) {
					jsonValue.PushBack(itemValue.Move(), document.GetAllocator());
				} else {
					return result;
				}
			}
			
			return true;
		}
	};
	
	template<typename TItem, size_t N>
	class Parser::Impl<std::array<TItem, N>> {
	public:
		static ParseResult Read(std::array<TItem, N>& value, const rapidjson::Value* jsonValue, const char* jsonPtr)
		{
			if (!jsonValue || jsonValue->IsNull()) {
				value = {};
				return true;
			} else if (!jsonValue->IsArray() || jsonValue->Size() != N) {
				return { false, jsonPtr };
			}
			
			std::vector<TItem> data;
			auto result = Impl<std::vector<TItem>>::Read(data, jsonValue, jsonPtr);
			
			if (result) {
				std::copy(data.begin(), data.end(), value.begin());
			}
			
			return result;
		}
		
		static ParseResult Write(const std::array<TItem, N>& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document)
		{
			std::vector<TItem> data;
			std::copy(value.begin(), value.end(), std::back_inserter(data));
			return Impl<std::vector<TItem>>::Write(data, jsonValue, jsonPtr, document);
		}
	};
	
	template<typename TItem, size_t N>
	class Parser::Impl<HashableArray<TItem, N>>: public Parser::Impl<std::array<TItem, N>> { };
	
	template<typename TWrappedType>
	class Parser::Impl<Hex<TWrappedType>> {
	public:
		static ParseResult Read(Hex<TWrappedType>& value, const rapidjson::Value* jsonValue, const char* jsonPtr)
		{
			if (!jsonValue || jsonValue->IsNull()) {
				value = TWrappedType{};
				return true;
			}

			std::string hexData;
			auto result = Impl<std::string>::Read(hexData, jsonValue, jsonPtr);

			if (result && !FromHex(hexData, value.unwrap())) {
				result = { false, jsonPtr };
			}

			return result;
		}

		static ParseResult Write(const Hex<TWrappedType>& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document)
		{
			std::string hexData = ToHex(value.unwrap());
			return Impl<std::string>::Write(hexData, jsonValue, jsonPtr, document);
		}
	};

	template<typename... TFields>
	class Parser::Impl<VariadicStruct<TFields...>> {
	public:
		using StructType = VariadicStruct<TFields...>;
		
	public:
		static ParseResult Read(StructType& value, const rapidjson::Value* jsonValue, const char* jsonPtr)
		{
			rapidjson::Value defaultValue(rapidjson::kObjectType);
			const auto* valuePtr = jsonValue && !jsonValue->IsNull() ? jsonValue : &defaultValue;
			
			if (!valuePtr->IsObject()) {
				return { false, jsonPtr };
			}
			
			return Read(value, valuePtr, jsonPtr, std::make_index_sequence<struct_size<StructType>::value>{});
		}
		
		static ParseResult Write(const StructType& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document)
		{
			jsonValue.SetObject();
			return Write(value, jsonValue, jsonPtr, document, std::make_index_sequence<struct_size<StructType>::value>{});
		}
		
		
	private:
		template<typename TTraits>
		static constexpr bool IsObjectDataField()
		{
			if constexpr (TTraits::Id() == FnvHash(Object_Data_Field_Name) &&
			              std::is_same_v<typename TTraits::ValueType, std::string>) {
				return true;
			} else {
				return false;
			}
		}

		template<size_t... Idx>
		static ParseResult Read(StructType& value,
		                        const rapidjson::Value* jsonValue,
		                        const char* jsonPtr,
		                        std::index_sequence<Idx...>)
		{
			constexpr bool isValidStruct = (IsValidField<typename struct_field_by_index<Idx, StructType>::NameType,
			                                             StructType,
			                                             typename struct_field_by_index<Idx, StructType>::ValueType,
			                                             typename struct_field_by_index<Idx, StructType>::DescriptorType>() && ...);

			if constexpr (isValidStruct) {
				ParseResult result = true;

				( ( result = ReadField<struct_field_by_index<Idx, StructType>>(value, jsonValue, jsonPtr),
				    result ) && ... );

				return result;
			} else {
				static_assert(sizeof...(Idx) < 0, "variadic struct is not supported by json parser");
			}
		}
		
		template<size_t... Idx>
		static ParseResult Write(const StructType& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document,
		                         std::index_sequence<Idx...>)
		{
			constexpr bool isValidStruct = (IsValidField<typename struct_field_by_index<Idx, StructType>::NameType,
			                                             StructType,
			                                             typename struct_field_by_index<Idx, StructType>::ValueType,
			                                             typename struct_field_by_index<Idx, StructType>::DescriptorType>() && ...);
			
			if constexpr (isValidStruct) {
				if constexpr (has_field_by_id_v<FnvHash(Object_Data_Field_Name), StructType>) {
					if (value.isSet<FnvHash(Object_Data_Field_Name)>()) {
						rapidjson::Document merged;
						merged.Parse(value.value<FnvHash(Object_Data_Field_Name)>().c_str(),
						             value.value<FnvHash(Object_Data_Field_Name)>().size());

						if (merged.HasParseError() || !merged.IsObject()) {
							std::string invalidField = MakeString{} << jsonPtr << "/" << Object_Data_Field_Name;
							return { false, invalidField };
						}

						jsonValue.CopyFrom(merged.Move(), document.GetAllocator());
					}
				}

				ParseResult result = true;

				( ( result = WriteField<struct_field_by_index<Idx, StructType>>(value, jsonValue, jsonPtr, document),
				    result ) && ... );
				return result;
			} else {
				static_assert(sizeof...(Idx) < 0, "variadic struct is not supported by json parser");
			}
		}
		
		template<typename TTraits, typename TStruct>
		static ParseResult ReadField(TStruct& obj, const rapidjson::Value* jsonValue, const char* jsonPtr)
		{
			if constexpr (IsObjectDataField<TTraits>()) {
				obj.set<FnvHash(Object_Data_Field_Name)>(ValueToString(jsonValue));
				return true;
			}

			std::string fieldPtr = MakeString{} << jsonPtr << "/" << TTraits::Name();
			auto it = jsonValue->FindMember(TTraits::Name());
			
			auto result = Impl<typename TTraits::ValueType>::Read(
				obj.template value<TTraits::Id()>(),
				it != jsonValue->MemberEnd() ? &(it->value) : nullptr,
				fieldPtr.c_str());
			
			obj.template isSet<TTraits::Id()>() =
				it != jsonValue->MemberEnd() && !(it->value.IsNull()) ? result : false;
			
			if (result) {
				bool isSet = it != jsonValue->MemberEnd() && !(it->value.IsNull());
				
				if constexpr (!desc::Is_Optional<typename TTraits::DescriptorType>) {
					if (!isSet) {
						return { false, fieldPtr };
					}
				}
				
				obj.template isSet<TTraits::Id()>() = isSet;
			}
			
			return result;
		}
		
		template<typename TTraits, typename TStruct>
		static ParseResult WriteField(TStruct& obj,
		                              rapidjson::Value& jsonValue, 
		                              const char* jsonPtr,
		                              rapidjson::Document& document)
		{
			if constexpr (IsObjectDataField<TTraits>()) {
				return true;
			}

			std::string fieldPtr = MakeString{} << jsonPtr << "/" << TTraits::Name();
			ParseResult result = true;
			
			if (obj.template isSet<TTraits::Id()>()) {
				rapidjson::Value memberName(TTraits::Name(), document.GetAllocator());
				rapidjson::Value memberValue;
				
				result = Impl<typename TTraits::ValueType>::Write(
					obj.template value<TTraits::Id()>(), memberValue, fieldPtr.c_str(), document);
				
				if (result) {
					jsonValue.RemoveMember(memberName);
					jsonValue.AddMember(memberName.Move(), memberValue.Move(), document.GetAllocator());
				}
			} else if constexpr (!desc::Is_Optional<typename TTraits::DescriptorType>) {
				result = { false, fieldPtr };
			}
			
			return result;
		}
	};
	
	template<>
	class Parser::Impl<Uint64> {
	public:
		static ParseResult Read(Uint64& value, const rapidjson::Value* jsonValue, const char* jsonPtr)
		{
			std::array<uint32_t, 2> data{};
			auto result = Impl<std::array<uint32_t, 2>>::Read(data, jsonValue, jsonPtr);
			
			if (result) {
				value = data[0];
				value <<= 32;
				value |= data[1];
			}
			
			return result;
		}
		
		static ParseResult Write(const Uint64& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document)
		{
			std::array<uint32_t, 2> arr{};
			arr[0] = static_cast<uint32_t>(value >> 32);
			arr[1] = static_cast<uint32_t>(value);
			return Impl<std::array<uint32_t, 2>>::Write(arr, jsonValue, jsonPtr, document);
		}
	};
}}}
