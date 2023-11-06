/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/network_time.h>

#include <string>
#include <chrono>

namespace xpx_chain_sdk {
	
	constexpr NetworkIdentifier Network_Id = NetworkIdentifier::Mijin_Test;
	constexpr std::chrono::system_clock::time_point Network_Epoch(std::chrono::milliseconds(1459468800ll * 1000));
	
	constexpr uint32_t Block_Avg_Fee_Multiplier = 20;
	
	constexpr double Transaction_Fee_Multiplier = 1.0;
	constexpr NetworkDuration Transaction_Deadline = std::chrono::minutes(10);
	constexpr NetworkDuration Transaction_Delta = std::chrono::milliseconds(10 * 60 * 1000);

	
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

		NetworkDuration TransactionDelta = Transaction_Delta;

		/// Generation Hash of the first block in the network required for signing transactions.
		std::string GenerationHash = "7B631D803F912B00DC0CBED3014BBD17A302BA50B99D233B9C2D9533B842ABDF";

		std::string nodeAddress = "localhost";
		std::string port = "80";
		bool useSSL = false;
		std::string basePath = "/";
        std::string baseWsPath = "/ws";

        struct WebsocketOptions
        {
            uint64_t resolveHostTimeoutSecSec = 60;
            std::chrono::seconds handshakeTimeoutSec = std::chrono::seconds(60);
            std::chrono::seconds idleTimeoutSec = std::chrono::seconds(600);
            bool keepAlivePings = true;
        };

        WebsocketOptions wsOptions;
	};
	
	/// Returns SDK config.
	Config& GetConfig();
}
