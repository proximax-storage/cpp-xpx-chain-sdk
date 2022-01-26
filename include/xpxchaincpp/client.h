/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/config.h>
#include <xpxchaincpp/client/blockchain_service.h>
#include <xpxchaincpp/client/mosaic_service.h>
#include <xpxchaincpp/client/namespace_service.h>
#include <xpxchaincpp/client/notification_service.h>
#include <xpxchaincpp/client/account_service.h>
#include <xpxchaincpp/client/network_service.h>
#include <xpxchaincpp/client/transaction_service.h>
#include <xpxchaincpp/client/storage_service.h>

#include <memory>

#include <xpxchaincpp/config.h>
#include <xpxchaincpp/client/blockchain_service.h>

namespace xpx_chain_sdk {
    class ErrorMessage  {
    public:
        std::string code;
        std::string message;
    };

	class InvalidRequest : public std::runtime_error {
	public:
		explicit InvalidRequest(const std::string& errorMessage, uint16_t code);

    public:
        int getErrorCode() const;
        ErrorMessage getErrorMessage() const;

	private:
		static std::string getErrorMessage(uint16_t code);

    private:
        const int errorCode;
        const std::string errorMessage;
	};

	class InvalidJson : public std::runtime_error {
	public:
		explicit InvalidJson(const runtime_error &error);
	};

	class IClient {
	public:
        virtual std::shared_ptr<AccountService> account() const = 0;
        virtual std::shared_ptr<BlockchainService> blockchain() const = 0;
        virtual std::shared_ptr<MosaicService> mosaics() const = 0;
        virtual std::shared_ptr<NamespaceService> namespaces() const = 0;
        virtual std::shared_ptr<NotificationService> notifications() const = 0;
        virtual std::shared_ptr<NetworkService> network() const = 0;
        virtual std::shared_ptr<TransactionService> transactions() const = 0;
        virtual std::shared_ptr<StorageService> storage() const = 0;
        virtual ~IClient() = default;
	};

	std::shared_ptr<IClient> getClient(std::shared_ptr<Config> config);
}
