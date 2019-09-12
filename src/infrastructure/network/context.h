#pragma once

#include <boost/asio/ssl.hpp>

namespace xpx_chain_sdk::internal::network {
	class Context {
	public:
		Context();

		Context(Context const&) = delete;
		Context& operator=(Context const&) = delete;

		boost::asio::ssl::context& get_ssl_context();
	private:
		boost::asio::ssl::context _ctx;
	};
}
