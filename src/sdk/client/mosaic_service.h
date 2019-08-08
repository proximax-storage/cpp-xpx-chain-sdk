/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <nemcpp/types.h>
#include <nemcpp/model/account/public_account.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <memory>
#include <infrastructure/network/context.h>
#include <infrastructure/network/http.h>
#include <nemcpp/client/mosaic_service.h>

namespace xpx_sdk {

    class MosaicService : public IMosaicService {
    public:
        MosaicService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
                internal::network::RequestParamsBuilder builder
        );
        MosaicInfo getMosaicInfo(const MosaicId& id) override;
        MultipleMosaicInfo getMosaicInfos(const std::vector<MosaicId>& ids) override;
        MosaicNames getMosaicsNames(const std::vector<MosaicId>& ids) override;
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
        internal::network::RequestParamsBuilder _builder;
    };


};
