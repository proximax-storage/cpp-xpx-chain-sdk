/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk {

    class RateData {
    public:
        uint64_t currencyAmount;
        uint64_t mosaicAmount;
    };

    class TurnoverData {
    public:
        RateData rate;
        uint64_t turnover;
    };

    class LiquidityProviderData {
    public:
        uint64_t mosaicId;
        std::string providerKey;
        std::string owner;
        uint64_t additionallyMinted;
        std::string slashingAccount;
        uint32_t slashingPeriod;
        uint16_t windowSize;
        uint64_t creationHeight;
        uint32_t alpha;
        uint32_t beta;
        TurnoverData recentTurnover;
        std::vector<TurnoverData> turnoverHistory;
    };

	class LiquidityProvider {
	public:
        LiquidityProviderData data;
	};

	class MultipleLiquidityProviders {
	public:
		std::vector<LiquidityProvider> liquidityProviders;
	};
}