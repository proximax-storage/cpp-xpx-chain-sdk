/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>
#include "transaction.h"

namespace xpx_chain_sdk::transactions_page {

    class TransactionMeta {
    public:
        uint64_t height;
        std::string hash;
        std::string merkleComponentHash;
        uint64_t index;
    };

    class Transaction {
    public:
        TransactionMeta meta;
        transactions_info::Transaction data;
        std::string id;
    };

	class Pagination {
	public:
        uint64_t totalEntries;
        uint64_t pageNumber;
        uint64_t pageSize;
        uint64_t totalPages;
	};

	class TransactionsPage {
	public:
        std::vector<Transaction> transactions;
        Pagination pagination;
	};
}