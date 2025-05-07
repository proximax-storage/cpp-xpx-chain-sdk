/*
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/config.h>
#include <memory>
#include <xpxchaincpp/model/supercontract_v2/supercontract.h>

namespace xpx_chain_sdk::internal::network {
    class Context;
    class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

    using internal::network::Context;
    using internal::network::RequestParamsBuilder;

    class SuperContractV2Service {
    public:
        SuperContractV2Service(
                const Config& config,
                std::shared_ptr<internal::network::Context> context);

        ~SuperContractV2Service() = default;

        SuperContractV2 getSuperContractByKey(const std::string& contractKey);

    private:
        const Config& _config;
        std::shared_ptr<internal::network::Context> _context;
    };
};
