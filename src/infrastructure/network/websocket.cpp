//#include "websocket.h"
//
//#include <boost/asio.hpp>
//#include <boost/beast.hpp>
//#include <boost/beast/websocket.hpp>
//#include <boost/beast/ssl.hpp>
//
//namespace asio = boost::asio;
//namespace ip = boost::asio::ip;
//namespace beast = boost::beast;
//namespace websocket = boost::beast::websocket;
//
//using namespace xpx_chain_sdk::internal::network;
//
//template <typename InternetProtocol>
//void _connectImpl(
//		std::shared_ptr<websocket::stream<beast::ssl_stream<beast::tcp_stream>>> ws,
//		ip::basic_resolver_results<InternetProtocol> endpoint
//) {
//	ws->next_layer().next_layer().connect(endpoint);
//	ws->next_layer().handshake(ssl::stream_base::client);
//}
//
//template <typename InternetProtocol>
//void _connectImpl(
//		std::shared_ptr<websocket::stream<beast::tcp_stream>> ws,
//		ip::basic_resolver_results<InternetProtocol> endpoint
//) {
//	ws->next_layer().connect(endpoint);
//}
//
//WebSocket::_IWebSocketImpl::~_IWebSocketImpl() {}
//
//template <typename TStream>
//WebSocket::_WebSocketImpl<TStream>::_WebSocketImpl(
//		ConnectionParams connect,
//		std::shared_ptr<asio::io_context> ioc,
//		std::shared_ptr<websocket::stream<TStream>> ws
//) :
//	_connect(connect),
//	_ioc(ioc),
//	_ws(ws)
//{}
//
//template <typename TStream>
//void WebSocket::_WebSocketImpl<TStream>::connect() {
//	ip::tcp::resolver resolver(*_ioc);
//	auto const endpoint = resolver.resolve(_connect.host, _connect.port);
//
//	_connectImpl(_ws, endpoint);
//	_ws->set_option(websocket::stream_base::decorator(
//		[](websocket::request_type& req) {
//			req.set(beast::http::field::user_agent,
//					std::string(BOOST_BEAST_VERSION_STRING)
//			);
//		}
//	));
//	_ws->handshake(_connect.host, _connect.path);
//}
//
//template <typename TStream>
//void WebSocket::_WebSocketImpl<TStream>::close() {
//	_ws->close(websocket::close_code::normal);
//}
//
//template <typename TStream>
//void WebSocket::_WebSocketImpl<TStream>::send(const std::string& data) {
//	_ws->write(asio::buffer(data));
//}
//
//template <typename TStream>
//std::string WebSocket::_WebSocketImpl<TStream>::receive() {
//	beast::flat_buffer buffer;
//	_ws->read(buffer);
//	return beast::buffers_to_string(buffer.data());
//}
//
//template <typename TStream>
//WebSocket::_WebSocketImpl<TStream>::~_WebSocketImpl() {
//	close();
//}
//
//WebSocket::WebSocket(Context& context, ConnectionParams& params) {
//	auto ioc = std::make_shared<asio::io_context>();
//
//	if (params.secure) {
//		auto ws = std::make_shared<websocket::stream<beast::ssl_stream<beast::tcp_stream>>>(*ioc, context.get_ssl_context());
//		_impl = new _WebSocketImpl(params, ioc, ws);
//	} else {
//		auto ws = std::make_shared<websocket::stream<beast::tcp_stream>>(*ioc);
//		_impl = new _WebSocketImpl(params, ioc, ws);
//	}
//}
//
//void WebSocket::connect() {
//	_impl->connect();
//}
//
//void WebSocket::close() {
//	_impl->close();
//}
//
//WebSocket::~WebSocket() {
//	delete _impl;
//}
//
//namespace xpx_chain_sdk::internal::network {
//	std::ostream& operator<<(std::ostream &output, const WebSocket &ws) {
//		output << ws._impl->receive();
//		return output;
//	}
//
//	std::istream& operator>>(std::istream &input, WebSocket& ws) {
//		std::string value;
//		input >> value;
//		ws._impl->send(value);
//		return input;
//	}
//}
