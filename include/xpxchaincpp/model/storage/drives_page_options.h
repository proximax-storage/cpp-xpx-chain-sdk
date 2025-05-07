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

    class DrivesPageOptions {
    public:
        std::map<std::string, std::string> toMap() const {
            std::map<std::string, std::string> options;
            if (size.has_value()) {
                options.insert(std::pair<std::string, std::string>("size",std::to_string(size.value())));
            }

            if (fromSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromSize",std::to_string(fromSize.value())));
            }

            if (toSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("toSize",std::to_string(toSize.value())));
            }

            if (usedSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("usedSize",std::to_string(usedSize.value())));
            }

            if (fromUsedSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromUsedSize",std::to_string(fromUsedSize.value())));
            }

            if (toUsedSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("toUsedSize",std::to_string(toUsedSize.value())));
            }

            if (metaFilesSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("metaFilesSize",std::to_string(metaFilesSize.value())));
            }

            if (fromMetaFilesSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromMetaFilesSize",std::to_string(fromMetaFilesSize.value())));
            }

            if (toMetaFilesSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("toMetaFilesSize",std::to_string(toMetaFilesSize.value())));
            }

            if (replicatorCount.has_value()) {
                options.insert(std::pair<std::string, std::string>("replicatorCount",std::to_string(replicatorCount.value())));
            }

            if (fromReplicatorCount.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromReplicatorCount",std::to_string(fromReplicatorCount.value())));
            }

            if (toReplicatorCount.has_value()) {
                options.insert(std::pair<std::string, std::string>("toReplicatorCount",std::to_string(toReplicatorCount.value())));
            }

            if (owner.has_value()) {
                options.insert(std::pair<std::string, std::string>("owner",owner.value()));
            }

            if (paginationOrderingOptions.has_value()) {
                auto poOptions = paginationOrderingOptions.value().toMap();
                options.insert(poOptions.begin(), poOptions.end());
            }

            return options;
        }

    public:
        std::optional<uint64_t> size;
        std::optional<uint64_t> fromSize;
        std::optional<uint64_t> toSize;
        std::optional<uint64_t> usedSize;
        std::optional<uint64_t> fromUsedSize;
        std::optional<uint64_t> toUsedSize;
        std::optional<uint64_t> metaFilesSize;
        std::optional<uint64_t> fromMetaFilesSize;
        std::optional<uint64_t> toMetaFilesSize;
        std::optional<uint64_t> replicatorCount;
        std::optional<uint64_t> fromReplicatorCount;
        std::optional<uint64_t> toReplicatorCount;
        std::optional<std::string> owner;
        std::optional<PaginationOrderingOptions> paginationOrderingOptions;
    };
}