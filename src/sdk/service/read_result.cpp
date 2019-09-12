/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/service/read_result.h>

namespace xpx_chain_sdk {
	
	ReadResult::ReadResult(ReadResultCode code, size_t consumed, std::string_view description):
		code_(code),
		description_(description),
		consumed_(consumed)
	{ }
	
	ReadResultCode ReadResult::code() const
	{
		return code_;
	}
	
	const std::string& ReadResult::description() const
	{
		return description_;
	}
	
	size_t ReadResult::consumed() const
	{
		return consumed_;
	}
	
	ReadResult::operator bool() const
	{
		return code_ == ReadResultCode::Success;
	}
}
