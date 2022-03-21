/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk {

    class CumulativePayment {
    public:
        std::string replicator;
        uint64_t payment;
    };

    class DownloadChannelData {
    public:
        std::string id;
        std::string consumer;
        std::string drive;
        uint64_t downloadSize;
        uint16_t downloadApprovalCountLeft;
        std::vector<std::string> listOfPublicKeys;
        std::vector<std::string> shardReplicators;
        std::vector<CumulativePayment> cumulativePayments;
    };

	class DownloadChannel {
	public:
        DownloadChannelData data;
	};

	class MultipleDownloadChannels {
	public:
		std::vector<DownloadChannel> channels;
	};
}