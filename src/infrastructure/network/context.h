#pragma once

#include <boost/asio/ssl.hpp>

namespace nem2_sdk::internal::network {
	namespace ssl = boost::asio::ssl;

	class Context {
	public:
		Context();

		Context(Context const&) = delete;
		Context& operator=(Context const&) = delete;

		ssl::context& get_ssl_context();
	private:
		ssl::context _ctx;
	};
}
