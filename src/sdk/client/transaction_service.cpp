/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <infrastructure/network/http.h>
#include "xpxchaincpp/client/transaction_service.h"
#include <infrastructure/utils/deserialization_json.h>
#include <xpxchaincpp/utils/bytes_to_string.h>
#include <iostream>


namespace xpx_chain_sdk {

	using internal::json::dto::from_json;
	using internal::json::dto::transaction_from_json;
    using internal::json::dto::transactions_from_json;
	using internal::json::dto::TransactionContainerDto;
	using ParserJson = internal::json::Parser;
	using transactions_info::TransactionContainer;
	using internal::json::dto::TransactionStatusDto;
	using internal::json::dto::transactions_page::TransactionsPageDto;
	using internal::json::dto::MultipleTransactionStatusDto;

	TransactionService::TransactionService(
			const Config& config,
			std::shared_ptr<internal::network::Context> context) :_config(config), _context(context) {}

	std::shared_ptr<transactions_info::BasicTransaction> TransactionService::getTransactionInfo(TransactionGroup group, const std::string &id) {
		std::stringstream path;
		path << "transactions/" << transactionGroupToString(group) << "/" << id;

        RequestParamsBuilder builder(_config);
        builder.setPath(path.str());
        builder.setMethod(internal::network::HTTPRequestMethod::GET);

		std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
		auto result = transaction_from_json(response);
		return result;
	}

    std::shared_ptr<transactions_info::BasicTransaction> TransactionService::getAnyTransactionInfo(const std::string &id) {
        TransactionGroup group = transactionGroupFromString(getTransactionStatus(id).group);
        return getTransactionInfo(group, id);
    }

    transactions_info::TransactionContainer TransactionService::getTransactionInfos(TransactionGroup group, const std::vector<std::string> &ids) {
		std::string requestJson;

		ParserJson::Write(ids, requestJson);
		requestJson = "{\"hashes\":" + requestJson + "}";

        std::stringstream path;
        path << "transactions/" << transactionGroupToString(group);

		RequestParamsBuilder builder(_config);
        builder.setPath(path.str());
        builder.setMethod(internal::network::HTTPRequestMethod::POST);
        builder.setRequestBody(requestJson);

		std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
		auto result = transactions_from_json(response);
		return result;
	}

	TransactionStatus TransactionService::getTransactionStatus(const std::string &id) {
		std::stringstream path;
        path << "transactionStatus/" << id;

		RequestParamsBuilder builder(_config);
        builder.setPath(path.str());
        builder.setMethod(internal::network::HTTPRequestMethod::GET);

		std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
		auto result = from_json<TransactionStatus, TransactionStatusDto>(response);
		return result;
	}

	MultipleTransactionStatus TransactionService::getTransactionStatuses(const std::vector<std::string> &ids) {
		std::string requestJson;

		ParserJson::Write(ids, requestJson);
		requestJson = "{\"hashes\":" + requestJson + "}";

		std::string path = "transactionStatus";

		RequestParamsBuilder builder(_config);
        builder.setPath(path);
        builder.setMethod(internal::network::HTTPRequestMethod::POST);
        builder.setRequestBody(requestJson);

		std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
		auto result = from_json<MultipleTransactionStatus, MultipleTransactionStatusDto>(response);

		return result;
	}

	bool TransactionService::announceNewTransaction(const std::vector<uint8_t > &payload) {
		std::string requestJson;
		std::string payloadStr = bytes_to_string(payload);
		requestJson = R"({"payload":")" + payloadStr + "\"}";

		std::string path = "transactions";

		RequestParamsBuilder builder(_config);
        builder.setPath(path);
        builder.setMethod(internal::network::HTTPRequestMethod::PUT);
        builder.setRequestBody(requestJson);

        internal::network::performHTTPRequest(_context, builder.getRequestParams());

		return true;
	}

	bool TransactionService::announceAggregateBoundedTransaction(const std::vector<uint8_t > &payload) {
		std::string requestJson;
		std::string payloadStr = bytes_to_string(payload);
        requestJson = R"({"payload":")" + payloadStr + "\"}";

		std::string path = "transactions/partial";

		RequestParamsBuilder builder(_config);
        builder.setPath(path);
        builder.setMethod(internal::network::HTTPRequestMethod::PUT);
        builder.setRequestBody(requestJson);

        internal::network::performHTTPRequest(_context, builder.getRequestParams());
        
		return true;
	}

	bool TransactionService::announceCosignatureTransaction(const std::vector<uint8_t > &payload) {
		std::string requestJson;
		std::string payloadStr = bytes_to_string(payload);
        requestJson = R"({"payload":")" + payloadStr + "\"}";

		std::string path = "transactions/cosignature";

		RequestParamsBuilder builder(_config);
        builder.setPath(path);
        builder.setMethod(internal::network::HTTPRequestMethod::PUT);
        builder.setRequestBody(requestJson);

        internal::network::performHTTPRequest(_context, builder.getRequestParams());
        
		return true;
	}

    transactions_page::TransactionsPage TransactionService::getTransactionsByGroup(TransactionGroup group, const TransactionsPageOptions &options) {
        std::stringstream path;
        path << "transactions/" << transactionGroupToString(group);

        RequestParamsBuilder builder(_config);
        builder.setPath(path.str(), options.toMap());
        builder.setMethod(internal::network::HTTPRequestMethod::GET);

        std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
        auto result = from_json<transactions_page::TransactionsPage, TransactionsPageDto>(response);
        return result;
    }

    std::string TransactionService::transactionGroupToString(TransactionGroup group) {
        std::string result;
        switch(group) {
            case TransactionGroup::Confirmed:
                result = "confirmed";
                break;
            case TransactionGroup::Unconfirmed:
                result = "unconfirmed";
                break;
            case TransactionGroup::Partial: {
                result = "partial";
            } break;
            default: {
                XPX_CHAIN_SDK_THROW_1(transaction_error, "unsupported transaction group", group)
            }
        }

        return result;
    }

    TransactionGroup TransactionService::transactionGroupFromString(const std::string& group) {
        if (group == "confirmed") {
            return TransactionGroup::Confirmed;
        } else if (group == "unconfirmed") {
            return TransactionGroup::Unconfirmed;
        } else if (group == "partial") {
            return TransactionGroup::Partial;
        } else {
            XPX_CHAIN_SDK_THROW_1(transaction_error, "unsupported transaction group", group)
        }
    }
}