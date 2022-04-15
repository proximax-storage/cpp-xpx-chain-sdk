/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "infrastructure/network/context.h"
#include <infrastructure/utils/deserialization_json.h>
#include <xpxchaincpp/client.h>
#include <xpxchaincpp/client/blockchain_service.h>
#include <xpxchaincpp/client/mosaic_service.h>
#include <xpxchaincpp/client/namespace_service.h>
#include <xpxchaincpp/client/network_service.h>
#include <xpxchaincpp/client/account_service.h>

using namespace xpx_chain_sdk;

class Client : public IClient {
public:
	explicit Client(std::shared_ptr<Config> config) : _config(config) {
        _context            = std::make_shared<internal::network::Context>();
        _account            = std::make_shared<AccountService>(config, _context);
        _blockchain         = std::make_shared<BlockchainService>(config, _context);
        _mosaic             = std::make_shared<MosaicService>(config, _context);
        _namespace          = std::make_shared<NamespaceService>(config, _context);
        _notification       = nullptr;
        _network            = std::make_shared<NetworkService>(config, _context);
        _liquidityProvider  = std::make_shared<LiquidityProviderService>(config, _context);
        _transaction        = std::make_shared<TransactionService>(config, _context);
        _storage            = std::make_shared<StorageService>(config, _context);
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

    std::shared_ptr<NotificationService> notifications() const override {
        if (!_notification) {
            _notification = std::make_shared<NotificationService>(_config, _context);
        }
        return _notification;
    }

    std::shared_ptr<NetworkService> network() const override {
        return _network;
    }

    std::shared_ptr<LiquidityProviderService> liquidityProvider() const override {
        return _liquidityProvider;
    }

	std::shared_ptr<TransactionService> transactions() const override {
		return _transaction;
	}

	std::shared_ptr<StorageService> storage() const override {
		return _storage;
	}

private:
    std::shared_ptr<Config> _config;
    std::shared_ptr<internal::network::Context> _context;

    std::shared_ptr<AccountService> _account;
    std::shared_ptr<BlockchainService> _blockchain;
    std::shared_ptr<MosaicService> _mosaic;
    std::shared_ptr<NamespaceService> _namespace;
    mutable std::shared_ptr<NotificationService> _notification;
    std::shared_ptr<NetworkService> _network;
    std::shared_ptr<LiquidityProviderService> _liquidityProvider;
    std::shared_ptr<TransactionService> _transaction;
    std::shared_ptr<StorageService> _storage;
};

namespace xpx_chain_sdk {
	std::shared_ptr<IClient> getClient(std::shared_ptr<Config> config) {
		return std::make_shared<Client>(config);
	}
}

InvalidRequest::InvalidRequest(const std::string& message, uint16_t code) :
    runtime_error(InvalidRequest::getErrorMessage(code)),
    httpErrorCode(code),
	errorMessage(message)
{}

int InvalidRequest::getHttpErrorCode() const {
    return httpErrorCode;
}

ErrorMessage InvalidRequest::getErrorMessage() const {
    return internal::json::dto::from_json<ErrorMessage, internal::json::dto::ErrorMessageDto>(errorMessage);
}

std::string InvalidRequest::getErrorMessage(uint16_t code) {
	std::stringstream s;
	s << "Server rejected the request. Error code: " << code;
	return s.str();
}

InvalidJson::InvalidJson(const std::runtime_error &error) : runtime_error(error) {}
