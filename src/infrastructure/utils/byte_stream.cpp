/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "infrastructure/utils/byte_stream.h"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <utility>

namespace xpx_chain_sdk { namespace internal {
	
	namespace {
		template<typename T>
		void SetPosition(size_t& position, size_t dataSize, T value, std::ios_base::seekdir direction = std::ios_base::beg)
		{
			size_t startPos = 0;
			
			switch (direction) {
			case std::ios_base::beg: startPos = 0;                  break;
			case std::ios_base::end: startPos = dataSize;           break;
			case std::ios_base::cur: startPos = position;           break;
			default: assert(!"unknown byte stream seek direction"); break;
			}
			
			value += startPos;
			
			if (value >= 0 && static_cast<size_t>(value) <= dataSize) {
				position = value;
			} else {
				position = Stream_Npos;
			}
		}
	}
	
	ReadOnlyByteStream::ReadOnlyByteStream(RawBuffer data, size_t getPosition):
		data_(data),
		startGetPos_(getPosition != Stream_Npos ? getPosition : data.size()),
		getPos_(0)
	{
		SetPosition(getPos_, data_.size(), startGetPos_);
	}
	
	size_t ReadOnlyByteStream::startg() const
	{
		return startGetPos_;
	}
	
	size_t ReadOnlyByteStream::tellg() const
	{
		return getPos_;
	}
	
	ReadOnlyByteStream& ReadOnlyByteStream::seekg(size_t position)
	{
		if (isValid()) {
			SetPosition(getPos_, data_.size(), position);
		}
		
		return *this;
	}
	
	ReadOnlyByteStream& ReadOnlyByteStream::seekg(int64_t offset, std::ios_base::seekdir direction)
	{
		if (isValid()) {
			SetPosition(getPos_, data_.size(), offset, direction);
		}
		
		return *this;
	}
	
	const RawBuffer& ReadOnlyByteStream::data() const
	{
		return data_;
	}
	
	bool ReadOnlyByteStream::canRead() const
	{
		return isValid() && getPos_ < data_.size();
	}
	
	ReadOnlyByteStream::operator bool() const
	{
		return isValid();
	}
	
	ReadOnlyByteStream& ReadOnlyByteStream::operator>>(MutableRawBuffer buffer)
	{
		if (isValid()) {
			auto start = data_.begin() + getPos_;
			
			if (buffer.size() <= static_cast<size_t>(std::distance(start, data_.end()))) {
				std::copy(start, start + buffer.size(), buffer.data());
				getPos_ += buffer.size();
			} else {
				std::memset(buffer.data(), 0, buffer.size());
				getPos_ = Stream_Npos;
			}
		} else {
			std::memset(buffer.data(), 0, buffer.size());
		}
		
		return *this;
	}
	
	bool ReadOnlyByteStream::isValid() const
	{
		return getPos_ != Stream_Npos;
	}
	
	void ReadOnlyByteStream::updateData(RawBuffer newData)
	{
		data_ = newData;
	}
	
	ByteStream::ByteStream(std::vector<uint8_t>& data, size_t putPosition, size_t getPosition):
		ReadOnlyByteStream(data, getPosition),
		data_(data),
		startPutPos_(putPosition != Stream_Npos ? putPosition : data_.size()),
		putPos_(0)
	{
		SetPosition(putPos_, data_.size(), startPutPos_);
	}
	
	size_t ByteStream::startp() const
	{
		return startPutPos_;
	}
	
	size_t ByteStream::tellp() const
	{
		return putPos_;
	}
	
	ByteStream& ByteStream::seekp(size_t position)
	{
		if (isValid()) {
			SetPosition(putPos_, data_.size(), position);
		}
		
		return *this;
	}
	
	ByteStream& ByteStream::seekp(int64_t offset, std::ios_base::seekdir direction)
	{
		if (isValid()) {
			SetPosition(putPos_, data_.size(), offset, direction);
		}
		
		return *this;
	}
	
	bool ByteStream::canWrite() const
	{
		return isValid();
	}
	
	ByteStream& ByteStream::operator<<(RawBuffer buffer)
	{
		if (isValid()) {
			auto remainingSize = static_cast<size_t>(std::distance(data_.begin() + putPos_, data_.end()));
			
			if (buffer.size() > remainingSize) {
				data_.resize(data_.size() + (buffer.size() - remainingSize));
				updateData(data_);
			}
			
			std::copy(buffer.begin(), buffer.end(), data_.begin() + putPos_);
			putPos_ += buffer.size();
		}
		
		return *this;
	}
	
	bool ByteStream::isValid() const
	{
		return ReadOnlyByteStream::isValid() && putPos_ != Stream_Npos;
	}
}}
