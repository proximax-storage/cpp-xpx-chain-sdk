
#include <xpxchaincpp/config.h>

namespace xpx_chain_sdk {
	
	Config& GetConfig()
	{
		static Config config;;
		return config;
	}
}
