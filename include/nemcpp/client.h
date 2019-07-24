#pragma once

#include <memory>

#include <nemcpp/config.h>
#include <nemcpp/client/blockchain_service.h>

namespace nem2_sdk {
	class InvalidRequest : public std::runtime_error {
	public:
		explicit InvalidRequest(uint16_t code);

	private:
		static std::string getErrorMessage(uint16_t code);
	};

	class InvalidJSON : public std::runtime_error {
	public:
		explicit InvalidJSON(const runtime_error &error);
	};

	class IClient {
	public:
		virtual std::shared_ptr<IBlockchain> blockchain() const = 0;
		virtual ~IClient() = default;
	};

	std::shared_ptr<IClient> getClient(std::shared_ptr<Config> config);
}
