//#pragma once
//
//#include "context.h"
//
//#include <string>
//
//#include <boost/beast.hpp>
//#include <boost/beast/websocket.hpp>
//#include <boost/beast/ssl.hpp>
//
//namespace xpx_chain_sdk::internal::network {
//	namespace asio = boost::asio;
//	namespace ip = boost::asio::ip;
//	namespace beast = boost::beast;
//	namespace websocket = boost::beast::websocket;
//
//	struct ConnectionParams {
//		bool secure;
//		std::string& host;
//		std::string& port;
//		std::string& path;
//	};
//
//	class WebSocket {
//	public:
//		WebSocket(Context& context, ConnectionParams& params);
//
//		WebSocket(WebSocket const&) = delete;
//		WebSocket& operator=(WebSocket const&) = delete;
//
//		~WebSocket();
//
//		void connect();
//		void close();
//
//		friend std::ostream& operator<<(std::ostream &output, const WebSocket& ws);
//		friend std::istream& operator>>(std::istream &input, WebSocket& ws);
//
//	private:
//		class _IWebSocketImpl {
//		public:
//			virtual void connect() = 0;
//			virtual void close() = 0;
//
//			virtual void send(const std::string&) = 0;
//			virtual std::string receive() = 0;
//
//			virtual ~_IWebSocketImpl() = 0;
//		};
//
//		template <typename TStream>
//		class _WebSocketImpl : public _IWebSocketImpl {
//		public:
//			_WebSocketImpl(ConnectionParams connect,
//						   std::shared_ptr<asio::io_context> ioc,
//						   std::shared_ptr<websocket::stream<TStream>> ws);
//
//			void connect() override;
//			void close() override;
//
//			void send(const std::string&) override;
//			std::string receive() override;
//
//			~_WebSocketImpl() override;
//
//		private:
//			ConnectionParams _connect;
//			std::shared_ptr<asio::io_context> _ioc;
//			std::shared_ptr<websocket::stream<TStream>> _ws;
//		};
//
//		_IWebSocketImpl* _impl;
//	};
//}

