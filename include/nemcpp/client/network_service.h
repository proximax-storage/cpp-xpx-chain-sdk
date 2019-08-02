#pragma once

#include <nemcpp/model/network/network_info.h>

namespace xpx_sdk {
    class INetworkService {
    public:
        virtual NetworkInfo getNetworkInfo();
    };
}