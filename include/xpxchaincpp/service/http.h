/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <boost/asio/ip/tcp.hpp>

namespace xpx_chain_sdk::http {
	using StatusCode = int;

	enum class ContextStatus {
		Ready,
		InvalidBaseURL,
	};

	class Context {
	public:
		explicit Context(char* p_base_url);

		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		ContextStatus status();

	private:
		boost::asio::io_context _io_context;
		char* _p_base_url;
	};
}
