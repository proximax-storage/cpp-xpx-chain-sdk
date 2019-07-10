#include "http.h"

#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

using namespace nem2_sdk::internal::network;

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

	if (method == HTTPRequestMethod::PUT || method == HTTPRequestMethod::POST) {
		request.set(http::field::body, request_body);
	}

	http::write(stream, request);

	beast::flat_buffer buffer;
	http::response<http::string_body> response;
	http::read(stream, buffer, response);

	return response.body();
}

std::string _performHTTPRequest(
		Context& context,
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
		auto& ctx = context.get_ssl_context();
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

std::string performHTTPRequest(Context& context, const RequestParams& requestParams) {
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
