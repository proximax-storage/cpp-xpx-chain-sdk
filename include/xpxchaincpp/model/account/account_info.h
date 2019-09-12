#pragma once

#include <string>
#include <xpxchaincpp/model/mosaic/mosaic.h>


namespace xpx_chain_sdk {
    class AccountInfo {
    public:
        std::string address;
        uint64_t addressHeight;
        std::string publicKey;
        uint64_t publicKeyHeight;
        std::vector<Mosaic> mosaics;
    };
}