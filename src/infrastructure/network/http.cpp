/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "http.h"
#include <xpxchaincpp/client.h>

#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>


using namespace xpx_chain_sdk::internal::network;

namespace asio = boost::asio;
namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;
namespace beast = boost::beast;
namespace http = boost::beast::http;

// HTTP 1.1
const auto HTTP_VERSION = 11;

template <class TStream>
std::string _performHTTPRequest_internal(
		TStream& stream,
		HTTPRequestMethod method,
		const std::string& host,
		const std::string& port,
		const std::string& path,
		const std::string& request_body
) {

	http::verb verb;
	switch (method) {
		case HTTPRequestMethod::GET:
			verb = http::verb::get;
			break;
		case HTTPRequestMethod::POST:
			verb = http::verb::post;
			break;
		case HTTPRequestMethod::PUT:
			verb = http::verb::put;
			break;
	}

	http::request<http::string_body> request{verb, path, HTTP_VERSION};
	request.set(http::field::host, host);
	request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request.set(http::field::content_type, "application/json");

	if (method == HTTPRequestMethod::PUT || method == HTTPRequestMethod::POST) {
		request.set(http::field::content_length, std::to_string(request_body.size()));
//		request.set(http::field::body, request_body);
		request.body() = request_body;
		request.prepare_payload();
	}

	http::write(stream, request);

	beast::flat_buffer buffer;
	http::response<http::string_body> response;
	http::read(stream, buffer, response);

	if (response.result() == http::status::found) {
		auto path = response.at("Location").to_string();
		return _performHTTPRequest_internal(stream, method, host, port, path, request_body);
	} else if (response.result() != http::status::ok && response.result() != http::status::accepted ) {
		throw xpx_chain_sdk::InvalidRequest(response.body(), static_cast<uint16_t>(response.result()));
	}

	return response.body();
}

std::string _performHTTPRequest(
		std::shared_ptr<Context> context,
		HTTPRequestMethod method,
		bool secure,
		const std::string& host,
		const std::string& port,
		const std::string& path,
		const std::string& request_body
) {

	asio::io_context ioc;

	ip::tcp::resolver resolver(ioc);
	auto const endpoint = resolver.resolve(host, port);

	if (secure) {
		auto& ctx = context->get_ssl_context();
		beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);
		beast::get_lowest_layer(stream).connect(endpoint);
		stream.handshake(ssl::stream_base::client);
		auto response = _performHTTPRequest_internal(stream, method, host, port, path, request_body);
		stream.shutdown();
		return response;
	} else {
		beast::tcp_stream stream(ioc);
		stream.connect(endpoint);
		auto response = _performHTTPRequest_internal(stream, method, host, port, path, request_body);
		stream.socket().shutdown(ip::tcp::socket::shutdown_both);
		return response;
	}
}

RequestParamsBuilder::RequestParamsBuilder(const xpx_chain_sdk::Config& config) :
	_method(HTTPRequestMethod::GET),
	_secure(config.useSSL),
	_host(config.nodeAddress),
	_port(config.port),
	_basePath(config.basePath)
{}

RequestParamsBuilder& RequestParamsBuilder::setMethod(HTTPRequestMethod method) {
	_method = method;
	return *this;
}

RequestParamsBuilder& RequestParamsBuilder::setSecurity(bool secure) {
	_secure = secure;
	return *this;
}

RequestParamsBuilder& RequestParamsBuilder::setHost(const std::string& host) {
	_host = host;
	return *this;
}

RequestParamsBuilder& RequestParamsBuilder::setPort(const std::string& port) {
	_port = port;
	return *this;
}

RequestParamsBuilder& RequestParamsBuilder::setBasePath(const std::string& basePath) {
	_basePath = basePath;
	return *this;
}

RequestParamsBuilder &RequestParamsBuilder::setPath(
        const std::string &path,
        const std::map<std::string, std::string> &options) {
    _path = path;

    const int size = static_cast<int>(options.size());
    auto it = options.begin();
    for (int i = 0; i < size; i++) {
        if (i == 0) {
            _path += "?" + it->first + "=" + it->second;
        } else {
            _path += "&" + it->first + "=" + it->second;
        }
        it++;
    }

    return *this;
}

RequestParamsBuilder& RequestParamsBuilder::setRequestBody(const std::string& requestBody) {
	_request_body = requestBody;
	return *this;
}

RequestParams RequestParamsBuilder::getRequestParams() {
	return RequestParams{
		_method,
		_secure,
		_host,
		_port,
		_basePath + _path,
		_request_body
	};
}

namespace xpx_chain_sdk::internal::network {
	std::string performHTTPRequest(std::shared_ptr<Context> context, const RequestParams &requestParams) {
		std::string port;
		if (requestParams.port.empty()) {
			if (requestParams.secure) {
				port = "443";
			} else {
				port = "80";
			}
		} else {
			port = requestParams.port;
		}
		std::string path;
        if (requestParams.path.empty()) {
            path = "/";
        } else {
            path = requestParams.path;
        }

        return _performHTTPRequest(
				context,
				requestParams.method,
				requestParams.secure,
				requestParams.host,
				port,
				path,
				requestParams.request_body
		);
    }
}
