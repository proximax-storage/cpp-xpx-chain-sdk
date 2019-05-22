
#include <nemcpp/config.h>

namespace nem2_sdk {
	
	Config& GetConfig()
	{
		static Config config;;
		return config;
	}
}
