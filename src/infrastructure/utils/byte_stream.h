/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/utils/buffer.h>

#include <cassert>
#include <ios>
#include <type_traits>
#include <vector>

namespace xpx_chain_sdk { namespace internal {
	
	static constexpr size_t Stream_Npos = -1;
	template<int64_t N> struct skip {};
	
	// Read-only byte stream.
	class ReadOnlyByteStream {
	public:
		ReadOnlyByteStream(RawBuffer data, size_t getPosition = 0);
		
		// Methods for working with input position.
		size_t startg() const;
		size_t tellg() const;
		ReadOnlyByteStream& seekg(size_t position);
		ReadOnlyByteStream& seekg(int64_t offset, std::ios_base::seekdir direction);
		
		// Methods for accessing stream data and state.
		const RawBuffer& data() const;
		bool canRead() const;
		explicit operator bool() const;
		
		// Reads buffer.size() bytes from stream input position.
		ReadOnlyByteStream& operator>>(MutableRawBuffer buffer);
		
		// Skips N bytes in forward or backward direction from input position.
		template<int64_t N>
		ReadOnlyByteStream& operator>>(skip<N>)
		{
			seekg(N, std::ios_base::cur);
			return *this;
		}
		
	protected:
		virtual bool isValid() const;
		
		// Called from mutable descendant when stream data is modified.
		void updateData(RawBuffer newData);
		
	private:
		RawBuffer data_;
		size_t startGetPos_;
		size_t getPos_;
	};
	
	// Byte stream.
	class ByteStream: public ReadOnlyByteStream {
	public:
		ByteStream(std::vector<uint8_t>& data, size_t putPosition = Stream_Npos, size_t getPosition = 0);
		
		// Methods for working with output position.
		size_t startp() const;
		size_t tellp() const;
		ByteStream& seekp(size_t position);
		ByteStream& seekp(int64_t offset, std::ios_base::seekdir direction);
		
		// Added for consistency with 'canRead'.
		bool canWrite() const;
		
		// Writes buffer to stream output position.
		ByteStream& operator<<(RawBuffer buffer);
		
		// Overload for rvalues which are not supported by RawBuffer because it stores pointer to data.
		// Here we can guarantee that stored in RawBuffer pointer will not outlive rvalue and use the
		// trick, that inside this operator 'value' will be lvalue if it is not forwarded.
		template<
			typename T,
			typename = typename std::enable_if<!std::is_lvalue_reference_v<T>>::type>
		ByteStream& operator<<(T&& value)
		{
			return (*this) << value;
		}
		
	protected:
		bool isValid() const final;
		
	private:
		std::vector<uint8_t>& data_;
		size_t startPutPos_;
		size_t putPos_;
	};
}}
