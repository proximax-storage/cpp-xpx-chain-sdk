/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <infrastructure/utils/variadic_struct.h>

namespace xpx_chain_sdk::internal::json::dto {
    using AccountNameDto = VariadicStruct<
            Field<STR_LITERAL("address"), std::string >,
            Field<STR_LITERAL("names"), std::vector<std::string> > >;

    using AccountNamesDto = std::vector<AccountNameDto>;
}