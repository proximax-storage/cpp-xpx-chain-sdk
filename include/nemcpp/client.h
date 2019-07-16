#pragma once

#include <nemcpp/config.h>
#include <nemcpp/client/block.h>

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

	class Client {
	public:
		explicit Client(std::shared_ptr<Config> config);
		std::shared_ptr<Blockchain> blockchain() const;

	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<Blockchain> _blockchain;
	};
}
