#pragma once
#include <string>
#include <nemcpp/model/mosaic/mosaic.h>


namespace nem2_sdk {
    class AccountInfo {
    public:
        std::string address;
        uint64_t addressHeight;
        std::string publicKey;
        uint64_t publicKeyHeight;
        std::vector<Mosaic> mosaics;
    };
}