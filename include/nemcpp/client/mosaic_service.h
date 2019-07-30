/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <vector>
#include <nemcpp/model/mosaic/mosaic.h>

namespace nem2_sdk {
    class IMosaicService {
    public:
        virtual MosaicInfo getMosaicInfo(const std::string& id) = 0;
        virtual std::vector<MosaicInfo> getMosaicInfos(const std::vector<std::string>& ids) = 0;
        virtual std::vector<std::string> getMosaicsNames(const std::vector<std::string>& ids) = 0;

        ~IMosaicService() = default;
    };
}