/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <string_view>

namespace xpx_chain_sdk {
	
	/// Result code for read operation.
	enum class ReadResultCode: uint8_t {
		Success,
		Not_Enough_Data,
		Failure
	};
	
	/// Read result.
	class ReadResult {
	public:
		/// Creates result.
		ReadResult(ReadResultCode code, size_t consumed, std::string_view description = {});
		
		/// Returns read operation result code.
		ReadResultCode code() const;
		
		/// Returns result description.
		const std::string& description() const;
		
		/// Returns number of consumed by read operation bytes.
		size_t consumed() const;
		
		/// Returns \c true if object was read from serialized data.
		operator bool() const;
		
	private:
		ReadResultCode code_;
		std::string description_;
		size_t consumed_;
	};
}
