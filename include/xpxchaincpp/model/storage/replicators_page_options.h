/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <map>
#include <optional>
#include <vector>
#include "xpxchaincpp/utils/paginationOrderingOptions.h"

namespace xpx_chain_sdk {

    class ReplicatorsPageOptions {
    public:
        std::map<std::string, std::string> toMap() const {
            std::map<std::string, std::string> options;
            if (version.has_value()) {
                options.insert(std::pair<std::string, std::string>("version",std::to_string(version.value())));
            }

            if (fromVersion.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromVersion",std::to_string(fromVersion.value())));
            }

            if (toVersion.has_value()) {
                options.insert(std::pair<std::string, std::string>("toVersion",std::to_string(toVersion.value())));
            }

            if (capacity.has_value()) {
                options.insert(std::pair<std::string, std::string>("capacity",std::to_string(capacity.value())));
            }

            if (fromCapacity.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromCapacity",std::to_string(fromCapacity.value())));
            }

            if (toCapacity.has_value()) {
                options.insert(std::pair<std::string, std::string>("toCapacity",std::to_string(toCapacity.value())));
            }

            if (paginationOrderingOptions.has_value()) {
                auto poOptions = paginationOrderingOptions.value().toMap();
                options.insert(poOptions.begin(), poOptions.end());
            }

            return options;
        }

    public:
        std::optional<uint64_t> version;
        std::optional<uint64_t> fromVersion;
        std::optional<uint64_t> toVersion;
        std::optional<uint64_t> capacity;
        std::optional<uint64_t> fromCapacity;
        std::optional<uint64_t> toCapacity;
        std::optional<PaginationOrderingOptions> paginationOrderingOptions;
    };
}