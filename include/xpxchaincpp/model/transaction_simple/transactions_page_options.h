/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <map>
#include <optional>
#include "xpxchaincpp/utils/paginationOrderingOptions.h"

namespace xpx_chain_sdk {

    enum TransactionGroup {
        Confirmed,
        Unconfirmed,
        Partial
    };
	
	class TransactionsPageOptions {
    public:
	    std::map<std::string, std::string> toMap() const {
            std::map<std::string, std::string> options;
            if (height.has_value()) {
                options.insert(std::pair<std::string, std::string>("Height",std::to_string(height.value())));
            }

            if (fromHeight.has_value()) {
                options.insert(std::pair<std::string, std::string>("FromHeight",std::to_string(fromHeight.value())));
            }

            if (toHeight.has_value()) {
                options.insert(std::pair<std::string, std::string>("ToHeight",std::to_string(toHeight.value())));
            }

            if (address.has_value()) {
                options.insert(std::pair<std::string, std::string>("Address",address.value()));
            }

            if (signerPublicKey.has_value()) {
                options.insert(std::pair<std::string, std::string>("SignerPublicKey",signerPublicKey.value()));
            }

            if (recipientAddress.has_value()) {
                options.insert(std::pair<std::string, std::string>("RecipientAddress",recipientAddress.value()));
            }

            if (type.has_value()) {
                std::string result;
                const int size = static_cast<int>(type.value().size());
                for (int i = 0; i < size; i++) {
                    if (i == 0) {
                        result += "=" + std::to_string(type.value()[i]);
                    } else {
                        result += "&Type=" + std::to_string(type.value()[i]);
                    }
                }

                options.insert(std::pair<std::string, std::string>("Type", result));
            }

            if (embedded.has_value()) {
                options.insert(std::pair<std::string, std::string>("Embedded",embedded.value() ? "true" : "false"));
            }

            if (paginationOrderingOptions.has_value()) {
                auto poOptions = paginationOrderingOptions.value().toMap();
                options.insert(poOptions.begin(), poOptions.end());
            }

	        return options;
	    }

    public:
        std::optional<unsigned int> height;
        std::optional<uint64_t> fromHeight;
        std::optional<uint64_t> toHeight;
        std::optional<std::string> address;
        std::optional<std::string> signerPublicKey;
        std::optional<std::string> recipientAddress;
        std::optional<std::vector<unsigned int>> type;
        std::optional<bool> embedded;
        std::optional<PaginationOrderingOptions> paginationOrderingOptions;
	};
}
