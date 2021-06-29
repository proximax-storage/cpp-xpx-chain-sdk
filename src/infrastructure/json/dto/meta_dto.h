/*
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <string>
#include "infrastructure/utils/variadic_struct.h"

namespace xpx_chain_sdk::internal::json::dto {

    using WebsocketMetaDto = VariadicStruct<
            Field<STR_LITERAL("channelName"), std::string>,
            Field<STR_LITERAL("address"), std::string> >;

    using WebsocketInfoDto = VariadicStruct<
            Field<STR_LITERAL("meta"), WebsocketMetaDto>>;
}
