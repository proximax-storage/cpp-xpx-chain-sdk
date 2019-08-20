#pragma once


#include <nemcpp/config.h>
#include <nemcpp/client/blockchain_service.h>
#include <nemcpp/client/mosaic_service.h>
#include <nemcpp/client/namespace_service.h>
#include <nemcpp/client/account_service.h>
#include <nemcpp/client/network_service.h>

#include <memory>

#include <nemcpp/config.h>
#include <nemcpp/client/blockchain_service.h>

namespace xpx_sdk {
	class InvalidRequest : public std::runtime_error {
	public:
		explicit InvalidRequest(uint16_t code);

	private:
		static std::string getErrorMessage(uint16_t code);
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
        virtual std::shared_ptr<NetworkService> network() const = 0;
        virtual ~IClient() = default;
	};

	std::shared_ptr<IClient> getClient(std::shared_ptr<Config> config);
}
