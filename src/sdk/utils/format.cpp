/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/utils/format.h>

namespace xpx_chain_sdk {
	
	std::string MakeString::str() const
	{
		return stream_.str();
	}

	MakeString::operator std::string() const
	{
		return str();
	}
}
