/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <map>
#include <optional>

namespace xpx_chain_sdk {

    class PaginationOrderingOptions {
    public:
        std::map<std::string, std::string> toMap() const {
            std::map<std::string, std::string> options;
            if (pageNumber.has_value()) {
                options.insert(std::pair<std::string, std::string>("pageNumber",std::to_string(pageNumber.value())));
            }

            if (pageSize.has_value()) {
                options.insert(std::pair<std::string, std::string>("pageSize",std::to_string(pageSize.value())));
            }

            if (offset.has_value()) {
                options.insert(std::pair<std::string, std::string>("offset",offset.value()));
            }

            if (sortDirection.has_value()) {
                options.insert(std::pair<std::string, std::string>("order",sortDirection.value()));
            }

            if (sortField.has_value()) {
                options.insert(std::pair<std::string, std::string>("sortField",sortField.value()));
            }
            return options;
        }

    public:
        std::optional<uint64_t> pageSize;
        std::optional<uint64_t> pageNumber;
        std::optional<std::string> offset;
        std::optional<std::string> sortField;
        std::optional<std::string> sortDirection;
    };
}