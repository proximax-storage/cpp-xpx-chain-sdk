#pragma once


#include <vector>
#include "account_info.h"


namespace xpx_chain_sdk {
    class MultipleAccountInfo {
    public:
        std::vector<AccountInfo> accountInfos;
    };
}