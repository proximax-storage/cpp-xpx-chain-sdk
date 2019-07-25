//
// Created by vulfke on 23.07.19.
//

#include <vector>
#include <nemcpp/model/mosaic/mosaic.h>

namespace nem2_sdk {
    class IMosaicService {
    public:
        virtual MosaicInfo getMosaicInfo(MosaicId id) = 0;

        virtual std::vector<MosaicInfo> getMosaicInfos(const std::vector<MosaicId>& ids) = 0;

        virtual std::vector<std::string> getMosaicsNames(const std::vector<MosaicId>& ids) = 0;

        ~IMosaicService() = default;
    };
}