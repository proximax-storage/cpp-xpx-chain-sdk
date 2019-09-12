/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
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
