
#pragma once

#include <nemcpp/types.h>

#include <iterator>
#include <utility>

namespace xpx_sdk {
	
	template<typename TElement, template<typename, typename> class TConversionTrait> class Buffer;
	
	/// Allows implicit conversion only when source and destination types are the same.
	template<typename TSource, typename TDest>
	struct is_strictly_convertible: public std::false_type
	{ };
	
	template<typename TSource>
	struct is_strictly_convertible<TSource, TSource>: public std::true_type
	{ };
	
	template<typename TSource, typename TDest>
	constexpr bool is_strictly_convertible_v = is_strictly_convertible<TSource, TDest>::value;
	
	/// Allows implicit conversion from any type if destination is \c uint8_t.
	template<typename TSource, typename TDest>
	struct is_raw_convertible: public std::integral_constant<bool, is_strictly_convertible_v<TSource, TDest>>
	{ };
	
	template<typename TSource>
	struct is_raw_convertible<TSource, uint8_t>: public std::true_type
	{ };
	
	template<typename TSource, typename TDest>
	constexpr bool is_raw_convertible_v = is_raw_convertible<TSource, TDest>::value;
	
	/// Fixed-size buffer composed of data pointer and size.
	/// \tparam TElement Element type.
	/// \tparam TConversionTrait Trait that determines whether a type can be converted to \c TElement.
	template<typename TElement, template<typename, typename> class TConversionTrait = is_strictly_convertible>
	class Buffer {
	public:
		/// Elements type.
		using ElementType = TElement;
		
		/// Equals \c true if \c TSource is convertible to \c TDest.
		template<typename TSource, typename TDest>
		static constexpr bool is_convertible_v = TConversionTrait<TSource, TDest>::value;
		
	public:
		/// Creates an empty buffer.
		constexpr Buffer():
			data_(nullptr),
			size_(0)
		{ }
		
		/// Creates buffer holding \a size elements.
		template<typename TSomeElement>
		constexpr Buffer(TSomeElement* data, size_t size):
			data_(convertData(data)),
			size_(convertSize<TSomeElement>(size))
		{ }
		
		/// Creates buffer holding elements from \a first to \a last.
		template<typename TSomeElement>
		constexpr Buffer(TSomeElement* first, TSomeElement* last): Buffer(first, std::distance(first, last))
		{ }
		
		/// Creates buffer around single element.
		template<
			typename TSomeElement,
			typename = typename std::enable_if<std::is_arithmetic_v<TSomeElement> || std::is_enum_v<TSomeElement>>::type>
		constexpr Buffer(TSomeElement& element): Buffer(&element, 1)
		{ }
		
		/// Creates buffer around null-terminated \a str.
		template<
			typename TChar,
			typename = typename std::enable_if<std::is_const_v<TChar>>::type>
		constexpr Buffer(TChar* str): Buffer(str, std::char_traits<TChar>::length(str))
		{ }
		
		/// Creates buffer around any \a container that provides methods \c data and \c size.
		/// \note \c std::array provides \c constexpr \c data and \c size since C++17.
		template<
			typename TContainer,
			typename = typename std::enable_if<is_container_v<TContainer>>::type,
			typename = void>
		constexpr Buffer(TContainer& container): Buffer(container.data(), container.size())
		{ }
		
		/// Makes buffer point to the same data as \a rhs.ю.
		template<typename TSomeElement, template<typename, typename> class TSomeConversionTrait>
		Buffer& operator=(const Buffer<TSomeElement, TSomeConversionTrait>& rhs)
		{
			data_ = convertData(rhs.data());
			size_ = convertSize<TSomeElement>(rhs.size());
			return *this;
		}
		
		/// Returns pointer to buffer.
		ElementType* data()
		{ return data_; }
		
		/// Returns constant pointer to buffer.
		const ElementType* data() const
		{ return data_; }
		
		/// Return number of elements in buffer.
		constexpr size_t size() const
		{ return size_; }
		
		/// Return total size in bytes.
		constexpr size_t totalSize() const
		{ return size_ * sizeof(ElementType); }
		
		/// Returns \c true if buffer is empty.
		constexpr bool empty() const
		{ return size_ == 0; }
		
