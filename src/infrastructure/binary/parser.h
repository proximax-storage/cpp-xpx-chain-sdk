
#pragma once

#include "infrastructure/binary/descriptors.h"
#include "infrastructure/utils/byte_stream.h"
#include "infrastructure/utils/variadic_struct.h"

#include <xpxchaincpp/utils/hashable_array.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace xpx_chain_sdk { namespace internal { namespace binary {
	
	// Binary DTOs should contain only following types:
	//
	// 1. arithmetic (int, double, etc) and enums
	// 2. std::array or xpx_chain_sdk::HashableArray which have fixed size
	// 3. std::string which requires size descriptor and treats it as string length
	// 4. std::vector which requires size descriptor and treats it as elements count
	// 5. other binary DTOs
	
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
		template<typename TStruct, size_t... Idx>
		static bool ReadStruct(TStruct& obj, ReadOnlyByteStream& bs, std::index_sequence<Idx...>)
		{
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
			bool isAllSet = (obj.template isSet<Field_Id<Idx, TStruct>>() && ...);
			
			if (!isAllSet) {
				return false;
			}
			
			return (Impl<typename struct_field_by_index<Idx, TStruct>::ValueType>::Write(obj.template value<Field_Id<Idx, TStruct>>(), bs) && ...);
		}
		
		template<typename TStruct, size_t... Idx>
		static size_t CalculateStructSize(const TStruct& obj, std::index_sequence<Idx...>)
		{
			if constexpr (struct_size<TStruct>::value != 0) {
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
				bs >> field;
				return static_cast<bool>(bs);
			}
			
			static bool Write(const TField& field, ByteStream& bs)
			{
				bs << field;
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
				static_assert(sizeof(TSizeDescriptor) == 0, "invalid binary field size descriptor");
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
				static_assert(sizeof(TSizeDescriptor) == 0, "invalid binary field size descriptor");
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
