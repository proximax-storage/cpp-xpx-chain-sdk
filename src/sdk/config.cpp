/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/config.h>

namespace xpx_chain_sdk {
	
	Config& GetConfig()
	{
		static Config config;;
		return config;
	}
}
