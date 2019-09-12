#pragma once

#include "context.h"

#include <string>

namespace xpx_chain_sdk::internal::network {

	enum class HTTPRequestMethod {
		GET,
		POST,
		PUT
	};

	struct RequestParams {
		HTTPRequestMethod method;
		bool secure;
		std::string host;
		std::string port;
		std::string path;
		std::string request_body;
	};

	class RequestParamsBuilder {
	public:
		RequestParamsBuilder();

		RequestParamsBuilder& setMethod(HTTPRequestMethod method);
		RequestParamsBuilder& setSecurity(bool secure);
		RequestParamsBuilder& setHost(const std::string& host);
		RequestParamsBuilder& setPort(const std::string& port);
		RequestParamsBuilder& setBasePath(const std::string& basePath);
		RequestParamsBuilder& setPath(const std::string& path);
		RequestParamsBuilder& setRequestBody(const std::string& requestBody);

		RequestParams getRequestParams();
	private:
		HTTPRequestMethod _method;
		bool _secure;
		std::string _host;
		std::string _port;
		std::string _path;
		std::string _request_body;

		std::string _basePath;
	};

	std::string performHTTPRequest(
			std::shared_ptr<Context> context,
			const RequestParams &requestParams
	);
}
