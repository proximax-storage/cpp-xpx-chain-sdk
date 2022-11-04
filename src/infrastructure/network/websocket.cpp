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
            std::shared_ptr<boost::asio::strand<boost::asio::io_context::executor_type>> strand,
            Callback connectionCallback,
            Callback receiverCallback,
            ErrorCallback errorCallback) :
            _config(config),
            _context(context),
            _buffer(std::make_shared<boost::asio::streambuf>()),
            _outgoingQueue(),
            _strand(strand),
            _resolver(boost::asio::ip::tcp::resolver(*_strand)),
            _ws(*_strand),
            _connectionCallback(connectionCallback),
            _receiverCallback(receiverCallback),
            _errorCallback(errorCallback) {}

    void WsClient::connect(uint64_t onResolveHostTimeoutSec) {
        boost::asio::post(*_strand, [pThis = shared_from_this(), onResolveHostTimeoutSec] {
            pThis->_resolver.async_resolve(pThis->_config->nodeAddress,
                                           pThis->_config->port,
                                           boost::asio::bind_executor(*pThis->_strand, [pThis, onResolveHostTimeoutSec] (
                                                   boost::beast::error_code errorCode,
                                                   const boost::asio::ip::tcp::resolver::results_type &resultsType) {
                                               pThis->onResolve(errorCode, resultsType, onResolveHostTimeoutSec);
                                           }));
        });
    }

    void WsClient::disconnect() {
        boost::asio::post(*_strand, [pThis = shared_from_this()] {
            auto disconnectTask = [pThis]() {
                if (pThis->isConnected()) {
                    pThis->_ws.async_close(boost::beast::websocket::close_code::normal,
                                           boost::asio::bind_executor(*pThis->_strand, [pThis](auto errorCode) {
                                               pThis->onClose(errorCode);
                                           }));
                }
            };

            if (pThis->_outgoingQueue.empty()) {
                disconnectTask();
            } else {
                pThis->_postponedDisconnect = disconnectTask;
            }
        });
    }

    void WsClient::send(const std::string& json, std::function<void()> onSuccess,
                        std::function<void(boost::beast::error_code errorCode)> onError) {
        boost::asio::post(*_strand, [pThis = shared_from_this(), json, onSuccess, onError] {
            pThis->_outgoingQueue.push_back({ json, { onSuccess, onError } });
            if (pThis->_outgoingQueue.size() > 1) {
                return;
            }

            pThis->doWrite();
        });
    }

    bool WsClient::isConnected() const {
        return _ws.is_open();
    }

    void WsClient::doWrite() {
        _ws.async_write(
                boost::asio::buffer(_outgoingQueue[0].first),
                boost::asio::bind_executor(*_strand, [pThis = shared_from_this()](auto errorCode, auto bytesTransferred) {
                    if (errorCode) {
                        boost::asio::post(*pThis->_strand, [errorCode, onError = pThis->_outgoingQueue[0].second.second]() {
                            if (onError) {
                                onError(errorCode);
                            }
                        });

                        return pThis->_outgoingQueue.pop_front();
                    }

                    boost::asio::post(*pThis->_strand, [onSuccess = pThis->_outgoingQueue[0].second.first]() {
                        if (onSuccess) {
                            onSuccess();
                        }
                    });

                    pThis->_outgoingQueue.pop_front();
                    if (!pThis->_outgoingQueue.empty()) {
                        pThis->doWrite();
                        return;
                    }

                    if (pThis->_postponedDisconnect) {
                        pThis->_postponedDisconnect();
                    }
                }));
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
                resultsType, boost::asio::bind_executor(*_strand, [pThis = shared_from_this()] (auto ec, const auto& et) {
                    pThis->onConnect(ec, et);
                }));
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
        _ws.async_handshake(host, _config->baseWsPath, boost::asio::bind_executor(*_strand, [pThis = shared_from_this()](auto ec) {
            pThis->onHandshake(ec);
        }));
    }

    void WsClient::onHandshake(boost::beast::error_code errorCode) {
        if (errorCode) {
            return _errorCallback(errorCode);
        } else {
            _ws.async_read(
                    *_buffer,
                    boost::asio::bind_executor(*_strand, [pThis = shared_from_this()] (
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

    void WsClient::onRead(boost::beast::error_code errorCode) {
        // If socket is closed last async_read operation aborted
        if (boost::asio::error::operation_aborted == errorCode) {
            return;
        }

        if (errorCode) {
            return _errorCallback(errorCode);
        }

        const std::string json = boost::beast::buffers_to_string(_buffer->data());
        _receiverCallback(json);

        readNext();
    }

    void WsClient::onClose(boost::beast::error_code errorCode) {
        boost::beast::get_lowest_layer(_ws).close();
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
                boost::asio::bind_executor(*_strand, [pThis = shared_from_this()](
                        boost::beast::error_code errorCode, std::size_t) {
                    pThis->onRead(errorCode);
                }));
    }
}