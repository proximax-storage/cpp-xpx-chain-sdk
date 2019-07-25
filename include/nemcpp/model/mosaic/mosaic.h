
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/utils/comparator.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <set>

namespace nem2_sdk {

    /// Mosaic data.
    struct Mosaic {
        /// Mosaic id.
        MosaicId id;

        /// Mosaic amount.
        Amount amount;

        /// Mosaic height
        //        Height height;

        /// Generates mosaic id using \a owner key and \a nonce
        static MosaicId GenerateId(const Key& owner, uint32_t nonce);
    };

    /// Returns \c true if \a lhs and \a rhs are equal.
    bool operator==(const Mosaic& lhs, const Mosaic& rhs);

    /// Returns \c true if \a lhs and \a rhs are not equal.
    bool operator!=(const Mosaic& lhs, const Mosaic& rhs);

    /// Mosaics container.
    using MosaicContainer = std::set<Mosaic, data_comparator<Mosaic, MosaicId, &Mosaic::id>>;

    class MosaicName {
    public:
        uint64_t parentId;
        uint64_t mosaicId;
        std::string name;
    };

    class MosaicMeta {
    public:
        bool active;
        uint64_t index;
        std::string id;
    };

    class MosaicData {
    public:
        uint64_t mosaicId;
        uint64_t amount;
        uint64_t height;
        std::string owner;
        std::vector<MosaicProperty> properties;

    };

    class MosaicInfo {
    public:
        MosaicMeta meta;
        MosaicData data;
    };
}
