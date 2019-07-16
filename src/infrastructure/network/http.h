#pragma once

#include "context.h"

#include <string>

namespace nem2_sdk::internal::network {

	enum class HTTPRequestMethod {
		GET,
		POST,
		PUT,
	};

	struct RequestParams {
		HTTPRequestMethod method;
		bool secure;
		std::string host;
		std::string port;
		std::string path;
		std::string request_body;
	};

	std::string performHTTPRequest(
			std::shared_ptr<Context> context,
			const RequestParams &requestParams
	);
}
