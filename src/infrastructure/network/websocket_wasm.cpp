/**
*** Copyright 2024 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#ifdef __EMSCRIPTEN__

#include "websocket_wasm.h"

namespace xpx_chain_sdk::internal::network {


    //{"uid": "MR4GD5MHKIE6MEFLAYMMG2NPD5COGILF"}

    static EM_BOOL onopen( int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData )
    {
        emscripten_log( EM_LOG_CONSOLE, "WsClient: opened" );
        return EM_TRUE;
    }

    static EM_BOOL onerror( int eventType, const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData )
    {
        emscripten_log( EM_LOG_ERROR, "WsClient: error: %d", eventType );

        WsClient* pThis = (WsClient*) userData;

        boost::beast::error_code errorCode( boost::system::errc::connection_refused, boost::system::generic_category());
        pThis->onError(errorCode);
        return EM_TRUE;
    }

    static EM_BOOL onclose( int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData )
    {
        emscripten_log( EM_LOG_CONSOLE, "WsClient: closed: %d", eventType );
        return EM_TRUE;
    }

    bool isFirstMessage = true;

    static EM_BOOL onmessage( int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData )
    {
        emscripten_log( EM_LOG_CONSOLE, "WsClient: message: %d", (char*)websocketEvent->data );

        WsClient* pThis = (WsClient*) userData;

        if (! websocketEvent->isText)
        {
            emscripten_log( EM_LOG_ERROR, "WsClient: message must be text" );

            boost::beast::error_code errorCode( boost::system::errc::protocol_error, boost::system::generic_category());
            pThis->onError(errorCode);
            return EM_TRUE;
        }

        if ( isFirstMessage )
        {
            pThis->onConnectionUid( std::string( (char*)websocketEvent->data, websocketEvent->numBytes) );
            isFirstMessage = false;
            return EM_TRUE;
        }

        pThis->onMessage( std::string( (char*)websocketEvent->data, websocketEvent->numBytes) );

        return EM_TRUE;
    }


    WsClient::WsClient(
            const Config& config,
            std::shared_ptr<boost::asio::io_context> context,
            Callback connectionCallback,
            Callback receiverCallback,
            ErrorCallback errorCallback) :
            _config(config),
            _io_context(context),
            _connectionCallback(connectionCallback),
            _receiverCallback(receiverCallback),
            _errorCallback(errorCallback)
    {
    }

    WsClient::~WsClient() {

    }

    void WsClient::connect()
    {
        if ( !emscripten_websocket_is_supported() )
        {
            emscripten_log( EM_LOG_ERROR, "WsClient: !emscripten_websocket_is_supported()" );

            boost::beast::error_code errorCode( boost::system::errc::wrong_protocol_type, boost::system::generic_category());
            onError(errorCode);
            return;
        }

        m_wsUrl = "wss://" + _config.nodeAddress + ":" + _config.port;
        EmscriptenWebSocketCreateAttributes ws_attrs = {
            m_wsUrl.c_str(),
            NULL,
            EM_FALSE //EM_TRUE
        };

        m_ws = emscripten_websocket_new( &ws_attrs );
        emscripten_websocket_set_onopen_callback( m_ws, this, onopen );
        emscripten_websocket_set_onerror_callback( m_ws, this, onerror);
        emscripten_websocket_set_onclose_callback( m_ws, this, onclose);
        emscripten_websocket_set_onmessage_callback( m_ws, this, onmessage);
    }

    void WsClient::disconnect()
    {
        auto result = emscripten_websocket_close( m_ws, 0, 0 );
        emscripten_websocket_delete( m_ws );
        if (result)
        {
            emscripten_log( EM_LOG_ERROR, "WsClient::disconnect: failed emscripten_websocket_close(): %d", result);
        }
    }

    void WsClient::send(const std::string& json, std::function<void()> onSuccess,
                        std::function<void(boost::beast::error_code errorCode)> onError)
    {
        auto result = emscripten_websocket_send_utf8_text( m_ws, json.c_str() );

        if (result)
        {
            boost::beast::error_code errorCode( boost::system::errc::protocol_error, boost::system::generic_category());
            onError(errorCode);
            return;
        }
        onSuccess();
    }

    bool WsClient::isConnected() const {
        return m_ws > 0 ;
    }

}

#endif // #ifdef __EMSCRIPTEN__
