/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include "context.h"

#include <string>
#include <xpxchaincpp/config.h>

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
		explicit RequestParamsBuilder(std::shared_ptr<xpx_chain_sdk::Config> config);

		RequestParamsBuilder& setMethod(HTTPRequestMethod method);
		RequestParamsBuilder& setSecurity(bool secure);
		RequestParamsBuilder& setHost(const std::string& host);
		RequestParamsBuilder& setPort(const std::string& port);
		RequestParamsBuilder& setBasePath(const std::string& basePath);
		RequestParamsBuilder& setPath(const std::string& path,
                                      const std::map<std::string, std::string> &options = std::map<std::string, std::string>());
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
