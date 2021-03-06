/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/
#pragma once

#include <ctype.h>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector> // we can use struct
#include <sstream>
#include "xpxchaincpp/crypto/private_key.h"
#include "xpxchaincpp/crypto/key_pair.h"
#include "xpxchaincpp/crypto/signer.h"
#include "xpxchaincpp/types.h"
#include "xpxchaincpp/utils/base32.h"
#include <xpxchaincpp/exceptions.h>

namespace xpx_chain_sdk {
    using PublicKey = Key;

    using ByteArray = std::vector<uint8_t>;

    enum MessageType : uint8_t {
        PlainMessageType,
        SecureMessageType
    };


    class Message {
    public:
        virtual MessageType Type() const = 0;

        virtual ByteArray Payload() const = 0;

        virtual std::string String() const = 0;

        virtual ~Message() = default;
    };


    class PlainMessage : public Message {
    public:
        using PlainMessagePtr = std::shared_ptr<PlainMessage>;
    public:
        PlainMessage(const std::string& payload_string) {
            payload = ByteArray(payload.begin(), payload.end());
        }
        ~PlainMessage()=default;
    public:
        MessageType Type() const override {
            return PlainMessageType;
        }

        ByteArray Payload() const override {
            return payload;
        }

        std::string String() const override {
            std::ostringstream result;
            for (auto x : payload) {
                result << (char) x;
            }
            return result.str();
        }

        std::string Message() {
            return String();
        }

    private:
        ByteArray payload;
    };


    std::shared_ptr<PlainMessage> NewPlainMessage(const std::string& payload) {
        return std::make_shared<PlainMessage>(payload);
    }

    std::shared_ptr<PlainMessage> NewPlainMessageFromEncodedData(
            ByteArray encodedData,
            PrivateKey recipient,
            PublicKey sender) {

		/// TODO: Implement this method;
    }


    class SecureMessage : public Message {
    public:
        SecureMessage(ByteArray&& encodedList) {
            encodedData = std::move(encodedList);
        }
        ~SecureMessage()=default;
    public:
        MessageType Type() const override {
            return SecureMessageType;
        }

        ByteArray Payload() const override {
            return encodedData;
        }

        std::string String() const override {
            std::ostringstream result;
            for (auto x : encodedData) {
                result << (char) x;
            }
            return result.str();
        }

        std::string Message() {
            return String();
        }

    private:
        ByteArray encodedData;
    };

    std::shared_ptr<SecureMessage> NewSecureMessage(ByteArray&& encodedData) {
        return std::make_shared<SecureMessage>(std::move(encodedData));
    }

    std::shared_ptr<SecureMessage> NewSecureMessageFromPlaintText(
            const std::string& plaintText,
            PrivateKey sender,
            PublicKey recipient) {


        /// TODO: Implement this method;
    }

    std::string ByteArrayToString(const ByteArray& data ) {
        std::ostringstream result;
        for (auto x : data) {
            result << (char) x;
        }
        return result.str();
    }

    class messageDTO {
    public:
        std::shared_ptr<Message> toStruct() {
            ByteArray b;

            if(Payload.length() != 0) {
                if(!Base32::Decode(Payload, b)) {
                    XPX_CHAIN_SDK_THROW(crypto_error, "Cannot decode Base32");
                }
            }

            switch(Type) {
                case PlainMessageType:
                    return std::shared_ptr<Message>(NewPlainMessage(ByteArrayToString(b)).get());
                case SecureMessageType:
                    return std::shared_ptr<Message>(NewSecureMessage(std::move(b)).get());
                default:
                    XPX_CHAIN_SDK_THROW(message_error, "Invalide message");
            }
        }
    private:
        MessageType Type;
        std::string Payload;
    };
}
