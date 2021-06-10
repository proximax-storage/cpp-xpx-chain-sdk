/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include "websocket.h"

namespace xpx_chain_sdk::internal::network {

    WsClient::WsClient(
            std::shared_ptr<Config> config,
            std::shared_ptr<internal::network::Context> context,
            Callback connectionCallback,
            Callback receiverCallback,
            ErrorCallback errorCallback) :
            _config(config), _context(context), _io_context(boost::asio::io_context()),
            _resolver(boost::asio::ip::tcp::resolver(_io_context)),
            _ws(boost::asio::make_strand(_io_context)),
            _connectionCallback(connectionCallback),
            _receiverCallback(receiverCallback),
            _errorCallback(errorCallback) {}

    WsClient::~WsClient() {
        disconnect();
    }

    void WsClient::connect(uint64_t onResolveHostTimeoutSec) {
        if (!isConnected()) {
            _resolver.async_resolve(_config->nodeAddress,
                                    _config->port,
                                    [pThis = shared_from_this(), onResolveHostTimeoutSec] (
                                            boost::beast::error_code errorCode,
                                            const boost::asio::ip::tcp::resolver::results_type &resultsType) {
                                        pThis->onResolve(errorCode, resultsType, onResolveHostTimeoutSec);
                                    });

            boost::thread thread([pThis = shared_from_this()]() { pThis->_io_context.run(); });
            thread.detach();
        }
    }

    void WsClient::disconnect() {
        if (isConnected()) {
            _ws.async_close(boost::beast::websocket::close_code::normal,
            [pThis = shared_from_this()](auto errorCode){
                pThis->onClose(errorCode);
            });
        }
    }

    void WsClient::send(const std::string& json) {
        if (isConnected()) {
            _ws.async_write(
                    boost::asio::buffer(json),
                    [pThis = shared_from_this()](auto errorCode, auto bytesTransferred) {
                        pThis->onWrite(errorCode, bytesTransferred);
                    });
        }
    }

    bool WsClient::isConnected() {
        return _ws.is_open();
    }

    void WsClient::onResolve(
            boost::beast::error_code errorCode,
            const boost::asio::ip::tcp::resolver::results_type& resultsType,
            uint64_t onResolveHostTimeoutSec) {
        if (errorCode) {
            return _errorCallback(errorCode);
        }

        boost::beast::get_lowest_layer(_ws).expires_after(std::chrono::seconds(onResolveHostTimeoutSec));
        boost::beast::get_lowest_layer(_ws).async_connect(
                resultsType, [pThis = shared_from_this()] (auto ec, const auto& et) {
                    pThis->onConnect(ec, et);
                });
    }

    void WsClient::onConnect(
            boost::beast::error_code errorCode,
            const boost::asio::ip::tcp::resolver::results_type::endpoint_type&) {
        if (errorCode) {
            return _errorCallback(errorCode);
        }

        // Turn off the timeout on the tcp_stream, because
        // the websocket stream has its own timeout system.
        boost::beast::get_lowest_layer(_ws).expires_never();

        // Set suggested timeout settings for the websocket
        _ws.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::client));

        const std::string host = _config->nodeAddress + ":" + _config->port;

        std::cout << "websocket: connection established: " << host << std::endl;

        // Perform the websocket handshake
        _ws.async_handshake(host, _config->baseWsPath,[pThis = shared_from_this()](auto ec){
            pThis->onHandshake(ec);
        });
    }

    void WsClient::onHandshake(boost::beast::error_code errorCode) {
        if (errorCode) {
            return _errorCallback(errorCode);
        } else {
            std::shared_ptr<boost::beast::flat_buffer> pBuffer(new boost::beast::flat_buffer);
            _ws.async_read(
                    *pBuffer,
                    [pThis = shared_from_this(), pBuffer] (
                            boost::beast::error_code ec,
                            std::size_t bytesTransferred) {
                        if (ec) {
                            return pThis->_errorCallback(ec);
                        }

                        boost::ignore_unused(bytesTransferred);
                        const std::string json = boost::beast::buffers_to_string(pBuffer->data());
                        pThis->_connectionCallback(json);
                        pThis->readNext();
                    });
        }
    }

    void WsClient::onRead(boost::beast::error_code errorCode, std::shared_ptr<boost::beast::flat_buffer> pData, std::size_t bytesTransferred) {
        if (errorCode) {
            return _errorCallback(errorCode);
        }

        boost::ignore_unused(bytesTransferred);

        const std::string json = boost::beast::buffers_to_string(pData->data());
        _receiverCallback(json);

        readNext();
    }

    void WsClient::onWrite(boost::beast::error_code errorCode, std::size_t bytesTransferred) {
        if (errorCode) {
            return _errorCallback(errorCode);
        }

        boost::ignore_unused(bytesTransferred);
    }

    void WsClient::onClose(boost::beast::error_code errorCode) {
        if (errorCode) {
            return _errorCallback(errorCode);
        } else {
            std::cout << "websocket: connection is closed gracefully" << std::endl;
        }
    }

    void WsClient::readNext() {
        std::shared_ptr<boost::beast::flat_buffer> pBuffer(new boost::beast::flat_buffer);
        _ws.async_read(
                *pBuffer,
                [pThis = shared_from_this(), pBuffer](boost::beast::error_code errorCode,
                                                      std::size_t bytesTransferred) {
                    pThis->onRead(errorCode, pBuffer, bytesTransferred);
                });
    }
}