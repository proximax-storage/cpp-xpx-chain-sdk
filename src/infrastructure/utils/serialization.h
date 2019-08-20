#pragma once

#include <infrastructure/json/parser.h>
#include <infrastructure/json/dto/block_dto.h>
#include <infrastructure/json/dto/mosaic_dto.h>
#include <infrastructure/json/dto/namespace_dto.h>
#include <infrastructure/json/dto/height.h>
#include <infrastructure/json/dto/account_info_dto.h>
#include <infrastructure/json/dto/multiple_namespace_dto.h>
#include <infrastructure/json/dto/multiple_blocks_dto.h>
#include <infrastructure/json/dto/account_info_dto.h>
#include <infrastructure/json/dto/multisig_graph_dto.h>
#include <infrastructure/json/dto/network_info_dto.h>
#include <infrastructure/json/dto/transaction_dto.h>

#include <nemcpp/model/blockchain/block.h>
#include <nemcpp/model/blockchain/score.h>
#include <nemcpp/model/blockchain/height.h>
#include <nemcpp/model/blockchain/storage_info.h>
#include <nemcpp/model/blockchain/multiple_block.h>
#include <nemcpp/model/mosaic/multiple_mosaic.h>
#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/account/account_info.h>
#include <nemcpp/model/mosaic/mosaic_name.h>
#include <nemcpp/model/mosaic/mosaic_names.h>
#include <nemcpp/model/namespace/namespace_info.h>
#include <nemcpp/model/namespace/namespace_name.h>
#include <nemcpp/model/namespace/namespace_names.h>
#include <nemcpp/model/namespace/multiple_namespace_info.h>
#include <nemcpp/model/account/account_properties.h>
#include <nemcpp/model/network/network_info.h>
#include <nemcpp/model/transaction_simple/transaction.h>


#include <nemcpp/exceptions.h>
#include <nemcpp/model/account/multiple_account_info.h>
#include <nemcpp/model/transaction_simple/transaction_container.h>
#include <infrastructure/binary/parser.h>


using namespace xpx_sdk::simple_transactions;

using xpx_sdk::internal::binary::Parser;

namespace xpx_sdk { namespace  internal { namespace json {
			namespace dto {
				template<typename Object, typename ObjectDto>
				ObjectDto toDto(const Object &transaction);

				using ByteArray = std::vector<uint8_t>;

				template<typename Object, typename ObjectDto>
				ByteArray to_bytes(const Object& transaction) {
					ObjectDto dto = toDto<Object, ObjectDto>(transaction);
					ByteArray data;
					Parser::Write(dto, data);
					return data;
				}

				template<>
				MosaicDto toDto<Mosaic, MosaicDto>(const Mosaic& mosaic);

				template<>
				CosignatureDto toDto<Cosignature, CosignatureDto>(const Cosignature &transaction);

				template<>
				CosignatoryModificationDto
				toDto<CosignatoryModification, CosignatoryModificationDto>(const CosignatoryModification &transaction);

				template<>
				TransactionPropertyModificationDto
				toDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(
						const AccountTransactionPropertyModification &transaction);

				template<>
				MosaicPropertyModificationDto toDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(
						const AccountMosaicPropertyModification &transaction);

				template<>
				AddressPropertyModificationDto
				toDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(
						const AccountAddressPropertyModification &transaction);

				template<>
				MosaicPropertyDto toDto<MosaicProperty, MosaicPropertyDto>(const MosaicProperty &transaction);

				template<>
				TransactionDto toDto<Transaction, TransactionDto>(const Transaction &transaction);

				template<>
				EmbeddedTransactionDto
				toDto<EmbeddedTransaction, EmbeddedTransactionDto>(const EmbeddedTransaction &transaction);

				template<>
				AggregateTransactionDto
				toDto<AggregateTransaction, AggregateTransactionDto>(const AggregateTransaction &transaction);

				template<>
				AccountLinkTransactionDto
				toDto<AccountLinkTransaction, AccountLinkTransactionDto>(const AccountLinkTransaction &transaction);

				template<>
				LockFundsTransactionDto
				toDto<LockFundsTransaction, LockFundsTransactionDto>(const LockFundsTransaction &transaction);

				template<>
				ModifyMultisigAccountTransactionDto
				toDto<ModifyMultisigAccountTransaction, ModifyMultisigAccountTransactionDto>(
						const ModifyMultisigAccountTransaction &transaction);

				template<>
				MosaicDefinitionTransactionDto toDto<MosaicDefinitionTransaction, MosaicDefinitionTransactionDto>(
						const MosaicDefinitionTransaction &transaction);


				template<>
				MosaicSupplyChangeTransactionDto toDto<MosaicSupplyChangeTransaction, MosaicSupplyChangeTransactionDto>(
						const MosaicSupplyChangeTransaction &transaction);

				template<>
				RegisterNamespaceTransactionDto toDto<RegisterNamespaceTransaction, RegisterNamespaceTransactionDto>(
						const RegisterNamespaceTransaction &transaction);

				template<>
				SecretLockTransactionDto
				toDto<SecretLockTransaction, SecretLockTransactionDto>(const SecretLockTransaction &transaction);

				template<>
				SecretProofTransactionDto
				toDto<SecretProofTransaction, SecretProofTransactionDto>(const SecretProofTransaction &transaction);

				template<>
				TransferTransactionDto
				toDto<TransferTransaction, TransferTransactionDto>(const TransferTransaction &transaction);

				template<>
				AliasTransactionBaseDto
				toDto<AliasTransactionBase, AliasTransactionBaseDto>(const AliasTransactionBase &transaction);

				template<>
				AddressAliasTransactionDto
				toDto<AddressAliasTransaction, AddressAliasTransactionDto>(const AddressAliasTransaction &transaction);

				template<>
				MosaicAliasTransactionDto
				toDto<MosaicAliasTransaction, MosaicAliasTransactionDto>(const MosaicAliasTransaction &transaction);

/// Account Property Transactions
				template<>
				AccountTransactionPropertyTransactionDto
				toDto<AccountTransactionPropertyTransaction, AccountTransactionPropertyTransactionDto>(
						const AccountTransactionPropertyTransaction &transaction);

				template<>
				AccountMosaicPropertyTransactionDto
				toDto<AccountMosaicPropertyTransaction, AccountMosaicPropertyTransactionDto>(
						const AccountMosaicPropertyTransaction &transaction);

				template<>
				AccountAddressPropertyTransactionDto
				toDto<AccountAddressPropertyTransaction, AccountAddressPropertyTransactionDto>(
						const AccountAddressPropertyTransaction &transaction);
			}
		}
	}
}

