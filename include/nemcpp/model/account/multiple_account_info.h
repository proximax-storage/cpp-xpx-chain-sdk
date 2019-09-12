#pragma once


#include <vector>
#include "account_info.h"


namespace xpx_sdk {
    class MultipleAccountInfo {
    public:
        std::vector<AccountInfo> accountInfos;
    };
}