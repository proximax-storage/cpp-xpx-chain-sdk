
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
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace nem2_sdk { namespace internal { namespace json {
	
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
			
			rapidjson::Value& jsonValue = CreateValueByPointer(document_, ptr);
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
					using Type = std::underlying_type_t<TField>;
					Type rawValue = Type{};

					auto result = Impl<Type>::Read(rawValue, jsonValue, jsonPtr);

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
					static_assert(sizeof(TField) == 0, "type is not supported by json parser");
				}
				
				return { false, jsonPtr };
			}
			
			static ParseResult Write(const TField& value,
			                         rapidjson::Value& jsonValue,
			                         const char* jsonPtr,
			                         rapidjson::Document& document)
			{
				if constexpr (std::is_enum_v<TField>) {
					using Type = std::underlying_type_t<TField>;
					return Impl<Type>::Write(static_cast<Type>(value), jsonValue, jsonPtr, document);
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
					static_assert(sizeof(TField) == 0, "type is not supported by json parser");
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
				TItem item;
				
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
			auto result = Impl<decltype(data)>::Read(data, jsonValue, jsonPtr);
			
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
			return Impl<decltype(data)>::Write(data, jsonValue, jsonPtr, document);
		}
	};
	
	template<typename TItem, size_t N>
	class Parser::Impl<HashableArray<TItem, N>>: public Parser::Impl<std::array<TItem, N>> { };
	
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
		template<size_t... Idx>
		static ParseResult Read(StructType& value,
		                        const rapidjson::Value* jsonValue,
		                        const char* jsonPtr,
		                        std::index_sequence<Idx...>)
		{
			ParseResult result = true;
			
			( ( result = ReadField<struct_field_by_index<Idx, StructType>>(value, jsonValue, jsonPtr),
			    result ) && ... );
			
			return result;
		}
		
		template<size_t... Idx>
		static ParseResult Write(const StructType& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document,
		                         std::index_sequence<Idx...>)
		{
			ParseResult result = true;
			
			( ( result = WriteField<struct_field_by_index<Idx, StructType>>(value, jsonValue, jsonPtr, document),
			    result ) && ... );
			
			return result;
		}
		
		template<typename TTraits, typename TStruct>
		static ParseResult ReadField(TStruct& obj, const rapidjson::Value* jsonValue, const char* jsonPtr)
		{
			std::string fieldPtr = MakeString{} << jsonPtr << "/" << TTraits::Name();
			auto it = jsonValue->FindMember(TTraits::Name());
			
			auto result = Impl<typename TTraits::ValueType>::Read(
				obj.template value<TTraits::Id()>(),
				it != jsonValue->MemberEnd() ? &(it->value) : nullptr,
				fieldPtr.c_str());
			
			obj.template isSet<TTraits::Id()>() =
				it != jsonValue->MemberEnd() && !(it->value.IsNull()) ? result : ParseResult(false);
			
			if (result) {
				bool isSet = it != jsonValue->MemberEnd() && !(it->value.IsNull());
				
				if constexpr (!std::is_same_v<typename TTraits::DescriptorType, desc::Optional>) {
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
			std::string fieldPtr = MakeString{} << jsonPtr << "/" << TTraits::Name();
			ParseResult result = true;
			
			if (obj.template isSet<TTraits::Id()>()) {
				rapidjson::Value memberName(TTraits::Name(), document.GetAllocator());
				rapidjson::Value memberValue;
				
				result = Impl<typename TTraits::ValueType>::Write(
					obj.template value<TTraits::Id()>(), memberValue, fieldPtr.c_str(), document);
				
				if (result) {
					jsonValue.AddMember(memberName.Move(), memberValue.Move(), document.GetAllocator());
				}
			} else if constexpr (!std::is_same_v<typename TTraits::DescriptorType, desc::Optional>) {
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
			std::array<uint32_t, 2> data;
			auto result = Impl<decltype(data)>::Read(data, jsonValue, jsonPtr);

			//changed order
			if (result) {
				value = data[1];
				value <<= 32;
				value |= data[0];
			}
			
			return result;
		}
		
		static ParseResult Write(const Uint64& value,
		                         rapidjson::Value& jsonValue,
		                         const char* jsonPtr,
		                         rapidjson::Document& document)
		{
			std::array<uint32_t, 2> arr;
			arr[0] = static_cast<uint32_t>(value >> 32);
			arr[1] = static_cast<uint32_t>(value);
			return Impl<decltype(arr)>::Write(arr, jsonValue, jsonPtr, document);
		}
	};
}}}
