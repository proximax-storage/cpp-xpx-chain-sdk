
#include <nemcpp/config.h>

namespace xpx_sdk {
	
	Config& GetConfig()
	{
		static Config config;;
		return config;
	}
}
