/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "deserialization_json.h"

using namespace xpx_chain_sdk::internal::json;
using namespace xpx_chain_sdk::internal::json::dto;

using namespace xpx_chain_sdk::transactions_info;


#define EXTRACT_TRANSACTION(transaction, dto) \
transaction.signature = dto.value<"signature"_>(); \
transaction.signer = dto.value<"signer"_>(); \
transaction.version = dto.value<"version"_>(); \
transaction.type = dto.value<"type"_>(); \
transaction.maxFee = dto.value<"maxFee"_>(); \
transaction.deadline = dto.value<"deadline"_>();

#define EXTRACT_EMBEDDED_TRANSACTION(transaction, dto) \
transaction.signer = dto.value<"signer"_>(); \
transaction.version = dto.value<"version"_>(); \
transaction.type = dto.value<"type"_>();

namespace xpx_chain_sdk::internal::json::dto {


	std::vector<std::string> parseJsonArray(const std::string& jsonStr, const char lbrace = '{', const char rbrace = '}') {
		std::vector<std::string> result;
		int balance = 0;
		for (auto x : jsonStr) {
			if (x == lbrace) {
				if (balance == 0) {
					result.push_back(std::string());
				}
				balance += 1;
			}
			if (balance) {
				result.back().push_back(x);
			}
			if (x == rbrace) {
				balance -= 1;
			}
		}
		return result;
	}

	TransactionContainer transactions_from_json(const std::string& jsonStr) {
		auto dto = parseJsonArray(jsonStr);
		return fromDto<TransactionContainer, TransactionContainerDto>(dto);
	}

	template<>
	TransactionContainer fromDto<TransactionContainer, TransactionContainerDto>(const TransactionContainerDto& dto) {
		TransactionContainer result;

		for(std::string transactionJson : dto) {
			result.Add(transaction_from_json(transactionJson));
		}

		return result;
	}

	AggregateTransaction aggregateTransactionFromJson(const std::string& jsonStr) {
		AggregateTransaction transaction;

		std::string aggregatedTransactionsJson = parseJsonArray(jsonStr, '[', ']')[0];
		auto transactionContainerDto = parseJsonArray(aggregatedTransactionsJson, '{', '}');
		transaction.transactions = fromDto<TransactionContainer, TransactionContainerDto>(transactionContainerDto);

		VariadicStruct<Field<STR_LITERAL("transaction"), AggregateTransactionDto> > tmp_dto;
		Parser::Read(tmp_dto, jsonStr);
		AggregateTransactionDto dto = tmp_dto.value<"transaction"_>();

		EXTRACT_TRANSACTION(transaction, dto)

		for(const auto& cosignaturesDto : dto.value<"cosignatures"_>()) {
			transaction.cosignatures.push_back(fromDto<Cosignature, CosignatureDto>(cosignaturesDto));
		}

		return transaction;

	}

