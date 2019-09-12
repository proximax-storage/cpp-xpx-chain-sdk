#include "infrastructure/network/context.h"
#include "infrastructure/network/http.h"
#include <nemcpp/client.h>
#include <nemcpp/client/blockchain_service.h>
#include <nemcpp/client/mosaic_service.h>
#include <nemcpp/client/namespace_service.h>
#include <nemcpp/client/network_service.h>
#include <nemcpp/client/account_service.h>

using namespace xpx_sdk;

class Client : public IClient {
public:
	explicit Client(std::shared_ptr<Config> config) : _config(config) {
		_context = std::make_shared<internal::network::Context>();
		_builder = _builder
			.setBasePath(_config->basePath)
			.setHost(_config->nodeAddress)
			.setPort(_config->port)
			.setSecurity(_config->useSSL);

		_account     = std::make_shared<AccountService>(config, _context, std::make_shared<RequestParamsBuilder>(_builder));
		_blockchain  = std::make_shared<BlockchainService>(config, _context, std::make_shared<RequestParamsBuilder>(_builder));
		_mosaic      = std::make_shared<MosaicService>(config, _context, std::make_shared<RequestParamsBuilder>(_builder));
		_namespace   = std::make_shared<NamespaceService>(config, _context, std::make_shared<RequestParamsBuilder>(_builder));
		_network     = std::make_shared<NetworkService>(config, _context, std::make_shared<RequestParamsBuilder>(_builder));
		_transaction = std::make_shared<TransactionService>(config, _context, std::make_shared<RequestParamsBuilder>(_builder));
	}

	std::shared_ptr<AccountService> account() const override  {
	    return _account;
	}

	std::shared_ptr<BlockchainService> blockchain() const override {
		return _blockchain;
	}

    std::shared_ptr<MosaicService> mosaics() const override {
        return _mosaic;
    }

    std::shared_ptr<NamespaceService> namespaces() const override {
        return _namespace;
    }

    std::shared_ptr<NetworkService> network() const override {
        return _network;
    }

	std::shared_ptr<TransactionService> transactions() const override {
		return _transaction;
	}

private:
	std::shared_ptr<Config> _config;
	internal::network::RequestParamsBuilder _builder;
	std::shared_ptr<internal::network::Context> _context;

	std::shared_ptr<AccountService> _account;
	std::shared_ptr<BlockchainService> _blockchain;
	std::shared_ptr<MosaicService> _mosaic;
	std::shared_ptr<NamespaceService> _namespace;
	std::shared_ptr<NetworkService> _network;
	std::shared_ptr<TransactionService> _transaction;
};

namespace xpx_sdk {
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

InvalidJson::InvalidJson(const std::runtime_error &error) : runtime_error(error) {}
