#include "infrastructure/network/context.h"
#include "infrastructure/network/http.h"
#include <nemcpp/client.h>
#include <sdk/client/blockchain_impl.h>

using namespace nem2_sdk;

class Client : public IClient {
public:
	explicit Client(std::shared_ptr<Config> config) : _config(config) {
		_context = std::make_shared<internal::network::Context>();
		_builder = _builder
			.setBasePath(_config->basePath)
			.setHost(_config->nodeAddress)
			.setPort(_config->port)
			.setSecurity(_config->useSSL);

		_blockchain = std::make_shared<Blockchain>(config, _context, _builder);
	}

	std::shared_ptr<IBlockchain> blockchain() const override {
		return _blockchain;
	}

private:
	std::shared_ptr<Config> _config;
	internal::network::RequestParamsBuilder _builder;
	std::shared_ptr<internal::network::Context> _context;
	std::shared_ptr<Blockchain> _blockchain;
};

namespace nem2_sdk {
	std::shared_ptr<IClient> getClient(std::shared_ptr<Config> config) {
		return std::make_shared<Client>(config);
	}
}

InvalidRequest::InvalidRequest(uint16_t code) :
	runtime_error(InvalidRequest::getErrorMessage(code))
{}

std::string InvalidRequest::getErrorMessage(uint16_t code) {
	std::stringstream s;
	s << "Server rejected the request. Error code: " << code;
	return s.str();
}

InvalidJSON::InvalidJSON(const std::runtime_error &error) : runtime_error(error) {}
