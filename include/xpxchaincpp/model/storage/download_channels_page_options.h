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

    class DownloadChannelsPageOptions {
    public:
        std::map<std::string, std::string> toMap() const {
            std::map<std::string, std::string> options;
            if (downloadSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("downloadSize",std::to_string(downloadSize.value())));
            }

            if (fromDownloadSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromDownloadSize",std::to_string(fromDownloadSize.value())));
            }

            if (toDownloadSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("toDownloadSize",std::to_string(toDownloadSize.value())));
            }

            if (downloadApprovalCount.has_value()) {
                options.insert(std::pair<std::string, std::string>("downloadApprovalCount",std::to_string(downloadApprovalCount.value())));
            }

            if (fromDownloadApprovalCount.has_value()) {
                options.insert(std::pair<std::string, std::string>("fromDownloadApprovalCount",std::to_string(fromDownloadApprovalCount.value())));
            }

            if (toDownloadApprovalCount.has_value()) {
                options.insert(std::pair<std::string, std::string>("toDownloadApprovalCount",std::to_string(toDownloadApprovalCount.value())));
            }

            if (consumerKey.has_value()) {
                options.insert(std::pair<std::string, std::string>("consumerKey",consumerKey.value()));
            }

            if (paginationOrderingOptions.has_value()) {
                auto poOptions = paginationOrderingOptions.value().toMap();
                options.insert(poOptions.begin(), poOptions.end());
            }

            return options;
        }

    public:
        std::optional<uint64_t> downloadSize;
        std::optional<uint64_t> fromDownloadSize;
        std::optional<uint64_t> toDownloadSize;
        std::optional<uint64_t> downloadApprovalCount;
        std::optional<uint16_t> fromDownloadApprovalCount;
        std::optional<uint16_t> toDownloadApprovalCount;
        std::optional<std::string> consumerKey;
        std::optional<PaginationOrderingOptions> paginationOrderingOptions;
    };
}