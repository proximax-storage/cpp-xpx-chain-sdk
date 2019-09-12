/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <string>
#include <infrastructure/json/uint64.h>
#include <boost/property_tree/ptree.hpp>
#include "infrastructure/utils/variadic_struct.h"

#include <stdint.h>
namespace xpx_chain_sdk::internal::json::dto {

    using xpx_chain_sdk::internal::json::Uint64;
    using HeightDto = VariadicStruct<Field<STR_LITERAL("height"), Uint64> >;

}
