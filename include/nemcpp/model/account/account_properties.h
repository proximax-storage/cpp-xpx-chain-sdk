#pragma once

#include "account_property.h"
#include <vector>


namespace xpx_sdk {
    class MultipleAccountProperty {
    public:
        std::vector<AccountProperties> accountProperties;
    };
}