#pragma once


#include "account_info_dto.h"
#include <nemcpp/model/account/multisig_graph.h>

namespace xpx_sdk::internal::json::dto {
    using MultisigLevelDto = VariadicStruct<
            Field<STR_LITERAL("level"), uint32_t >,
            Field<STR_LITERAL("multisigEntries"), std::vector<MultisigInfoDto> > >;

    using MultisigGraphDto = std::vector<MultisigEntrieDto>;


}