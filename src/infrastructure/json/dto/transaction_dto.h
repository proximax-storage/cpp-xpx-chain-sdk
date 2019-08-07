/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/
#pragma once

#include "infrastructure/utils/variadic_struct.h"
#include "infrastructure/json/uint64.h"

#include <string>
#include <stdint.h>
#include <array>
#include <vector>
#include <nemcpp/types.h>
#include <nemcpp/model/transaction_simple/transaction_type.h>
#include <nemcpp/model/transaction_simple/account_link_transaction_types.h>
#include <nemcpp/model/transaction_simple/modify_multisig_account_transaction_types.h>
#include <nemcpp/model/transaction_simple/mosaic_supply_change_transaction_types.h>
#include <nemcpp/model/namespace/namespace.h>
#include <nemcpp/model/transaction_simple/secret_hash_algorithm.h>
#include <nemcpp/model/transaction_simple/account_property_transaction_types.h>
#include "mosaic_dto.h"

namespace xpx_sdk::internal::json::dto {
    using xpx_sdk::internal::json::Uint64;

    using MosaicPropertyDto = std::array<uint64_t , 2>;

    using AccountPropertyModificationDTO = VariadicStruct<
            Field<STR_LITERAL("value"),            std::string>,
            Field<STR_LITERAL("modificationType"), AccountPropertyModificationType>>;

    using CosignatureDTO = VariadicStruct<
            Field<STR_LITERAL("publicKey"), Key>,
            Field<STR_LITERAL("signature"), Signature>>;

    using CosignatoryModificationDTO = VariadicStruct<
            Field<STR_LITERAL("publicKey"),        Key>,
            Field<STR_LITERAL("modificationType"), CosignatoryModificationType>>;

    using TransactionMetaDto = VariadicStruct<
            Field<STR_LITERAL("height"), Uint64>,
            Field<STR_LITERAL("hash"), std::string>,
            Field<STR_LITERAL("merkleComponentHash"), std::string>,
            Field<STR_LITERAL("index"), uint64_t >,
            Field<STR_LITERAL("id"), std::string> >;


    using GeneralTransactionDto = VariadicStruct<
            Field<STR_LITERAL("size"),                    uint32_t>,
            Field<STR_LITERAL("signature"),               Signature>,
            Field<STR_LITERAL("signer"),                  Key>,
            Field<STR_LITERAL("version"),                 uint16_t>,
            Field<STR_LITERAL("type"),                    TransactionType>,
            Field<STR_LITERAL("maxFee"),                  Amount>,
            Field<STR_LITERAL("deadline"),                int64_t>,
            Field<STR_LITERAL("payloadSize"),             uint32_t>,
            Field<STR_LITERAL("payload"),                 std::vector<uint8_t> >,
            Field<STR_LITERAL("cosignatures"),            std::vector<CosignatureDTO> >,
            Field<STR_LITERAL("linkAction"),              AccountLinkTransactionAction>,
            Field<STR_LITERAL("remoteAccountKey"),        Key>,
            Field<STR_LITERAL("lockedMosaic"),            MosaicDto>,
            Field<STR_LITERAL("lockDuration"),            BlockDuration>,
            Field<STR_LITERAL("lockHash"),                Hash256>,
            Field<STR_LITERAL("minRemovalDelta"),         int8_t>,
            Field<STR_LITERAL("minApprovalDelta"),        int8_t>,
            Field<STR_LITERAL("modificationsCount"),      uint8_t>,
            Field<STR_LITERAL("modifications"),           std::vector<CosignatoryModificationDTO>> ,
            Field<STR_LITERAL("nonce"),                   uint32_t>,
            Field<STR_LITERAL("mosaicId"),                MosaicId>,
            Field<STR_LITERAL("optionalPropertiesCount"), uint8_t>,
            Field<STR_LITERAL("flags"),                   MosaicFlags>,
            Field<STR_LITERAL("divisibility"),            uint8_t>,
            Field<STR_LITERAL("optionalProperties"),      std::vector<MosaicPropertyDto> >,
            Field<STR_LITERAL("mosaicId"),                MosaicId>,
            Field<STR_LITERAL("direction"),               MosaicSupplyChangeDirection>,
            Field<STR_LITERAL("delta"),                   Amount>,
            Field<STR_LITERAL("namespaceType"),           NamespaceType>,
            Field<STR_LITERAL("durationOrParentId"),      uint64_t>,
            Field<STR_LITERAL("namespaceId"),             NamespaceId>,
            Field<STR_LITERAL("namespaceNameSize"),       uint8_t>,
            Field<STR_LITERAL("namespaceName"),           std::string>,
            Field<STR_LITERAL("mosaicId"),                MosaicId>,
            Field<STR_LITERAL("amount"),                  Amount>,
            Field<STR_LITERAL("duration"),                BlockDuration>,
            Field<STR_LITERAL("hashAlgorithm"),           SecretHashAlgorithm>,
            Field<STR_LITERAL("secret"),                  Hash256>,
            Field<STR_LITERAL("recipient"),               AddressData>,
            Field<STR_LITERAL("hashAlgorithm"),           SecretHashAlgorithm>,
            Field<STR_LITERAL("secret"),                  Hash256>,
            Field<STR_LITERAL("proofSize"),               uint16_t>,
            Field<STR_LITERAL("proof"),                   std::vector<uint8_t> >,
            Field<STR_LITERAL("recipient"),               AddressData>,
            Field<STR_LITERAL("messageSize"),             uint16_t>,
            Field<STR_LITERAL("mosaicsCount"),            uint8_t>,
            Field<STR_LITERAL("message"),                 std::vector<uint8_t> >,
            Field<STR_LITERAL("mosaics"),                 std::vector<MosaicDto> >,
            Field<STR_LITERAL("namespaceId"),             NamespaceId>,
            Field<STR_LITERAL("address"),                 AddressData>,
            Field<STR_LITERAL("mosaicId"),                MosaicId>,
            Field<STR_LITERAL("propertyType"),            uint8_t>,
            Field<STR_LITERAL("modificationsCount"),      uint8_t>,
            Field<STR_LITERAL("modifications"),           std::vector<AccountPropertyModificationDTO> > >;

}

