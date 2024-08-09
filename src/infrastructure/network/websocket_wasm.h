/**
*** Copyright 2024 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/config.h>
#include <boost/beast/core.hpp>
#include <cstdlib>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>

namespace xpx_chain_sdk::internal::network {

    class Context;

    using Callback = std::function<void(const std::string& json)>;
    using ErrorCallback = std::function<void(const boost::beast::error_code& errorCode)>;

    class WsClient : public std::enable_shared_from_this<WsClient>
    {
        EMSCRIPTEN_WEBSOCKET_T  m_ws = 0;
        std::string             m_wsUrl;

    public:
        WsClient(
            const Config& config,
            std::shared_ptr<boost::asio::io_context> context,
            Callback connectionCallback,
            Callback receiverCallback,
            ErrorCallback errorCallback);

        ~WsClient();

        void connect();
        void disconnect();
        void send(const std::string& json, std::function<void()> onSuccess,
                  std::function<void(boost::beast::error_code errorCode)> onError);
        bool isConnected() const;

        void onError( boost::beast::error_code errorCode )
        {
            _io_context->post( [this,errorCode=errorCode] { _errorCallback(errorCode); } );
        }

        void onMessage( const std::string& json )
        {
            _io_context->post( [this,json=json] {_receiverCallback(json); } );
        }

        void onConnectionUid( const std::string& json )
        {
            _io_context->post( [this,json=json] { _connectionCallback(json); } );
        }

    private:
        const Config& _config;
        // std::deque<std::pair<std::string, std::pair<std::function<void()>, std::function<void(boost::beast::error_code errorCode)>>>> _outgoingQueue;
        std::shared_ptr<boost::asio::io_context> _io_context;
        std::function<void()> _postponedDisconnect;
        Callback _connectionCallback;
        Callback _receiverCallback;
        ErrorCallback _errorCallback;
    };
}
