//
// Created by vulfke on 23.07.19.
//

#include <vector>
#include <infrastructure/dto/mosaic_dto.h>

namespace nem2_sdk {
    using internal::dto::MosaicDataDto;
    class IMosaicService {
    public:
        virtual MosaicDataDto getMosaicInfo(MosaicId id) = 0;

        virtual std::vector<MosaicDataDto> getMosaicInfos(const std::vector<MosaicId>& ids) = 0;

        virtual std::vector<std::string> getMosaicsNames(const std::vector<MosaicId>& ids) = 0;

        ~IMosaicService() = default;
    };
}