	std::shared_ptr<transactions_info::BasicTransaction> transaction_from_json(const std::string& jsonStr) {

		VariadicStruct<Field<STR_LITERAL("transaction"), BasicTransactionDto> > dto;
		Parser::Read(dto, jsonStr);

		TransactionContainer transactions;
		TransactionType type = dto.value<"transaction"_>().value<"type"_>();

		std::shared_ptr<transactions_info::BasicTransaction> result = nullptr;
		switch(type) {

			case TransactionType::Transfer: {
				VariadicStruct<Field<STR_LITERAL("transaction"), TransferTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<TransferTransaction, TransferTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<TransferTransaction>(transaction);
				break;
			}

			case TransactionType::Mosaic_Definition: {
				VariadicStruct<Field<STR_LITERAL("transaction"), MosaicDefinitionTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<MosaicDefinitionTransaction, MosaicDefinitionTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<MosaicDefinitionTransaction>(transaction);
				break;
			}

			case TransactionType::Mosaic_Supply_Change: {
				VariadicStruct<Field<STR_LITERAL("transaction"), MosaicSupplyChangeTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<MosaicSupplyChangeTransaction, MosaicSupplyChangeTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<MosaicSupplyChangeTransaction>(transaction);
				break;
			}

			case TransactionType::Register_Namespace: {
				VariadicStruct<Field<STR_LITERAL("transaction"), RegisterNamespaceTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<RegisterNamespaceTransaction, RegisterNamespaceTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<RegisterNamespaceTransaction>(transaction);
				break;
			}

			case TransactionType::Address_Alias: {
				VariadicStruct<Field<STR_LITERAL("transaction"), AddressAliasTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<AddressAliasTransaction, AddressAliasTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<AddressAliasTransaction>(transaction);
				break;
			}

			case TransactionType::Mosaic_Alias: {
				VariadicStruct<Field<STR_LITERAL("transaction"), MosaicAliasTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<MosaicAliasTransaction, MosaicAliasTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<MosaicAliasTransaction>(transaction);
				break;
			}

			case TransactionType::Modify_Multisig_Account: {
				VariadicStruct<Field<STR_LITERAL("transaction"), ModifyMultisigAccountTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<ModifyMultisigAccountTransaction, ModifyMultisigAccountTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<ModifyMultisigAccountTransaction>(transaction);
				break;
			}

			case TransactionType::Aggregate_Complete: {
				auto transaction = aggregateTransactionFromJson(jsonStr);
				result = std::make_shared<AggregateTransaction>(transaction);
				break;
			}

			case TransactionType::Aggregate_Bonded: {
				VariadicStruct<Field<STR_LITERAL("transaction"), AggregateTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<AggregateTransaction, AggregateTransactionDto>(
						t_dto.value<"transaction"_>()); /// Don't know the difference between this two, so need to check.
				result = std::make_shared<AggregateTransaction>(transaction);
				break;
			}

			case TransactionType::Lock_Funds: {
				VariadicStruct<Field<STR_LITERAL("transaction"), LockFundsTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<LockFundsTransaction, LockFundsTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<LockFundsTransaction>(transaction);
				break;
			}

			case TransactionType::Address_Property: {
				VariadicStruct<Field<STR_LITERAL("transaction"), AccountAddressPropertyTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<AccountAddressPropertyTransaction, AccountAddressPropertyTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<AccountAddressPropertyTransaction>(transaction);
				break;
			}

			case TransactionType::Mosaic_Property: {
				VariadicStruct<Field<STR_LITERAL("transaction"), AccountMosaicPropertyTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<AccountMosaicPropertyTransaction, AccountMosaicPropertyTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<AccountMosaicPropertyTransaction>(transaction);
				break;
			}

			case TransactionType::Transaction_Property: {
				VariadicStruct<Field<STR_LITERAL("transaction"), AccountTransactionPropertyTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}
				auto transaction = fromDto<AccountTransactionPropertyTransaction, AccountTransactionPropertyTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<AccountTransactionPropertyTransaction>(transaction);
				break;
			}

			case TransactionType::Secret_Lock: {
				VariadicStruct<Field<STR_LITERAL("transaction"), SecretLockTransactionDto> > t_dto;

				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<SecretLockTransaction, SecretLockTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<SecretLockTransaction>(transaction);
				break;
			}

			case TransactionType::Secret_Proof: {
				VariadicStruct<Field<STR_LITERAL("transaction"), SecretProofTransactionDto> > t_dto;

				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<SecretProofTransaction, SecretProofTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<SecretProofTransaction>(transaction);
				break;
			}

			case TransactionType::Account_Link: {
				VariadicStruct<Field<STR_LITERAL("transaction"), AccountLinkTransactionDto> > t_dto;

				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<AccountLinkTransaction, AccountLinkTransactionDto>(
						t_dto.value<"transaction"_>());
				result = std::make_shared<AccountLinkTransaction>(transaction);
				break;
			}

			case TransactionType::Mosaic_Levy_Change: {
				XPX_CHAIN_SDK_THROW(serialization_error, "Mosaic levy transaction deserialization is not implemented");
			}

			case TransactionType::Prepare_Bc_Drive: {
				VariadicStruct<Field<STR_LITERAL("transaction"), PrepareBcDriveTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<PrepareBcDriveTransaction, PrepareBcDriveTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<PrepareBcDriveTransaction>(transaction);
				break;
			}

			case TransactionType::Data_Modification: {
				VariadicStruct<Field<STR_LITERAL("transaction"), DataModificationTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<DataModificationTransaction, DataModificationTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<DataModificationTransaction>(transaction);
				break;
			}

			case TransactionType::Download: {
				VariadicStruct<Field<STR_LITERAL("transaction"), DownloadTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<DownloadTransaction, DownloadTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<DownloadTransaction>(transaction);
				break;
			}

			case TransactionType::Data_Modification_Approval: {
				VariadicStruct<Field<STR_LITERAL("transaction"), DataModificationApprovalTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<DataModificationApprovalTransaction, DataModificationApprovalTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<DataModificationApprovalTransaction>(transaction);
				break;
			}

			case TransactionType::Data_Modification_Cancel: {
				VariadicStruct<Field<STR_LITERAL("transaction"), DataModificationCancelTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<DataModificationCancelTransaction, DataModificationCancelTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<DataModificationCancelTransaction>(transaction);
				break;
			}

			case TransactionType::Replicator_Onboarding: {
				VariadicStruct<Field<STR_LITERAL("transaction"), ReplicatorOnboardingTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<ReplicatorOnboardingTransaction, ReplicatorOnboardingTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<ReplicatorOnboardingTransaction>(transaction);
				break;
			}

			case TransactionType::Replicator_Offboarding: {
				VariadicStruct<Field<STR_LITERAL("transaction"), ReplicatorOffboardingTransactionDto> > t_dto;
				auto err = Parser::Read(t_dto, jsonStr);
				if (!err) {
					XPX_CHAIN_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", err.invalidField());
				}

				auto transaction = fromDto<ReplicatorOffboardingTransaction, ReplicatorOffboardingTransactionDto>(t_dto.value<"transaction"_>());
				result = std::make_shared<ReplicatorOffboardingTransaction>(transaction);
				break;
			}

			case TransactionType::Unknown: {
				XPX_CHAIN_SDK_THROW(serialization_error, "Transaction type unknown");
			}
		};
		return result;
	}
    template<>
    BlockMeta fromDto<BlockMeta, BlockMetaDto>(const BlockMetaDto &dto) {
        BlockMeta meta;
        meta.hash = dto.value<"hash"_>();
        meta.generationHash = dto.value<"generationHash"_>();
        meta.totalFee = dto.value<"totalFee"_>();
        meta.numTransactions = dto.value<"numTransactions"_>();
        return meta;
    }

    template<>
    BlockData fromDto<BlockData, BlockDataDto>(const BlockDataDto &dto) {
        BlockData blockData;
        blockData.signature = dto.value<"signature"_>();
        blockData.timestamp = dto.value<"timestamp"_>();
        blockData.difficulty = dto.value<"difficulty"_>();
        blockData.feeMultiplier = dto.value<"feeMultiplier"_>();
        blockData.previousBlockHash = dto.value<"previousBlockHash"_>();
        blockData.blockTransactionsHash = dto.value<"blockTransactionsHash"_>();
        blockData.blockReceiptsHash = dto.value<"blockReceiptsHash"_>();
        blockData.stateHash = dto.value<"stateHash"_>();
        blockData.beneficiaryPublicKey = dto.value<"beneficiary"_>();
        return blockData;
    }

    template<>
    Block fromDto<Block, BlockDto>(const BlockDto &dto) {
        Block block;
        block.meta = fromDto<BlockMeta, BlockMetaDto>(dto.value<"meta"_>());
        block.data = fromDto<BlockData, BlockDataDto>(dto.value<"block"_>());
        return block;
    }

    template<>
    StorageInfo fromDto<StorageInfo, StorageInfoDto>(const StorageInfoDto &dto) {

        StorageInfo storageInfo = {
                dto.value<"numBlocks"_>(),
                dto.value<"numTransactions"_>(),
                dto.value<"numAccounts"_>()
        };

        return storageInfo;
    }

    template<>
    ScoreInfo fromDto<ScoreInfo, ScoreInfoDto>(const ScoreInfoDto &dto) {
        ScoreInfo scoreInfo = {
                dto.value<"scoreHigh"_>(),
                dto.value<"scoreLow"_>()
        };

        return scoreInfo;
    }

    template<>
    Height fromDto<Height, HeightDto>(const HeightDto &dto) {
        Height height = {
                dto.value<"height"_>()
        };
        return height;
    }

    template<>
    Mosaic fromDto<Mosaic, MosaicDto>(const MosaicDto &dto) {
        return {dto.value<"id"_>(), dto.value<"amount"_>()};
    }

    template<>
    MosaicMeta fromDto<MosaicMeta, MosaicMetaDto>(const MosaicMetaDto &dto) {
        MosaicMeta mosaicMeta = {
                dto.value<"id"_>()
        };

        return mosaicMeta;
    }

    template<>
    MosaicData fromDto<MosaicData, MosaicDataDto>(const MosaicDataDto &dto) {
        MosaicData mosaicData;
        mosaicData.mosaicId = dto.value<"mosaicId"_>();
        mosaicData.amount = dto.value<"supply"_>();
        mosaicData.height = dto.value<"height"_>();
        mosaicData.owner = dto.value<"owner"_>();
        for (auto& property : dto.value<"properties"_>()) {
            xpx_chain_sdk::MosaicProperty tmp = {MosaicPropertyId(property.value<"id"_>()), property.value<"value"_>()};
            mosaicData.properties.push_back(tmp);
        }
        return mosaicData;
    }

    template<>
    MosaicInfo fromDto<MosaicInfo, MosaicInfoDto>(const MosaicInfoDto &dto) {
        MosaicInfo mosaicInfo = {
                fromDto<MosaicMeta, MosaicMetaDto>(dto.value<"meta"_>()),
                fromDto<MosaicData, MosaicDataDto>(dto.value<"mosaic"_>())
        };
        return mosaicInfo;
    }

    template<>
    MultipleMosaicInfo fromDto<MultipleMosaicInfo, MultipleMosaicInfoDto>(const MultipleMosaicInfoDto &dto) {
        MultipleMosaicInfo mmdto;

        for (auto &mosaicDto: dto) {
            MosaicInfo mosaic = fromDto<MosaicInfo, MosaicInfoDto>(mosaicDto);
            mmdto.mosaics.push_back(mosaic);
        }

        return mmdto;
    }

    template<>
    MosaicName fromDto<MosaicName, MosaicNameDto>(const MosaicNameDto &dto) {
        MosaicName mosaicName = {
                dto.value<"mosaicId"_>(),
                dto.value<"names"_>()
        };
        return mosaicName;
    }

    template<>
    MosaicNames fromDto<MosaicNames, MosaicNamesDto>(const MosaicNamesDto &dto) {
        MosaicNames names;
        for (auto &nameDto: dto) {
            MosaicName mosaic = fromDto<MosaicName, MosaicNameDto>(nameDto);
            names.names.push_back(mosaic);
        }

        return names;
    }

    template<>
    Alias fromDto<Alias, AliasDto>(const AliasDto &dto) {

        Alias alias = {
                dto.value<"type"_>(),
                dto.value<"mosaicId"_>(),
                dto.value<"address"_>()
        };
        return alias;
    }

    template<>
    NamespaceMeta fromDto<NamespaceMeta, NamespaceMetaDto>(const NamespaceMetaDto &dto) {
        NamespaceMeta namespaceMeta = {
                dto.value<"active"_>(),
                dto.value<"index"_>(),
                dto.value<"id"_>()
        };
        return namespaceMeta;
    }

    template<>
    NamespaceData fromDto<NamespaceData, NamespaceDataDto>(const NamespaceDataDto &dto) {
        NamespaceData namespaceData = {
                dto.value<"owner"_>(),
                dto.value<"ownerAddress"_>(),
                dto.value<"startHeight"_>(),
                dto.value<"endHeight"_>(),
                dto.value<"depth"_>(),
                dto.value<"level0"_>(),
                dto.value<"level1"_>(),
                dto.value<"level2"_>(),
                dto.value<"type"_>(),
                fromDto<Alias, AliasDto>(dto.value<"alias"_>()),
                dto.value<"parentId"_>()
        };
        return namespaceData;
    }

    template<>
    NamespaceInfo fromDto<NamespaceInfo, NamespaceInfoDto>(const NamespaceInfoDto &dto) {
        NamespaceInfo namespaceInfo = {
                fromDto<NamespaceMeta, NamespaceMetaDto>(dto.value<"meta"_>()),
                fromDto<NamespaceData, NamespaceDataDto>(dto.value<"namespace"_>())
        };
        return namespaceInfo;
    }

    template<>
    MultipleNamespaceInfo fromDto<MultipleNamespaceInfo, MultipleNamespaceDto>(const MultipleNamespaceDto &dto) {
        MultipleNamespaceInfo namespaceInfos;
        for (auto &nInfosDto : dto) {
            NamespaceInfo namespaceInfo = fromDto<NamespaceInfo, NamespaceInfoDto>(nInfosDto);
            namespaceInfos.namespaceInfos.push_back(namespaceInfo);
        }
        return namespaceInfos;
    }


    template<>
    NamespaceName fromDto<NamespaceName, NamespaceNameDto>(const NamespaceNameDto &dto) {
        NamespaceName namespaceName = {
                dto.value<"name"_>()
        };
        return namespaceName;
    }

    template<>
    NamespaceNames fromDto<NamespaceNames, NamespaceNamesDto>(const NamespaceNamesDto &dto) {
        NamespaceNames names;

        for (auto &nameT : dto) {
            names.names.push_back(fromDto<NamespaceName, NamespaceNameDto>(nameT));
        }
        return names;
    }

    template<>
    MultipleBlock fromDto<MultipleBlock, MultipleBlocksDto>(const MultipleBlocksDto &dto) {
        MultipleBlock mbdto;

        for (auto &blockDto: dto) {
            Block block = fromDto<Block, BlockDto>(blockDto);
            mbdto.blocks.push_back(block);
        }

        return mbdto;
    }

    // Accounts

    template<>
    Property fromDto<Property, PropertyDto>(const PropertyDto& dto) {
        Property property = {
                dto.value<"propertyType"_>(),
                dto.value<"values"_>()
        };
        return property;
    }

    template<>
    AccountProperties fromDto<AccountProperties, AccountPropertyDto>(const AccountPropertyDto& dto) {
        AccountProperties accountProperty;
        accountProperty.address = dto.value<"address"_>();
        for(auto & pdto : dto.value<"properties"_>()) {
            accountProperty.properties.push_back(fromDto<Property, PropertyDto>(pdto));
        }
        return accountProperty;
    }

    template<>
    MultipleAccountProperty fromDto<MultipleAccountProperty, MultipleAccountPropertyDto>(const MultipleAccountPropertyDto& dto) {
        MultipleAccountProperty multipleAccountProperty;
        for(auto & apdto : dto ){
            multipleAccountProperty.accountProperties.push_back(fromDto<AccountProperties, AccountPropertyDto>(apdto));
        }
        return multipleAccountProperty;
    }

    template<>
    MultisigInfo fromDto<MultisigInfo, MultisigInfoDto> (const MultisigInfoDto& dto) {
        MultisigInfo multisigInfo = {
                dto.value<"account"_>(),
                dto.value<"accountAddress"_>(),
                dto.value<"minApproval"_>(),
                dto.value<"minRemoval"_>(),
                dto.value<"cosignatories"_>(),
                dto.value<"multisigAccounts"_>()
        };
        return multisigInfo;
    }

    template<>
    AccountInfo fromDto<AccountInfo, AccountInfoDto> (const AccountInfoDto& dto) {
        AccountInfo accountInfo;
		auto accountDto = dto.value<"account"_>();
        accountInfo.address =  accountDto.value<"address"_>();
        accountInfo.addressHeight = accountDto.value<"addressHeight"_>();
        accountInfo.publicKey = accountDto.value<"publicKey"_>();
        accountInfo.publicKeyHeight = accountDto.value<"publicKeyHeight"_>();
        for(auto& mosaicDto : accountDto.value<"mosaics"_>()) {
            accountInfo.mosaics.push_back(fromDto<Mosaic, MosaicDto>(mosaicDto));
        }

        return accountInfo;
    }

	template<>
	MultipleAccountInfo fromDto<MultipleAccountInfo, MultipleAccountInfoDto>(const MultipleAccountInfoDto & dto) {
		MultipleAccountInfo multipleAccountInfo;
		for(auto & accountInfoDto : dto) {
			multipleAccountInfo.accountInfos.push_back(fromDto<AccountInfo, AccountInfoDto>(accountInfoDto));
		}
		return multipleAccountInfo;
	}

    template<>
    AccountName fromDto<AccountName, AccountNameDto>(const AccountNameDto& dto) {
        AccountName accountName;
        accountName.address = dto.value<"address"_>();
        accountName.names = dto.value<"names"_>();
        return accountName;
    }

    template<>
    AccountNames fromDto<AccountNames, AccountNamesDto>(const AccountNamesDto & dto) {
        AccountNames accountNames;
        for(auto & accountNameDto : dto) {
            accountNames.names.push_back(fromDto<AccountName, AccountNameDto>(accountNameDto));
        }
        return accountNames;
    }

    template<>
    MultisigLevel fromDto<MultisigLevel, MultisigLevelDto>(const MultisigLevelDto& dto) {
        MultisigLevel multisigLevel;
        multisigLevel.level = dto.value<"level"_>();
        for(auto& multisig : dto.value<"multisigEntries"_>()) {
            multisigLevel.multisigEntries.push_back(fromDto<MultisigInfo, MultisigInfoDto>(multisig));
        }
        return multisigLevel;
    }

    template<>
    MultisigGraph fromDto<MultisigGraph, MultisigGraphDto>(const MultisigGraphDto& dto) {
        MultisigGraph multisigGraph;
        for(auto& multisigLevel : dto) {
            multisigGraph.multisigLevels.push_back(fromDto<MultisigLevel, MultisigLevelDto>(multisigLevel));
        }
        return multisigGraph;
    }

    //Network
    template<>
    NetworkInfo fromDto<NetworkInfo, NetworkInfoDto> (const NetworkInfoDto& dto) {
        NetworkInfo networkInfo = {
                dto.value<"name"_>(),
                dto.value<"description"_>()
        };
        return networkInfo;
    }

    template<>
    NetworkConfigData fromDto<NetworkConfigData, NetworkConfigDataDto> (const NetworkConfigDataDto& dto) {
        NetworkConfigData networkConfigData;
        networkConfigData.height = dto.value<"height"_>();
        networkConfigData.config = dto.value<"networkConfig"_>();
        networkConfigData.supportedEntityVersions = dto.value<"supportedEntityVersions"_>();
        return networkConfigData;
    }

    template<>
    NetworkConfig fromDto<NetworkConfig, NetworkConfigDto>(const NetworkConfigDto &dto) {
        return {fromDto<NetworkConfigData, NetworkConfigDataDto>(dto.value<"networkConfig"_>())};
    }

    template<>
    NetworkVersionData fromDto<NetworkVersionData, NetworkVersionDataDto> (const NetworkVersionDataDto& dto) {
        NetworkVersionData networkVersionData;
        networkVersionData.height = dto.value<"height"_>();
        networkVersionData.version = dto.value<"blockChainVersion"_>();
        return networkVersionData;
    }

    template<>
    NetworkVersion fromDto<NetworkVersion, NetworkVersionDto> (const NetworkVersionDto& dto) {
        return {fromDto<NetworkVersionData, NetworkVersionDataDto>(dto.value<"blockchainUpgrade"_>())};
    }

	/// Transaction Meta

	template<>
	TransactionInfo fromDto<TransactionInfo, TransactionInfoDto>(const TransactionInfoDto& dto) {
		TransactionInfo transactionInfo;
        transactionInfo.height = dto.value<"height"_>();
        transactionInfo.index = dto.value<"index"_>();
        transactionInfo.id = dto.value<"id"_>();
        transactionInfo.hash = dto.value<"hash"_>();
        transactionInfo.merkleComponentHash = dto.value<"merkleComponentHash"_>();
        transactionInfo.aggregateHash = dto.value<"aggregateHash"_>();
        transactionInfo.uniqueAggregateHash = dto.value<"uniqueAggregateHash"_>();
        transactionInfo.aggregateId = dto.value<"aggregateId"_>();
		return transactionInfo;
	}

	template<>
	TransactionStatus fromDto<TransactionStatus, TransactionStatusDto>(const TransactionStatusDto& dto) {
		TransactionStatus transactionStatus;
		transactionStatus.group = dto.value<"group"_>();
		transactionStatus.status = dto.value<"status"_>();
		transactionStatus.hash = dto.value<"hash"_>();
		transactionStatus.deadline = dto.value<"deadline"_>();
		return transactionStatus;
	}

	template<>
	MultipleTransactionInfo fromDto<MultipleTransactionInfo, MultipleTransactionInfoDto>(const MultipleTransactionInfoDto& dto) {
		MultipleTransactionInfo transactionInfos;

		for(auto& transactionInfoDto : dto) {
			transactionInfos.infos.push_back(fromDto<TransactionInfo, TransactionInfoDto>(transactionInfoDto));
		}
		return transactionInfos;
	}

	template<>
	MultipleTransactionStatus fromDto<MultipleTransactionStatus, MultipleTransactionStatusDto>(const MultipleTransactionStatusDto& dto) {
		MultipleTransactionStatus transactionStatuses;

		for(auto& transactionStatusDto : dto) {
			transactionStatuses.statuses.push_back(fromDto<TransactionStatus, TransactionStatusDto>(transactionStatusDto));
		}
		return transactionStatuses;
	}

    template<>
    xpx_chain_sdk::transactions_page::TransactionMeta
    fromDto<xpx_chain_sdk::transactions_page::TransactionMeta, dto::transactions_page::TransactionMetaDto>(
            const dto::transactions_page::TransactionMetaDto &dto) {
        xpx_chain_sdk::transactions_page::TransactionMeta transactionMeta;
        transactionMeta.id = dto.value<"id"_>();
        transactionMeta.hash = dto.value<"hash"_>();
        transactionMeta.height = dto.value<"height"_>();
        transactionMeta.merkleComponentHash = dto.value<"merkleComponentHash"_>();
        transactionMeta.index = dto.value<"index"_>();
        return transactionMeta;
    }

    template<>
    xpx_chain_sdk::transactions_page::Transaction
    fromDto<xpx_chain_sdk::transactions_page::Transaction, dto::transactions_page::TransactionDto>(
            const dto::transactions_page::TransactionDto &dto) {
        xpx_chain_sdk::transactions_page::Transaction transaction;
        transaction.data = fromDto<Transaction, TransactionDto>(dto.value<"transaction"_>());
        transaction.meta = fromDto<xpx_chain_sdk::transactions_page::TransactionMeta, dto::transactions_page::TransactionMetaDto>(dto.value<"meta"_>());
        return transaction;
    }

    template<>
    xpx_chain_sdk::transactions_page::Pagination fromDto<
        xpx_chain_sdk::transactions_page::Pagination,
        dto::transactions_page::PaginationDto>(
        const dto::transactions_page::PaginationDto &dto) {
        xpx_chain_sdk::transactions_page::Pagination pagination {
                dto.value<"totalEntries"_>(),
                dto.value<"pageNumber"_>(),
                dto.value<"pageSize"_>(),
                dto.value<"totalPages"_>()
        };
        return pagination;
    }

    template<>
    xpx_chain_sdk::transactions_page::TransactionsPage fromDto<
            xpx_chain_sdk::transactions_page::TransactionsPage,
            dto::transactions_page::TransactionsPageDto>(
            const dto::transactions_page::TransactionsPageDto& dto) {
        xpx_chain_sdk::transactions_page::TransactionsPage transactionsPage;
        transactionsPage.pagination = fromDto<xpx_chain_sdk::transactions_page::Pagination, dto::transactions_page::PaginationDto>(dto.value<"pagination"_>());

        for(const auto& itemDto : dto.value<"data"_>()) {
            transactionsPage.transactions.push_back(fromDto<xpx_chain_sdk::transactions_page::Transaction, dto::transactions_page::TransactionDto>(itemDto));
        }
        return transactionsPage;
    }

    // Transactions

	template<>
	Cosignature fromDto<Cosignature, CosignatureDto>(const CosignatureDto& dto) {
		Cosignature result =  {
			dto.value<"publicKey"_>(),
			dto.value<"signature"_>()
		};
		return result;
	}

	template<>
	CosignatoryModification fromDto<CosignatoryModification, CosignatoryModificationDto>(const CosignatoryModificationDto& dto) {
		CosignatoryModification result =  {
				dto.value<"publicKey"_>(),
				dto.value<"modificationType"_>()
		};
		return result;
	}

	template<>
	AccountTransactionPropertyModification fromDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(const TransactionPropertyModificationDto& dto) {
		AccountTransactionPropertyModification result;

		result.value = dto.value<"value"_>();
		result.modificationType = dto.value<"modificationType"_>();

		return result;
	}

	template<>
	AccountMosaicPropertyModification fromDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(const MosaicPropertyModificationDto& dto) {
		AccountMosaicPropertyModification result = {
				dto.value<"value"_>(),
				dto.value<"modificationType"_>()
		};
		return result;
	}

	template<>
	AccountAddressPropertyModification fromDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(const AddressPropertyModificationDto& dto) {
		AccountAddressPropertyModification result = {
				dto.value<"value"_>(),
				dto.value<"modificationType"_>()
		};
		return result;
	}

	template<>
	MosaicProperty fromDto<MosaicProperty, MosaicPropertyDto>(const MosaicPropertyDto& dto) {
		MosaicProperty result = {
				dto.value<"id"_>(),
				dto.value<"value"_>()
		};

		return result;
	}

	template<>
	Transaction fromDto<Transaction, TransactionDto >(const TransactionDto & dto) {
		Transaction transaction;
		EXTRACT_TRANSACTION(transaction, dto)
		return transaction;
	}

	template<>
	transactions_info::EmbeddedTransaction fromDto<transactions_info::EmbeddedTransaction, EmbeddedTransactionDto >(const EmbeddedTransactionDto & dto) {
		xpx_chain_sdk::transactions_info::EmbeddedTransaction transaction;
		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)
		return transaction;
	}

	template<>
	AggregateTransaction fromDto<AggregateTransaction, AggregateTransactionDto >(const AggregateTransactionDto & dto) {
		AggregateTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.payloadSize = dto.value<"payloadSize"_>();
//		transaction.payload = dto.value<"payload"_>();
		for(auto & cosignature : dto.value<"cosignatures"_>()) {
			transaction.cosignatures.push_back(fromDto<Cosignature, CosignatureDto>(cosignature));
		}
		return transaction;
	}


	template<>
	AccountLinkTransaction fromDto<AccountLinkTransaction, AccountLinkTransactionDto>(const AccountLinkTransactionDto & dto) {
		AccountLinkTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.linkAction = dto.value<"linkAction"_>();
		transaction.remoteAccountKey = dto.value<"remoteAccountKey"_>();
		return transaction;
	}


	template<>
	LockFundsTransaction fromDto<LockFundsTransaction, LockFundsTransactionDto >(const LockFundsTransactionDto & dto) {
		LockFundsTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.lockedMosaic = fromDto<Mosaic, MosaicDto>(dto.value<"lockedMosaic"_>());
		transaction.lockDuration = dto.value<"lockDuration"_>();
		transaction.lockHash = dto.value<"lockHash"_>();
		return transaction;
	}


	template<>
	ModifyMultisigAccountTransaction fromDto<ModifyMultisigAccountTransaction, ModifyMultisigAccountTransactionDto >(const ModifyMultisigAccountTransactionDto & dto) {
		 
		ModifyMultisigAccountTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.minRemovalDelta = dto.value<"minRemovalDelta"_>();
		transaction.minApprovalDelta = dto.value<"minApprovalDelta"_>();
		for(auto& cosignatoryModificationDto : dto.value<"modifications"_>()) {
			transaction.modifications.push_back(fromDto<CosignatoryModification, CosignatoryModificationDto>(cosignatoryModificationDto));
		}
		return transaction;
	}


	template<>
	MosaicDefinitionTransaction fromDto<MosaicDefinitionTransaction, MosaicDefinitionTransactionDto >(const MosaicDefinitionTransactionDto & dto) {
		MosaicDefinitionTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.nonce = dto.value<"nonce"_>();
		transaction.mosaicId = dto.value<"mosaicId"_>();
		transaction.flags = dto.value<"flags"_>();
		transaction.divisibility = dto.value<"divisibility"_>();
		for(auto & mosaicProperty : dto.value<"optionalProperties"_>()) {
			transaction.optionalProperties.push_back(fromDto<MosaicProperty, MosaicPropertyDto>(mosaicProperty)) ;
		}
		return transaction;
	}


	template<>
	MosaicSupplyChangeTransaction fromDto<MosaicSupplyChangeTransaction, MosaicSupplyChangeTransactionDto >(const MosaicSupplyChangeTransactionDto & dto) {
		MosaicSupplyChangeTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.mosaicId = dto.value<"mosaicId"_>();
		transaction.direction = dto.value<"direction"_>();
		transaction.delta = dto.value<"delta"_>();
		return transaction;
	}


	template<>
	RegisterNamespaceTransaction fromDto<RegisterNamespaceTransaction, RegisterNamespaceTransactionDto >(const RegisterNamespaceTransactionDto & dto) {
		RegisterNamespaceTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.namespaceType = dto.value<"namespaceType"_>();
		transaction.durationOrParentId = dto.value<"durationOrParentId"_>();
		transaction.namespaceId = dto.value<"namespaceId"_>();
		transaction.namespaceNameSize = dto.value<"namespaceNameSize"_>();
		transaction.namespaceName = dto.value<"namespaceName"_>();

		return transaction;
	}

	template<>
	SecretLockTransaction fromDto<SecretLockTransaction, SecretLockTransactionDto >(const SecretLockTransactionDto & dto) {
		SecretLockTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.mosaicId = dto.value<"mosaicId"_>();
		transaction.amount = dto.value<"amount"_>();
		transaction.duration = dto.value<"duration"_>();
		transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
		transaction.secret = dto.value<"secret"_>();
		transaction.recipient = dto.value<"recipient"_>();


		return transaction;
	}


	template<>
	SecretProofTransaction fromDto<SecretProofTransaction, SecretProofTransactionDto >(const SecretProofTransactionDto & dto) {
		SecretProofTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
		transaction.secret = dto.value<"secret"_>();
		transaction.proof = dto.value<"proof"_>();


		return transaction;
	}

	template<>
	TransferTransactionMessage fromDto<TransferTransactionMessage, TransferTransactionMessageDto >(const TransferTransactionMessageDto & dto) {
		TransferTransactionMessage message;
		message.type = dto.value<"type"_>();
		message.payload = dto.value<"payload"_>();
		return message;
	}

	template<>
	TransferTransaction fromDto<TransferTransaction, TransferTransactionDto >(const TransferTransactionDto & dto) {
		TransferTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.recipient = dto.value<"recipient"_>();
		transaction.message = fromDto<TransferTransactionMessage, TransferTransactionMessageDto >(dto.value<"message"_>());
		for(auto& mosaicDto : dto.value<"mosaics"_>()) {
			transaction.mosaics.push_back(fromDto<Mosaic, MosaicDto>(mosaicDto));
		}
		return transaction;
	}


	template<>
	AliasTransactionBase fromDto<AliasTransactionBase, AliasTransactionBaseDto >(const AliasTransactionBaseDto & dto) {
		AliasTransactionBase transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.aliasAction = dto.value<"aliasAction"_>();
		transaction.namespaceId = dto.value<"namespaceId"_>();

		return transaction;
	}

	template<>
	AddressAliasTransaction fromDto<AddressAliasTransaction, AddressAliasTransactionDto >(const AddressAliasTransactionDto & dto) {
		AddressAliasTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.address = dto.value<"address"_>();
		return transaction;
	}

	template<>
	MosaicAliasTransaction fromDto<MosaicAliasTransaction, MosaicAliasTransactionDto >(const MosaicAliasTransactionDto & dto) {
		MosaicAliasTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.mosaicId = dto.value<"mosaicId"_>();
		return transaction;
	}

	/// Account Property Transactions
	template<>
	AccountTransactionPropertyTransaction fromDto<AccountTransactionPropertyTransaction, AccountTransactionPropertyTransactionDto >(const AccountTransactionPropertyTransactionDto & dto) {
		AccountTransactionPropertyTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.propertyType = dto.value<"propertyType"_>();
		transaction.modificationsCount = dto.value<"modificationsCount"_>();
		for(auto& accountTransactionPropertyDto : dto.value<"modifications"_>()){
			transaction.modifications.push_back(fromDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(accountTransactionPropertyDto));
		}

		return transaction;
	}

	template<>
	AccountMosaicPropertyTransaction fromDto<AccountMosaicPropertyTransaction, AccountMosaicPropertyTransactionDto >(const AccountMosaicPropertyTransactionDto & dto) {
		AccountMosaicPropertyTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.propertyType = dto.value<"propertyType"_>();
		transaction.modificationsCount = dto.value<"modificationsCount"_>();
		for(auto& accountTransactionPropertyDto : dto.value<"modifications"_>()){
			transaction.modifications.push_back(fromDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(accountTransactionPropertyDto));
		}

		return transaction;
	}

	template<>
	AccountAddressPropertyTransaction fromDto<AccountAddressPropertyTransaction, AccountAddressPropertyTransactionDto >(const AccountAddressPropertyTransactionDto & dto) {
		AccountAddressPropertyTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)


		transaction.propertyType = dto.value<"propertyType"_>();
		transaction.modificationsCount = dto.value<"modificationsCount"_>();
		for(auto& accountTransactionPropertyDto : dto.value<"modifications"_>()){
			transaction.modifications.push_back(fromDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(accountTransactionPropertyDto));
		}

		return transaction;
	}

	///Embedded transactions

	template<>
	EmbeddedAccountLinkTransaction fromDto<EmbeddedAccountLinkTransaction, EmbeddedAccountLinkTransactionDto >(const EmbeddedAccountLinkTransactionDto & dto) {
		EmbeddedAccountLinkTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.linkAction = dto.value<"linkAction"_>();
		transaction.remoteAccountKey = dto.value<"remoteAccountKey"_>();
		return transaction;
	}


	template<>
	EmbeddedLockFundsTransaction fromDto<EmbeddedLockFundsTransaction, EmbeddedLockFundsTransactionDto >(const EmbeddedLockFundsTransactionDto & dto) {
		EmbeddedLockFundsTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.lockedMosaic = fromDto<Mosaic, MosaicDto>(dto.value<"lockedMosaic"_>());
		transaction.lockDuration = dto.value<"lockDuration"_>();
		transaction.lockHash = dto.value<"lockHash"_>();
		return transaction;
	}


	template<>
	EmbeddedModifyMultisigAccountTransaction fromDto<EmbeddedModifyMultisigAccountTransaction, EmbeddedModifyMultisigAccountTransactionDto >(const EmbeddedModifyMultisigAccountTransactionDto & dto) {
		EmbeddedModifyMultisigAccountTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.minRemovalDelta = dto.value<"minRemovalDelta"_>();
		transaction.minApprovalDelta = dto.value<"minApprovalDelta"_>();
		for(auto& cosignatoryModificationDto : dto.value<"modifications"_>()) {
			transaction.modifications.push_back(fromDto<CosignatoryModification, CosignatoryModificationDto>(cosignatoryModificationDto));
		}

		return transaction;
	}


	template<>
	EmbeddedMosaicDefinitionTransaction fromDto<EmbeddedMosaicDefinitionTransaction, EmbeddedMosaicDefinitionTransactionDto >(const EmbeddedMosaicDefinitionTransactionDto & dto) {
		EmbeddedMosaicDefinitionTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)


		transaction.nonce = dto.value<"nonce"_>();
		transaction.mosaicId = dto.value<"mosaicId"_>();
		transaction.flags = dto.value<"flags"_>();
		transaction.divisibility = dto.value<"divisibility"_>();
		for(auto & mosaicProperty : dto.value<"optionalProperties"_>()) {
			transaction.optionalProperties.push_back(fromDto<MosaicProperty, MosaicPropertyDto>(mosaicProperty)) ;
		}
		return transaction;
	}


	template<>
	EmbeddedMosaicSupplyChangeTransaction fromDto<EmbeddedMosaicSupplyChangeTransaction, EmbeddedMosaicSupplyChangeTransactionDto >(const EmbeddedMosaicSupplyChangeTransactionDto & dto) {
		EmbeddedMosaicSupplyChangeTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.mosaicId = dto.value<"mosaicId"_>();
		transaction.direction = dto.value<"direction"_>();
		transaction.delta = dto.value<"delta"_>();
		return transaction;
	}


	template<>
	EmbeddedRegisterNamespaceTransaction fromDto<EmbeddedRegisterNamespaceTransaction, EmbeddedRegisterNamespaceTransactionDto >(const EmbeddedRegisterNamespaceTransactionDto & dto) {
		EmbeddedRegisterNamespaceTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.namespaceType = dto.value<"namespaceType"_>();
		transaction.durationOrParentId = dto.value<"durationOrParentId"_>();
		transaction.namespaceId = dto.value<"namespaceId"_>();
		transaction.namespaceNameSize = dto.value<"namespaceNameSize"_>();
		transaction.namespaceName = dto.value<"namespaceName"_>();

		return transaction;
	}

	template<>
	EmbeddedSecretLockTransaction fromDto<EmbeddedSecretLockTransaction, EmbeddedSecretLockTransactionDto >(const EmbeddedSecretLockTransactionDto & dto) {
		EmbeddedSecretLockTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)


		transaction.mosaicId = dto.value<"mosaicId"_>();
		transaction.amount = dto.value<"amount"_>();
		transaction.duration = dto.value<"duration"_>();
		transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
		transaction.secret = dto.value<"secret"_>();
		transaction.recipient = dto.value<"recipient"_>();


		return transaction;
	}


	template<>
	EmbeddedSecretProofTransaction fromDto<EmbeddedSecretProofTransaction, EmbeddedSecretProofTransactionDto >(const EmbeddedSecretProofTransactionDto & dto) {
		EmbeddedSecretProofTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.hashAlgorithm = dto.value<"hashAlgorithm"_>();
		transaction.secret = dto.value<"secret"_>();
		transaction.proof = dto.value<"proof"_>();


		return transaction;
	}


	template<>
	EmbeddedTransferTransaction fromDto<EmbeddedTransferTransaction, EmbeddedTransferTransactionDto >(const EmbeddedTransferTransactionDto & dto) {
		EmbeddedTransferTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)


		transaction.recipient = dto.value<"recipient"_>();
		transaction.message = fromDto<TransferTransactionMessage, TransferTransactionMessageDto >(dto.value<"message"_>());

		for(auto& mosaicDto : dto.value<"mosaics"_>()) {
			transaction.mosaics.push_back(fromDto<Mosaic, MosaicDto>(mosaicDto));
		}

		return transaction;
	}


	template<>
	EmbeddedAliasTransactionBase fromDto<EmbeddedAliasTransactionBase, EmbeddedAliasTransactionBaseDto >(const EmbeddedAliasTransactionBaseDto & dto) {
		EmbeddedAliasTransactionBase transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)


		transaction.aliasAction = dto.value<"aliasAction"_>();
		transaction.namespaceId = dto.value<"namespaceId"_>();

		return transaction;
	}

	template<>
	EmbeddedAddressAliasTransaction fromDto<EmbeddedAddressAliasTransaction, EmbeddedAddressAliasTransactionDto >(const EmbeddedAddressAliasTransactionDto & dto) {
		EmbeddedAddressAliasTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.address = dto.value<"address"_>();
		return transaction;
	}

	template<>
	EmbeddedMosaicAliasTransaction fromDto<EmbeddedMosaicAliasTransaction, EmbeddedMosaicAliasTransactionDto >(const EmbeddedMosaicAliasTransactionDto & dto) {
		EmbeddedMosaicAliasTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.mosaicId = dto.value<"mosaicId"_>();
		return transaction;
	}

	/// Account Property Transactions
	template<>
	EmbeddedAccountTransactionPropertyTransaction fromDto<EmbeddedAccountTransactionPropertyTransaction, EmbeddedAccountTransactionPropertyTransactionDto >(const EmbeddedAccountTransactionPropertyTransactionDto & dto) {
		EmbeddedAccountTransactionPropertyTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.propertyType = dto.value<"propertyType"_>();
		transaction.modificationsCount = dto.value<"modificationsCount"_>();
		for(auto& accountTransactionPropertyDto : dto.value<"modifications"_>()){
			transaction.modifications.push_back(fromDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(accountTransactionPropertyDto));
		}

		return transaction;
	}

	template<>
	EmbeddedAccountMosaicPropertyTransaction fromDto<EmbeddedAccountMosaicPropertyTransaction, EmbeddedAccountMosaicPropertyTransactionDto >(const EmbeddedAccountMosaicPropertyTransactionDto & dto) {
		EmbeddedAccountMosaicPropertyTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)


		transaction.propertyType = dto.value<"propertyType"_>();
		transaction.modificationsCount = dto.value<"modificationsCount"_>();
		for(auto& accountTransactionPropertyDto : dto.value<"modifications"_>()){
			transaction.modifications.push_back(fromDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(accountTransactionPropertyDto));
		}

		return transaction;
	}

	template<>
	EmbeddedAccountAddressPropertyTransaction fromDto<EmbeddedAccountAddressPropertyTransaction, EmbeddedAccountAddressPropertyTransactionDto >(const EmbeddedAccountAddressPropertyTransactionDto & dto) {
		EmbeddedAccountAddressPropertyTransaction transaction;

		EXTRACT_EMBEDDED_TRANSACTION(transaction, dto)

		transaction.propertyType = dto.value<"propertyType"_>();
		transaction.modificationsCount = dto.value<"modificationsCount"_>();
		for(auto& accountTransactionPropertyDto : dto.value<"modifications"_>()){
			transaction.modifications.push_back(fromDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(accountTransactionPropertyDto));
		}

		return transaction;
	}

	template<>
	PrepareBcDriveTransaction fromDto<PrepareBcDriveTransaction, PrepareBcDriveTransactionDto >(const PrepareBcDriveTransactionDto & dto) {
		PrepareBcDriveTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.driveSize = dto.value<"driveSize"_>();
		transaction.replicatorCount = dto.value<"replicatorCount"_>();

		return transaction;
	}

	template<>
	DataModificationTransaction fromDto<DataModificationTransaction, DataModificationTransactionDto >(const DataModificationTransactionDto & dto) {
		DataModificationTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.driveKey = dto.value<"driveKey"_>();
		transaction.downloadDataCdi = dto.value<"downloadDataCdi"_>();
		transaction.uploadSize = dto.value<"uploadSize"_>();

		return transaction;
	}

	template<>
	DownloadTransaction fromDto<DownloadTransaction, DownloadTransactionDto >(const DownloadTransactionDto & dto) {
		DownloadTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.driveKey = dto.value<"driveKey"_>();
		transaction.downloadSize = dto.value<"downloadSize"_>();
		transaction.transactionFee = dto.value<"transactionFee"_>();

		return transaction;
	}

	template<>
	DataModificationApprovalTransaction fromDto<DataModificationApprovalTransaction, DataModificationApprovalTransactionDto >(const DataModificationApprovalTransactionDto & dto) {
		DataModificationApprovalTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.driveKey = dto.value<"driveKey"_>();
		transaction.dataModificationId = dto.value<"dataModificationId"_>();
		transaction.fileStructureCdi = dto.value<"fileStructureCdi"_>();
		transaction.fileStructureSize = dto.value<"fileStructureSize"_>();
		transaction.usedDriveSize = dto.value<"usedDriveSize"_>();

		return transaction;
	}

	template<>
	DataModificationCancelTransaction fromDto<DataModificationCancelTransaction, DataModificationCancelTransactionDto >(const DataModificationCancelTransactionDto & dto) {
		DataModificationCancelTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.driveKey = dto.value<"driveKey"_>();
		transaction.dataModificationId = dto.value<"dataModificationId"_>();

		return transaction;
	}

	template<>
	ReplicatorOnboardingTransaction fromDto<ReplicatorOnboardingTransaction, ReplicatorOnboardingTransactionDto >(const ReplicatorOnboardingTransactionDto & dto) {
		ReplicatorOnboardingTransaction transaction;

		EXTRACT_TRANSACTION(transaction, dto)

		transaction.capacity = dto.value<"capacity"_>();
		transaction.blskey = dto.value<"blskey"_>();

		return transaction;
	}

	template<>
	ReplicatorOffboardingTransaction fromDto<ReplicatorOffboardingTransaction, ReplicatorOffboardingTransactionDto >(const ReplicatorOffboardingTransactionDto & dto) {
		ReplicatorOffboardingTransaction transaction;

		return transaction;
	}

    template<>
    Uid fromDto<Uid, UidDto>(const UidDto &dto) {
        return { dto.value<"uid"_>() };
    }

    template<>
    WebsocketMeta fromDto<WebsocketMeta, WebsocketMetaDto>(const WebsocketMetaDto &dto) {
        WebsocketMeta meta;
        meta.channelName = dto.value<"channelName"_>();
        meta.address = dto.value<"address"_>();
        return meta;
    }

    template<>
    WebsocketInfo fromDto<WebsocketInfo, WebsocketInfoDto>(const WebsocketInfoDto &dto) {
        return { fromDto<WebsocketMeta, WebsocketMetaDto>(dto.value<"meta"_>()) };
    }

    template<>
    TransactionData fromDto<TransactionData, TransactionDataDto>(const TransactionDataDto &dto) {
        TransactionData transactionData;
        transactionData.signature = dto.value<"signature"_>();
        transactionData.signer = dto.value<"signer"_>();
        transactionData.version = dto.value<"version"_>();
        transactionData.type = dto.value<"type"_>();
        transactionData.maxFee = dto.value<"maxFee"_>();
        transactionData.deadline = dto.value<"deadline"_>();
        transactionData.hash = dto.value<"hash"_>();
        return transactionData;
    }

    template<>
    TransactionMeta fromDto<TransactionMeta, TransactionMetaDto>(const TransactionMetaDto &dto) {
        TransactionMeta transactionMeta;
        transactionMeta.hash = dto.value<"hash"_>();
        transactionMeta.merkleComponentHash = dto.value<"merkleComponentHash"_>();
        transactionMeta.height = dto.value<"height"_>();
        return transactionMeta;
    }

    template<>
    TransactionNotification fromDto<TransactionNotification, TransactionNotificationDto>(const TransactionNotificationDto &dto) {
        TransactionNotification transactionNotification;
        transactionNotification.data = fromDto<TransactionData, TransactionDataDto>(dto.value<"transaction"_>());
        transactionNotification.meta = fromDto<TransactionMeta, TransactionMetaDto>(dto.value<"meta"_>());
        return transactionNotification;
    }

    template<>
    TransactionStatusNotification fromDto<TransactionStatusNotification, TransactionStatusNotificationDto>(const TransactionStatusNotificationDto &dto) {
        TransactionStatusNotification transactionStatusNotification;
        transactionStatusNotification.hash = dto.value<"hash"_>();
        transactionStatusNotification.status = dto.value<"status"_>();
        return transactionStatusNotification;
    }

    template<>
    SignerInfoNotification fromDto<SignerInfoNotification, SignerInfoNotificationDto>(const SignerInfoNotificationDto &dto) {
        SignerInfoNotification signerInfoNotification;
        signerInfoNotification.signer = dto.value<"signer"_>();
        signerInfoNotification.signature = dto.value<"signature"_>();
        signerInfoNotification.hash = dto.value<"hash"_>();
        return signerInfoNotification;
    }

    template<>
    DriveStateNotification fromDto<DriveStateNotification, DriveStateNotificationDto>(const DriveStateNotificationDto &dto) {
        DriveStateNotification driveStateNotification;
        driveStateNotification.driveKey = dto.value<"driveKey"_>();
        driveStateNotification.state = dto.value<"state"_>();
        return driveStateNotification;
    }
}