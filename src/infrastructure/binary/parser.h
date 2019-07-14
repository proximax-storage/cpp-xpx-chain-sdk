
#pragma once

#include "infrastructure/binary/descriptors.h"
#include "infrastructure/utils/byte_stream.h"
#include "infrastructure/utils/variadic_struct.h"

#include <nemcpp/utils/hashable_array.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace nem2_sdk { namespace internal { namespace binary {
	
	// Binary DTOs should contain only following types:
	//
	// 1. other DTOs with null (void) descriptor and fields of types 1-5
	// 2. integral (int, short, etc) and enums with null (void) descriptor
	// 3. std::array or nem2_sdk::HashableArray with elements of type 1-3 and null (void) descriptor
	// 4. std::vector with elements of type 1-3 and VariableSize or TrailingSize descriptor used for vector size calculation
	// 5. std::string with VariableSize or TrailingSize descriptor used for string size calculation
	
	class ParseResult {
	public:
		ParseResult(bool result, size_t processedSize);
		
		size_t processedSize() const;
		operator bool() const;
		
	private:
		bool result_;
		size_t processedSize_;
	};
	
	class Parser {
	public:
		template<typename... TFields>
		static ParseResult Read(VariadicStruct<TFields...>& obj,
		                        RawBuffer data,
		                        size_t readPosition = 0)
		{
			using StructType = VariadicStruct<TFields...>;
			ReadOnlyByteStream bs(data, readPosition);
			
			if (!bs) {
				return { false, 0 };
			}
			
			bool result = ReadStruct(obj, bs, std::make_index_sequence<struct_size<StructType>::value>{});
			return { result, result ? bs.tellg() - bs.startg() : 0 };
		}
		
		template<typename... TFields>
		static ParseResult Write(const VariadicStruct<TFields...>& obj,
		                         std::vector<uint8_t>& data,
		                         size_t writePosition = Stream_Npos)
		{
			using StructType = VariadicStruct<TFields...>;
			ByteStream bs(data, writePosition);
			
			if (!bs) {
				return { false, 0 };
			}
			
			// Saving data that can be overwritten during invalid write.
			std::vector<uint8_t> guardedData(data.begin() + bs.startp(), data.end());
			
			bool result = WriteStruct(obj, bs, std::make_index_sequence<struct_size<StructType>::value>{});
			
			if (!result) {
				data.erase(data.begin() + bs.startp(), data.end());
				std::copy(guardedData.begin(), guardedData.end(), std::back_inserter(data));
			}
			
			return { result, result ?  bs.tellp() - bs.startp() : 0 };
		}
		
		template<typename... TFields>
		static size_t CalculateSize(const VariadicStruct<TFields...>& obj)
		{
			using StructType = VariadicStruct<TFields...>;
			return CalculateStructSize(obj, std::make_index_sequence<struct_size<StructType>::value>{});
		}
		
	private:
		template<typename TName, typename TStruct, typename TField, typename TDescriptor, bool IsItem = false>
		static constexpr void CheckField()
		{
			if constexpr (std::is_integral_v<TField> || std::is_enum_v<TField> || is_array_v<TField> || is_variadic_struct_v<TField>) {
				if constexpr (!std::is_same_v<TDescriptor, void>) {
					static_assert(sizeof(TField) == 0, "invalid field descriptor (should be void)");
				}

				if constexpr (is_array_v<TField>) {
					CheckField<TName, TStruct, typename TField::value_type, void, true>();
				}
			} else if constexpr (!IsItem && (is_specialization_v<TField, std::vector> || std::is_same_v<TField, std::string>)) {
				if constexpr (desc::is_variable_size_v<TDescriptor> || desc::is_trailing_size_v<TDescriptor>) {
					if constexpr (!has_field_by_name<typename TDescriptor::SizeFieldName, TStruct>::value) {
						static_assert(sizeof(TField) == 0, "unknown size field name");
					} else if constexpr (!std::is_integral_v<typename struct_field_by_name<typename TDescriptor::SizeFieldName, TStruct>::ValueType>) {
						static_assert(sizeof(TField) == 0, "invalid size field type (should be integral)");
					}
				} else {
					static_assert(sizeof(TField) == 0, "invalid container field descriptor (should be 'VariableSize' or 'TrailingSize')");
				}

				if constexpr (is_specialization_v<TField, std::vector>) {
					return CheckField<TName, TStruct, typename TField::value_type, void, true>();
				}
			} else {
				if constexpr (IsItem) {
					static_assert(sizeof(TField) == 0, "invalid container field item type");
				} else {
					static_assert(sizeof(TField) == 0, "invalid field type");
				}
			}
		}

		template<typename TStruct, size_t... Idx>
		static bool ReadStruct(TStruct& obj, ReadOnlyByteStream& bs, std::index_sequence<Idx...>)
		{
			(CheckField<typename struct_field_by_index<Idx, TStruct>::NameType,
			            TStruct,
			            typename struct_field_by_index<Idx, TStruct>::ValueType,
			            typename struct_field_by_index<Idx, TStruct>::DescriptorType>(), ...);

			auto result =
				(Impl<typename struct_field_by_index<Idx, TStruct>::ValueType>::template Read<typename struct_field_by_index<Idx, TStruct>::DescriptorType>(
				     obj, obj.template value<Field_Id<Idx, TStruct>>(), bs)
			     && ...);
			
			if (result) {
				(obj.template isSet<Field_Id<Idx, TStruct>>() = ... = true);
			}
			
			return result;
		}
		
		template<typename TStruct, size_t... Idx>
		static bool WriteStruct(const TStruct& obj, ByteStream& bs, std::index_sequence<Idx...>)
		{
			(CheckField<typename struct_field_by_index<Idx, TStruct>::NameType,
				TStruct,
				typename struct_field_by_index<Idx, TStruct>::ValueType,
				typename struct_field_by_index<Idx, TStruct>::DescriptorType>(), ...);

			bool isAllSet = (obj.template isSet<Field_Id<Idx, TStruct>>() && ...);
			
			if (!isAllSet) {
				return false;
			}
			
			return (Impl<typename struct_field_by_index<Idx, TStruct>::ValueType>::Write(obj.template value<Field_Id<Idx, TStruct>>(), bs) && ...);
		}
		
		template<typename TStruct, size_t... Idx>
		static size_t CalculateStructSize(const TStruct& obj, std::index_sequence<Idx...>)
		{
			(CheckField<typename struct_field_by_index<Idx, TStruct>::NameType,
				TStruct,
				typename struct_field_by_index<Idx, TStruct>::ValueType,
				typename struct_field_by_index<Idx, TStruct>::DescriptorType>(), ...);

			if constexpr (struct_size<TStruct>::value) {
				return (Impl<typename struct_field_by_index<Idx, TStruct>::ValueType>::CalculateSize(obj.template value<Field_Id<Idx, TStruct>>()) + ...);
			} else {
				return 0;
			}
		}
		
		template<typename TContainer>
		static void WriteContainer(const TContainer& c, ByteStream& bs)
		{
			if constexpr (std::is_class_v<typename TContainer::value_type>) {
				for (const auto& item: c) {
					Impl<typename TContainer::value_type>::Write(item, bs);
				}
			} else {
				bs << c;
			}
		}
		
		template<typename TContainer>
		static size_t CalculateContainerSize(const TContainer& c)
		{
			size_t size = 0;
			
			if constexpr (std::is_class_v<typename TContainer::value_type>) {
				for (const auto& item: c) {
					size += Impl<typename TContainer::value_type>::CalculateSize(item);
				}
			} else {
				size = sizeof(typename TContainer::value_type) * c.size();
			}
			
			return size;
		}
		
	private:
		template<typename TField>
		class Impl {
		public:
			template<typename TSizeDescriptor, typename TStruct>
			static bool Read(TStruct&, TField& field, ReadOnlyByteStream& bs)
			{
				if constexpr (std::is_integral_v<TField> || std::is_enum_v<TField>) {
					bs >> field;
				} else {
					static_assert(sizeof(TField) == 0, "field type is not supported by binary parser");
				}

				return static_cast<bool>(bs);			}
			
			template<typename TUnused = void> // without this some of 'CheckField' asserts can be skipped
			static bool Write(const TField& field, ByteStream& bs)
			{
				if constexpr (std::is_integral_v<TField> || std::is_enum_v<TField>) {
					bs << field;
				} else {
					static_assert(sizeof(TField) == 0, "field type is not supported by binary parser");
				}

				return true;
			}
			
			static size_t CalculateSize(const TField&)
			{
				return sizeof(TField);
			}
		};
	};
	
	// Implementation specializations for complex types.
	
	template<typename TItem, size_t N>
	class Parser::Impl<std::array<TItem, N>> {
	public:
		template<typename TSizeDescriptor, typename TStruct>
		static bool Read(TStruct& obj, std::array<TItem, N>& field, ReadOnlyByteStream& bs)
		{
			if constexpr (std::is_class_v<TItem>) {
				for (size_t i = 0; i < N && bs; ++i) {
					TItem item;
					Impl<TItem>::template Read<void>(obj, item, bs);
					field[i] = std::move(item);
				}
			} else {
				bs >> field;
			}
			
			return static_cast<bool>(bs);
		}
		
		static bool Write(const std::array<TItem, N>& field, ByteStream& bs)
		{
			WriteContainer(field, bs);
			return true;
		}
		
		static size_t CalculateSize(const std::array<TItem, N>& field)
		{
			return CalculateContainerSize(field);
		}
	};
	
	template<typename TItem, size_t N>
	class Parser::Impl<HashableArray<TItem, N>>: public Parser::Impl<std::array<TItem, N>> { };
	
	template<typename TItem>
	class Parser::Impl<std::vector<TItem>> {
	public:
		template<typename TSizeDescriptor, typename TStruct>
		static bool Read(TStruct& obj, std::vector<TItem>& field, ReadOnlyByteStream& bs)
		{
			field.clear();
			
			if constexpr (desc::is_variable_size_v<TSizeDescriptor>) {
				size_t size = TSizeDescriptor::GetSize(obj);
				
				if constexpr (std::is_class_v<TItem>) {
					for (size_t i = 0; i < size && bs; ++i) {
						TItem item;
						Impl<TItem>::template Read<void>(obj, item, bs);
						field.push_back(std::move(item));
					}
				} else {
					field.resize(size);
					bs >> field;
				}
			} else if constexpr (desc::is_trailing_size_v<TSizeDescriptor>) {
				size_t endPosition = TSizeDescriptor::GetTotalSize(obj) + bs.startg();
				
				while (bs && bs.tellg() < endPosition) {
					TItem item;
					Impl<TItem>::template Read<void>(obj, item, bs);
					field.push_back(item);
				}
			} else {
				static_assert(sizeof(TStruct) == 0, "binary parser unable to determine container field size");
			}
			
			return static_cast<bool>(bs);
		}
		
		static bool Write(const std::vector<TItem>& field, ByteStream& bs)
		{
			WriteContainer(field, bs);
			return true;
		}
		
		static size_t CalculateSize(const std::vector<TItem>& field)
		{
			return CalculateContainerSize(field);
		}
	};
	
	template<>
	class Parser::Impl<std::string>  {
	public:
		template<typename TSizeDescriptor, typename TStruct>
		static bool Read(TStruct& obj, std::string& field, ReadOnlyByteStream& bs)
		{
			if constexpr (desc::is_variable_size_v<TSizeDescriptor>) {
				size_t size = TSizeDescriptor::GetSize(obj);
				field.resize(size);
				bs >> field;
			} else if constexpr (desc::is_trailing_size_v<TSizeDescriptor>) {
				size_t endPosition = TSizeDescriptor::GetTotalSize(obj) + bs.startg();
				size_t size = endPosition > bs.tellg() ? endPosition - bs.tellg() : 0;
				field.resize(size);
				bs >> field;
			} else {
				static_assert(sizeof(TStruct) == 0, "binary parser unable to determine container field size");
			}
			
			return static_cast<bool>(bs);
		}
		
		static bool Write(const std::string& field, ByteStream& bs)
		{
			WriteContainer(field, bs);
			return true;
		}
		
		static size_t CalculateSize(const std::string& field)
		{
			return CalculateContainerSize(field);
		}
	};
	
	template<typename... TFields>
	class Parser::Impl<VariadicStruct<TFields...>> {
	public:
		using StructType = VariadicStruct<TFields...>;
		
	public:
		template<typename TFieldTraits, typename TParentStruct>
		static bool Read(TParentStruct& obj, StructType& field, ReadOnlyByteStream& bs)
		{
			ReadOnlyByteStream structBs(bs.data(), bs.tellg());
			
			bool result = Parser::ReadStruct(field, structBs, std::make_index_sequence<struct_size<StructType>::value>{});
			
			if (result) {
				bs.seekg(structBs.tellg());
			} else {
				bs.seekg(Stream_Npos); // invalidating parent stream
			}
			
			return result;
		}
		
		static bool Write(const StructType& field, ByteStream& bs)
		{
			return Parser::WriteStruct(field, bs, std::make_index_sequence<struct_size<StructType>::value>{});
		}
		
		static size_t CalculateSize(const StructType& obj)
		{
			return Parser::CalculateStructSize(obj, std::make_index_sequence<struct_size<StructType>::value>{});
		}
	};
}}}
