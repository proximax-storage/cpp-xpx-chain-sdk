/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once


#include "account_info_dto.h"
#include <xpxchaincpp/model/account/multisig_graph.h>

namespace xpx_chain_sdk::internal::json::dto {

    using MultisigLevelDto = VariadicStruct<
            Field<STR_LITERAL("level"), uint32_t >,
            Field<STR_LITERAL("multisigEntries"), std::vector<MultisigInfoDto> > >;

    using MultisigGraphDto = std::vector<MultisigLevelDto>;


}