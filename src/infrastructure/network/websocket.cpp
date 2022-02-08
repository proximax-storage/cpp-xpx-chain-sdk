/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include "websocket.h"
#include <thread>

namespace xpx_chain_sdk::internal::network {

    WsClient::WsClient(
            std::shared_ptr<Config> config,
            std::shared_ptr<internal::network::Context> context,
            std::shared_ptr<boost::asio::io_context> ioContext,
            Callback connectionCallback,
            Callback receiverCallback,
            ErrorCallback errorCallback) :
            _config(config),
            _context(context),
            _buffer(std::make_shared<boost::asio::streambuf>()),
            _io_context(ioContext),
            _outgoingQueue(),
            _isConnected(false),
            _strand(boost::asio::make_strand(_io_context->get_executor())),
            _resolver(boost::asio::ip::tcp::resolver(*_io_context)),
            _ws(_strand),
            _connectionCallback(connectionCallback),
            _receiverCallback(receiverCallback),
            _errorCallback(errorCallback) {}

    WsClient::~WsClient() {
        disconnect();
    }

    void WsClient::connect(uint64_t onResolveHostTimeoutSec) {
        boost::asio::post(_strand, [pThis = shared_from_this(), onResolveHostTimeoutSec] {
            pThis->_resolver.async_resolve(pThis->_config->nodeAddress,
                                           pThis->_config->port,
                                           boost::asio::bind_executor(pThis->_strand, [pThis, onResolveHostTimeoutSec] (
                                                   boost::beast::error_code errorCode,
                                                   const boost::asio::ip::tcp::resolver::results_type &resultsType) {
                                               pThis->onResolve(errorCode, resultsType, onResolveHostTimeoutSec);
                                           }));
        });

        boost::thread thread([pThis = shared_from_this()]() { pThis->_io_context->run(); });
        thread.detach();
    }

    void WsClient::disconnect() {
        boost::asio::post(_strand, [pThis = shared_from_this()] {
            if (pThis->isConnected()) {
                pThis->_ws.async_close(boost::beast::websocket::close_code::normal,
                                       boost::asio::bind_executor(pThis->_strand, [pThis](auto errorCode) {
                                           pThis->onClose(errorCode);
                                       }));
            } else {
                pThis->_isConnected = pThis->_ws.is_open();
            }
        });
    }

    void WsClient::send(const std::string& json) {
        boost::asio::post(_strand, [pThis = shared_from_this(), json] {
            pThis->_outgoingQueue.push_back(json);
            if (pThis->_outgoingQueue.size() > 1) {
                return;
            }

            pThis->doWrite();
        });
    }

    bool WsClient::isConnected() {
        return _isConnected;
    }

    void WsClient::doWrite() {
        _ws.async_write(
                boost::asio::buffer(_outgoingQueue[0]),
                boost::asio::bind_executor(_strand, [pThis = shared_from_this()](auto errorCode, auto bytesTransferred) {
                    pThis->_outgoingQueue.pop_front();
                    if (errorCode) {
                        pThis->_isConnected = pThis->_ws.is_open();
                        return pThis->_errorCallback(errorCode);
                    }

                    if (!pThis->_outgoingQueue.empty()) {
                        pThis->doWrite();
                    }
                }));
    }

    void WsClient::onResolve(
            boost::beast::error_code errorCode,
            const boost::asio::ip::tcp::resolver::results_type& resultsType,
            uint64_t onResolveHostTimeoutSec) {
        if (errorCode) {
            _isConnected = _ws.is_open();
            return _errorCallback(errorCode);
        }

        boost::beast::get_lowest_layer(_ws).expires_after(std::chrono::seconds(onResolveHostTimeoutSec));
        boost::beast::get_lowest_layer(_ws).async_connect(
                resultsType, boost::asio::bind_executor(_strand, [pThis = shared_from_this()] (auto ec, const auto& et) {
                    pThis->onConnect(ec, et);
                }));
    }

    void WsClient::onConnect(
            boost::beast::error_code errorCode,
            const boost::asio::ip::tcp::resolver::results_type::endpoint_type&) {
        if (errorCode) {
            _isConnected = _ws.is_open();
            return _errorCallback(errorCode);
        }

        // Turn off the timeout on the tcp_stream, because
        // the websocket stream has its own timeout system.
        boost::beast::get_lowest_layer(_ws).expires_never();

        // Set suggested timeout settings for the websocket
        _ws.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::client));

        _isConnected = true;

        const std::string host = _config->nodeAddress + ":" + _config->port;

        std::cout << "websocket: connection established: " << host << std::endl;

        // Perform the websocket handshake
        _ws.async_handshake(host, _config->baseWsPath, boost::asio::bind_executor(_strand, [pThis = shared_from_this()](auto ec) {
            pThis->onHandshake(ec);
        }));
    }

    void WsClient::onHandshake(boost::beast::error_code errorCode) {
        if (errorCode) {
            _isConnected = _ws.is_open();
            return _errorCallback(errorCode);
        } else {
            _ws.async_read(
                    *_buffer,
                    boost::asio::bind_executor(_strand, [pThis = shared_from_this()] (
                            boost::beast::error_code ec,
                            std::size_t) {
                        if (ec) {
                            return pThis->_errorCallback(ec);
                        }

                        const std::string json = boost::beast::buffers_to_string(pThis->_buffer->data());
                        pThis->_connectionCallback(json);
                        pThis->readNext();
                    }));
        }
    }

    void WsClient::onRead(boost::beast::error_code errorCode, std::size_t) {
        if (errorCode) {
            _isConnected = _ws.is_open();
            return _errorCallback(errorCode);
        }

        const std::string json = boost::beast::buffers_to_string(_buffer->data());
        _receiverCallback(json);

        readNext();
    }

    void WsClient::onClose(boost::beast::error_code errorCode) {
        _isConnected = _ws.is_open();
        if (errorCode) {
            return _errorCallback(errorCode);
        } else {
            std::cout << "websocket: connection is closed gracefully" << std::endl;
        }
    }

    void WsClient::readNext() {
        _buffer->consume(_buffer->size());
        _ws.async_read(
                *_buffer,
                boost::asio::bind_executor(_strand, [pThis = shared_from_this()](
                        boost::beast::error_code errorCode,
                        std::size_t bytesTransferred) {
                    pThis->onRead(errorCode, bytesTransferred);
                }));
    }
}