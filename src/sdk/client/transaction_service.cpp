#include <infrastructure/network/http.h>
#include "nemcpp/client/transaction_service.h"
#include <infrastructure/utils/deserialization_json.h>


namespace xpx_sdk {

	using internal::json::dto::from_json;
	using ParserJson = internal::json::Parser;

	TransactionService::TransactionService(
			std::shared_ptr<Config> config,
			std::shared_ptr<internal::network::Context> context,
			std::shared_ptr<RequestParamsBuilder> builder):_config(config), _context(context), _builder(builder) {}

	TransactionInfo TransactionService::getTransactionInfo(const std::string &id) {
		std::stringstream path;
		path << "transaction/" << id;

		auto requestParams = _builder
				->setPath(path.str())
				.setMethod(internal::network::HTTPRequestMethod::GET)
				.getRequestParams();

		std::string response = internal::network::performHTTPRequest(_context, requestParams);
		auto result = from_json<TransactionInfo, TransactionInfoDto>(response);
		return result;
	}

	MultipleTransactionStatus TransactionService::getTransactionInfos(const std::vector<std::string> &ids) {
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
		auto result = from_json<MultipleTransactionInfo, MultipleTransactionInfoDto>(response);

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

	bool TransactionService::announceNewTransaction(const std::string &payload) {

	}

	bool TransactionService::announceAggregateBoundedTransaction(const std::string &payload) {

	}

	bool TransactionService::announceCosignatureTransaction(const std::string &payload) {

	}
}