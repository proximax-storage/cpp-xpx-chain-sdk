/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>

namespace xpx_chain_sdk {
	
	/// Network clock which starts from genesis block timestamp.
	class network_clock {
	public:
		using duration = std::chrono::duration<int64_t, std::milli>;
		using rep = duration::rep;
		using period = duration::period;
		using time_point = std::chrono::time_point<network_clock, duration>;
		static constexpr bool is_steady = false;
		
	public:
		/// Returns time point representing current network time.
		static time_point now();
		
		/// Converts network time point to Unix \c time_t.
		static std::time_t to_time_t(const time_point& tp);
		
		/// Converts Unix \c time_t to network time point.
		/// \note Created network time point can precede network epoch (i.e. has negative duration).
		///       This it is not considered an error because such time points can occur during
		///       computations while final result will be correct.
		static time_point from_time_t(std::time_t t);
		
		/// Converts network time point to system time point.
		static std::chrono::system_clock::time_point to_system(const time_point& tp);
		
		/// Converts system time point to network time point.
		/// \note Created network time point can precede network epoch (i.e. has negative duration).
		///       This it is not considered an error because such time points can occur during
		///       computations while final result will be correct.
		static time_point from_system(const std::chrono::system_clock::time_point& tp);
	};
	
	using NetworkDuration = network_clock::duration;
	using NetworkTimepoint = network_clock::time_point;
}
