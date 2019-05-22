
#include <nemcpp/utils/network_time.h>
#include <nemcpp/config.h>

namespace nem2_sdk {
	
	network_clock::time_point network_clock::now()
	{
		return from_system(std::chrono::system_clock::now());
	}
	
	std::time_t network_clock::to_time_t(const time_point& tp)
	{
		return std::chrono::system_clock::to_time_t(to_system(tp));
	}
	
	network_clock::time_point network_clock::from_time_t(std::time_t t)
	{
		return from_system(std::chrono::system_clock::from_time_t(t));
	}
	
	std::chrono::system_clock::time_point network_clock::to_system(const time_point& tp)
	{
		return std::chrono::system_clock::time_point(
			tp.time_since_epoch() + GetConfig().NetworkEpoch.time_since_epoch());
	}
	
	network_clock::time_point network_clock::from_system(const std::chrono::system_clock::time_point& tp)
	{
		return time_point(std::chrono::duration_cast<duration>(
			tp.time_since_epoch() - GetConfig().NetworkEpoch.time_since_epoch()));
	}
}