		/// Returns a reference to the element at specified position.
		ElementType& operator[](size_t pos)
		{ return data_[pos]; }
		
		/// Return a constant reference to the element at specified position.
		const ElementType& operator[](size_t pos) const
		{ return data_[pos]; }
		
		/// Returns an iterator to the first element.
		constexpr auto begin()
		{ return data_; }
		
		/// Returns a constant iterator to the first element.
		constexpr auto begin() const
		{ return static_cast<const ElementType*>(data_); }
		
		/// Returns a const iterator to the first element.
		constexpr auto cbegin() const
		{ return begin(); }
		
		/// Returns an iterator to the element following the last element.
		constexpr auto end()
		{ return data_ + size_; }
		
		/// Returns a constant iterator to the element following the last element.
		constexpr auto end() const
		{ return static_cast<const ElementType*>(data_ + size_); }
		
		/// Returns a constant iterator to the element following the last element.
		constexpr auto cend() const
		{ return end(); }
		
		/// Returns a reverse iterator to the first element of the reversed buffer.
		constexpr auto rbegin()
		{ return std::make_reverse_iterator(end()); }
		
		/// Returns a constant reverse iterator to the first element of the reversed buffer.
		constexpr auto rbegin() const
		{ return std::make_reverse_iterator(end()); }
		
		/// Returns a constant reverse iterator to the first element of the reversed buffer.
		constexpr auto crbegin() const
		{ return rbegin(); }
		
		/// Returns a reverse iterator to the element following the last element of the reversed buffer.
		constexpr auto rend()
		{ return std::make_reverse_iterator(begin()); }
		
		/// Returns a constant reverse iterator to the element following the last element of the reversed buffer.
		constexpr auto rend() const
		{ return std::make_reverse_iterator(begin()); }
		
		/// Returns a constant reverse iterator to the element following the last element of the reversed buffer.
		constexpr auto crend() const
		{ return rend(); }
		
	private:
		// Performs implicit conversion from another buffer pointer type.
		template<typename TSomeElement>
		constexpr ElementType* convertData(TSomeElement* data)
		{
			static_assert(is_convertible_v<std::remove_cv_t<TSomeElement>, std::remove_cv_t<ElementType>>,
			              "can't perform implicit conversion");
			
			if constexpr (std::is_same_v<ElementType, TSomeElement>) {
				// reinterpret_cast is not allowed in constexpr function, avoiding it if possible
				return data;
			} else {
				return reinterpret_cast<ElementType*>(data);
			}
		}
		
		// Performs implicit conversion from another buffer size.
		template<typename TSomeElement>
		constexpr size_t convertSize(size_t size)
		{
			static_assert(is_convertible_v<std::remove_cv_t<TSomeElement>, std::remove_cv_t<ElementType>>,
			              "can't perform implicit conversion");
			return size * (sizeof(TSomeElement) / sizeof(ElementType));
		}
		
	private:
		ElementType* data_;
		size_t size_;
	};
	
	/// Explicit cast for \c Buffer.
	/// \note Throws \c buffer_cast_error if buffers have incompatible sizes.
	template<
		typename TDest,
		typename TSourceElement, template<typename, typename> typename TSourceConversionTrait>
	TDest buffer_cast(Buffer<TSourceElement, TSourceConversionTrait> source)
	{
		using DestElementType = typename TDest::ElementType;
		
		if (source.totalSize() % sizeof(DestElementType)) {
			NEM2_SDK_THROW_2(buffer_cast_error, "failed to cast buffer, source size is not multiple of destination element size",
			                 source.totalSize(),
			                 sizeof(DestElementType));
		}
		
		return TDest(reinterpret_cast<DestElementType*>(source.data()),
		             source.totalSize() / sizeof(DestElementType));
	}
	
	/// Raw buffer of bytes that allows implicit conversion from any type.
	using RawBuffer = Buffer<const uint8_t, is_raw_convertible>;
	
	/// Mutabler raw buffer of bytes that allows implicit conversion from any type.
	using MutableRawBuffer = Buffer<uint8_t, is_raw_convertible>;
}
