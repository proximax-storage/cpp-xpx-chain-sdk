/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/
#pragma once


#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/mosaic/multiple_mosaic.h>
#include <nemcpp/model/mosaic/mosaic_names.h>

#include <vector>

namespace xpx_sdk {
    class IMosaicService {
    public:
        virtual MosaicInfo getMosaicInfo(const std::string& id) = 0;
        virtual MultipleMosaicInfo getMosaicInfos(const std::vector<std::string>& ids) = 0;
        virtual MosaicNames getMosaicsNames(const std::vector<std::string>& ids) = 0;

        virtual ~IMosaicService() = default;
    };
}