#pragma once

#include <boost/asio/ip/tcp.hpp>

namespace xpx_sdk::http {
	typedef int StatusCode;

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
