/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/comparator.h>
#include <xpxchaincpp/model/mosaic/mosaic_property.h>
#include <set>
#include <vector>

namespace xpx_chain_sdk {

    /// Mosaic data.
    class Mosaic {
    public:
        Mosaic() = default;
        Mosaic(uint64_t _id, uint64_t _amount) :id(_id) , amount(_amount) {}
        ~Mosaic() = default;

    public:
        /// Mosaic id.
        uint64_t id;

        /// Mosaic amount.
        uint64_t amount;

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



    class MosaicMeta {
    public:
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
