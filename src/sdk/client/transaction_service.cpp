#include <infrastructure/network/http.h>
#include "nemcpp/client/transaction_service.h"
#include <infrastructure/utils/deserialization_json.h>
#include <nemcpp/utils/bytes_to_string.h>


namespace xpx_sdk {

	using internal::json::dto::from_json;
	using internal::json::dto::transaction_from_json;
	using internal::json::dto::TransactionContainerDto;
	using ParserJson = internal::json::Parser;
	using simple_transactions::TransactionContainer;
	using internal::json::dto::TransactionStatusDto;
	using internal::json::dto::MultipleTransactionStatusDto;


	TransactionService::TransactionService(
			std::shared_ptr<Config> config,
			std::shared_ptr<internal::network::Context> context,
			std::shared_ptr<RequestParamsBuilder> builder):_config(config), _context(context), _builder(builder) {}

	std::shared_ptr<simple_transactions::BasicTransaction> TransactionService::getTransactionInfo(const std::string &id) {
		std::stringstream path;
		path << "transaction/" << id;

		auto requestParams = _builder
				->setPath(path.str())
				.setMethod(internal::network::HTTPRequestMethod::GET)
				.getRequestParams();

		std::string response = internal::network::performHTTPRequest(_context, requestParams);
		auto result = transaction_from_json(response);
		return result;
	}

	TransactionContainer TransactionService::getTransactionInfos(const std::vector<std::string> &ids) {
		std::string requestJson;

		ParserJson::Write(ids, requestJson);
		requestJson = "{\"transactionIds\":" + requestJson + "}";

		std::string path = "transaction";

		auto requestParams = _builder
				->setPath(path)
				.setMethod(internal::network::HTTPRequestMethod::POST)
				.setRequestBody(requestJson)
				.getRequestParams();

		std::string response = internal::network::performHTTPRequest(_context, requestParams);
		auto result = from_json<TransactionContainer, TransactionContainerDto>(response);

		return result;
	}

	TransactionStatus TransactionService::getTransactionStatus(const std::string &id) {
		std::stringstream path;
		path << "transaction/" << id << "/status";

		auto requestParams = _builder
				->setPath(path.str())
				.setMethod(internal::network::HTTPRequestMethod::GET)
				.getRequestParams();

		std::string response = internal::network::performHTTPRequest(_context, requestParams);
		auto result = from_json<TransactionStatus, TransactionStatusDto>(response);
		return result;
	}

	MultipleTransactionStatus TransactionService::getTransactionStatuses(const std::vector<std::string> &ids) {
		std::string requestJson;

		ParserJson::Write(ids, requestJson);
		requestJson = "{\"transactionIds\":" + requestJson + "}";

		std::string path = "transaction/status";

		auto requestParams = _builder
				->setPath(path)
				.setMethod(internal::network::HTTPRequestMethod::POST)
				.setRequestBody(requestJson)
				.getRequestParams();

		std::string response = internal::network::performHTTPRequest(_context, requestParams);
		auto result = from_json<MultipleTransactionStatus, MultipleTransactionStatusDto>(response);

		return result;
	}

	bool TransactionService::announceNewTransaction(const std::vector<uint8_t > &payload) {
		std::string requestJson;
		std::string payloadStr = bytes_to_string(payload);
		requestJson = "{\"payload\":" + payloadStr + "}";

		std::string path = "transaction";

		auto requestParams = _builder
				->setPath(path)
				.setMethod(internal::network::HTTPRequestMethod::PUT)
				.setRequestBody(requestJson)
				.getRequestParams();
		try {
			internal::network::performHTTPRequest(_context, requestParams);
		}
		catch(std::exception& e) {
			return false;
		}
		return true;
	}

	bool TransactionService::announceAggregateBoundedTransaction(const std::vector<uint8_t > &payload) {
		std::string requestJson;
		std::string payloadStr = bytes_to_string(payload);
		requestJson = "{\"payload\":" + payloadStr + "}";

		std::string path = "transaction/partial";

		auto requestParams = _builder
				->setPath(path)
				.setMethod(internal::network::HTTPRequestMethod::PUT)
				.setRequestBody(requestJson)
				.getRequestParams();
		try {
			internal::network::performHTTPRequest(_context, requestParams);
		}
		catch(std::exception& e) {
			return false;
		}
		return true;
	}

	bool TransactionService::announceCosignatureTransaction(const std::vector<uint8_t > &payload) {
		std::string requestJson;
		std::string payloadStr = bytes_to_string(payload);
		requestJson = "{\"payload\":" + payloadStr + "}";

		std::string path = "transaction/cosignature";

		auto requestParams = _builder
				->setPath(path)
				.setMethod(internal::network::HTTPRequestMethod::PUT)
				.setRequestBody(requestJson)
				.getRequestParams();
		try {
			internal::network::performHTTPRequest(_context, requestParams);
		}
		catch(std::exception& e) {
			return false;
		}
		return true;
	}
}