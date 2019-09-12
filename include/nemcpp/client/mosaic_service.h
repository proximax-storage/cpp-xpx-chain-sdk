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
#include <nemcpp/client/mosaic_service.h>
#include <nemcpp/model/mosaic/multiple_mosaic.h>
#include <nemcpp/model/mosaic/mosaic_names.h>

namespace xpx_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;

    class MosaicService {
    public:
        MosaicService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
				std::shared_ptr<RequestParamsBuilder> builder
        );
        ~MosaicService() = default;
        MosaicInfo getMosaicInfo(const MosaicId& id);
        MultipleMosaicInfo getMosaicInfos(const std::vector<MosaicId>& ids);
        MosaicNames getMosaicsNames(const std::vector<MosaicId>& ids);
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
    };


};
