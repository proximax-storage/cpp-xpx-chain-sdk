/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/config.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/thread.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace xpx_chain_sdk::internal::network {

    class Context;

    using Callback = std::function<void(const std::string& json)>;
    using ErrorCallback = std::function<void(const boost::beast::error_code& errorCode)>;

    class WsClient : public std::enable_shared_from_this<WsClient> {
    public:
        WsClient(
            std::shared_ptr<Config> config,
            std::shared_ptr<internal::network::Context> context,
            std::shared_ptr<boost::asio::io_context> ioContext,
            Callback connectionCallback,
            Callback receiverCallback,
            ErrorCallback errorCallback);
        ~WsClient();

        void connect(uint64_t onResolveHostTimeoutSec = 30);
        void disconnect();
        void send(const std::string& json);
        bool isConnected();

    private:
        void doWrite();
        void onResolve(
                boost::beast::error_code errorCode,
                const boost::asio::ip::tcp::resolver::results_type& resultsType,
                uint64_t onResolveHostTimeoutSec);
        void onConnect(
                boost::beast::error_code errorCode,
                const boost::asio::ip::tcp::resolver::results_type::endpoint_type& endpointType);
        void onHandshake(boost::beast::error_code errorCode);
        void onRead(
                boost::beast::error_code errorCode,
                std::size_t bytes_transferred);
        void readNext();
        void onClose(boost::beast::error_code errorCode);

    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
        std::shared_ptr<boost::asio::streambuf> _buffer;
        std::shared_ptr<boost::asio::io_context> _io_context;
        std::deque<std::string> _outgoingQueue;
        std::atomic<bool> _isConnected;
        boost::asio::strand<boost::asio::io_context::executor_type> _strand;
        boost::asio::ip::tcp::resolver _resolver;
        boost::beast::websocket::stream<boost::beast::tcp_stream> _ws;
        Callback _connectionCallback;
        Callback _receiverCallback;
        ErrorCallback _errorCallback;
    };
}