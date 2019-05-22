
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/utils/network_time.h>

#include <chrono>

namespace nem2_sdk {
	
	constexpr NetworkIdentifier Network_Id = NetworkIdentifier::Public;
	constexpr std::chrono::system_clock::time_point Network_Epoch(std::chrono::seconds(1459468800));
	
	constexpr uint32_t Block_Avg_Fee_Multiplier = 20;
	
	constexpr double Transaction_Fee_Multiplier = 1.0;
	constexpr NetworkDuration Transaction_Deadline = std::chrono::hours(1);
	
	/// SDK configuration.
	struct Config {
		/// Determines network for which transactions are created by default.
		NetworkIdentifier NetworkId = Network_Id;
		
		/// Determines network epoch in system clocks.
		std::chrono::system_clock::time_point NetworkEpoch = Network_Epoch;
		
		/// Determines average block fee multiplier for network.
		/// \note Block fee multiplier is used by harvesting nodes to determine minimum transaction
		///       fee for transactions they accept. SDK should be configured with approximate value
		///       of this parameter in order to provide some good-enough (not too low or too high)
		///       default value for created transactions fees.
		uint32_t BlockAvgFeeMultiplier = Block_Avg_Fee_Multiplier;
		
		/// Determines transaction fee multiplier for default fee calculation.
		/// \note Transaction fee multilpier is applied to default good-enough transaction fee (when
		///       it is not set explicitly). If set to value greater then 1, transaction will be confirmed
		///       faster (harvesting nodes tend to include in blocks transactions with higher fees first).
		///       If set to value less then 1 - transaction's confirmation time will increase. If set to
		///       value close to 0, transaction can even be totally ignored by network.
		double TransactionFeeMultiplier = Transaction_Fee_Multiplier;
		
		/// Determines transaction deadline which is used by default.
		NetworkDuration TransactionDeadline = Transaction_Deadline;
	};
	
	/// Returns SDK config.
	Config& GetConfig();
}
