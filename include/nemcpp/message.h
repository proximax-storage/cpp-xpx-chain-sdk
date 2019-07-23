//
// Created by vulfke on 18.07.19.
//

#pragma once

#include <ctype.h>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector> // we can use struct
#include <sstream>
#include "nemcpp/crypto/private_key.h"
#include "nemcpp/crypto/key_pair.h"
#include "nemcpp/crypto/signer.h"
#include "nemcpp/types.h"
#include "nemcpp/utils/base32.h"

namespace nem2_sdk {
    using PublicKey = Key;
//    class ByteArray { // we can use vector<uint8_t>
//        uint8_t *array;
//        int size;
//    };

    using ByteArray = std::vector<uint8_t>;

    enum MessageType : uint8_t {
        PlainMessageType,
        SecureMessageType
    };


    class Message {
        virtual MessageType Type() const = 0;

        virtual ByteArray Payload() const = 0;

        virtual std::string String() const = 0;
    };


    class PlainMessage : public Message {
    public:
        using PlainMessagePtr = std::shared_ptr<PlainMessage>;
    public:
        PlainMessage(const std::string& payload_string) {
            for(auto x : payload_string) {
                payload.push_back(x);
            }
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
            std::ostringstream result;
            for (auto x : payload) {
                result << (char) x;
            }
            return result.str();
        }

    private:
        ByteArray payload; //ByteArray = struct { uint8_t* array, int size;}
    };


    std::shared_ptr<PlainMessage> NewPlainMessage(const std::string& payload) {
        return std::make_shared<PlainMessage>(payload);
    }

    std::shared_ptr<PlainMessage> NewPlainMessageFromEncodedData(
            ByteArray encodedData,
            PrivateKey recipient,
            PublicKey sender) {

        // TODO
    }


    class SecureMessage : public Message {
    public:
        SecureMessage(const ByteArray& encodedList) {
            encodedData.resize(encodedList.size());
            std::copy(encodedList.begin(), encodedList.end(), encodedData.begin());
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
            std::ostringstream result;
            for (auto x : encodedData) {
                result << (char) x;
            }
            return result.str();
        }

    private:
        ByteArray encodedData; //ByteArray = struct { uint8_t* array, int size;}
    };

    std::shared_ptr<SecureMessage> NewSecureMessage(const ByteArray& encodedData) {
        return std::make_shared<SecureMessage>(encodedData);
    }

    std::shared_ptr<SecureMessage> NewSecureMessageFromPlaintText(
            const std::string& plaintText,
            PrivateKey sender,
            PublicKey recipient) {


        // TODO
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
                    throw "Hex Decode Error";  // TODO:change to custom exception
                }
            }

            switch(Type) {
                case PlainMessageType:
                    return std::shared_ptr<Message>(NewPlainMessage(ByteArrayToString(b)).get());
                case SecureMessageType:
                    return std::shared_ptr<Message>(NewSecureMessage(b).get());
                default:
                    throw "Invalide Message Type Error";  // TODO:change to custom exception
            }
        }
    private:
        MessageType Type;
        std::string Payload;
    };
}
