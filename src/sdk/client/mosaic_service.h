#pragma once
//
// Created by vulfke on 22.07.19.
//

//#ifndef NEM2_SDK_CPP_MOSAIC_INFO_H
//#define NEM2_SDK_CPP_MOSAIC_INFO_H
//
//#endif //NEM2_SDK_CPP_MOSAIC_INFO_H

#include <nemcpp/types.h>
#include <nemcpp/model/account/public_account.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <memory>
#include <infrastructure/network/context.h>
#include <infrastructure/network/http.h>
#include <nemcpp/client/mosaic_service.h>

namespace nem2_sdk {

    class MosaicService : IMosaicService {
    public:
        MosaicService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
                internal::network::RequestParamsBuilder builder
        );
        MosaicDataDto getMosaicInfo(MosaicId id) override;
        std::vector<MosaicDataDto> getMosaicInfos(std::vector<MosaicId> ids) override;
        std::vector<std::string> getMosaicsNames(std::vector<MosaicId> ids) override;
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
        internal::network::RequestParamsBuilder _builder;
    };


};
