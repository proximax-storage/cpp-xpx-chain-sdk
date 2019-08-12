#include "infrastructure/network/context.h"
#include "infrastructure/network/http.h"
#include <nemcpp/client.h>
#include <sdk/client/blockchain_service.h>
#include <sdk/client/mosaic_service.h>
#include <sdk/client/namespace_service.h>
#include <sdk/client/network_service.h>
#include <sdk/client/account_service.h>

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

		_account    = std::make_shared<AccountService>(config, _context, _builder);
		_blockchain = std::make_shared<BlockchainService>(config, _context, _builder);
		_mosaic     = std::make_shared<MosaicService>(config, _context, _builder);
		_namespace  = std::make_shared<NamespaceService>(config, _context, _builder);
		_network    = std::make_shared<NetworkService>(config, _context, _builder);
	}

	std::shared_ptr<IAccountService> account() const override  {
	    return _account;
	}

	std::shared_ptr<IBlockchainService> blockchain() const override {
		return _blockchain;
	}

    std::shared_ptr<IMosaicService> mosaics() const override {
        return _mosaic;
    }

    std::shared_ptr<INamespaceService> namespaces() const override {
        return _namespace;
    }

    std::shared_ptr<INetworkService> network() const override {
        return _network;
    }

private:
	std::shared_ptr<Config> _config;
	internal::network::RequestParamsBuilder _builder;
	std::shared_ptr<internal::network::Context> _context;

	std::shared_ptr<IAccountService> _account;
	std::shared_ptr<IBlockchainService> _blockchain;
	std::shared_ptr<IMosaicService> _mosaic;
	std::shared_ptr<INamespaceService> _namespace;
	std::shared_ptr<INetworkService> _network;
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
