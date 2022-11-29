/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/service/serialization.h>
#include <xpxchaincpp/utils/format.h>

#include "sdk/model/transaction/create_transaction.h"
#include "sdk/model/transaction/transaction_utils.h"
#include "infrastructure/binary/parser.h"
#include "infrastructure/binary/dto/transaction_dto.h"

#include <iterator>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace xpx_chain_sdk::internal;
using namespace xpx_chain_sdk::internal::binary;
using namespace xpx_chain_sdk;

namespace xpx_chain_sdk {
	
	namespace {
		
		bool ReadEmbeddedTransactions(RawBuffer data, EmbeddedTransactions& embeddedTransactions);
		
		template<
			typename TImpl,
			typename TDto,
			typename... TArgs,
			typename = typename std::enable_if<std::is_base_of_v<Transaction, TImpl>>::type>
		auto CreateTransaction(const TDto& dto, RawBuffer binaryData, TArgs&&... args)
		{
			bool isSigned = (dto.template value<"signer"_>() != Key{}) &&
			                (dto.template value<"signature"_>() != Signature{});
			std::vector<uint8_t> data(binaryData.begin(), binaryData.end());
			
			return std::make_unique<TImpl>(
				std::forward<TArgs>(args)...,
				dto.template value<"version"_>(),
				dto.template value<"maxFee"_>(),
				dto.template value<"deadline"_>(),
				std::move(data),
				isSigned ? std::optional<Key>(dto.template value<"signer"_>()) : std::nullopt,
				isSigned ? std::optional<Signature>(dto.template value<"signature"_>()) : std::nullopt);
		}
		
		template<
			typename TImpl,
			typename TDto,
			typename... TArgs,
			typename = typename std::enable_if<std::is_base_of_v<EmbeddedTransaction, TImpl>>::type>
		std::unique_ptr<TImpl> CreateTransaction(const TDto& dto, RawBuffer, TArgs&&... args)
		{
			return std::make_unique<TImpl>(
				std::forward<TArgs>(args)..., dto.template value<"version"_>(), dto.template value<"signer"_>());
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    AccountLinkTransactionImpl,
			                                    EmbeddedAccountLinkTransactionImpl>>
		std::unique_ptr<TImpl> CreateAccountLinkTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"linkAction"_>(), dto.template value<"remoteAccountKey"_>());
		}
		
		template<typename TImpl, typename TDto>
		std::unique_ptr<TImpl> CreateAccountPropertyTransaction(const TDto& dto, RawBuffer binaryData)
		{
			typename TImpl::PropertyModifications modifications;
			
			for (const auto& modificationDTO: dto.template value<"modifications"_>()) {
				modifications.insert({ modificationDTO.template value<"value"_>(),
				                       modificationDTO.template value<"modificationType"_>() });
			}
			
			AccountPropertyRule propertyRule =
				static_cast<AccountPropertyRule>(dto.template value<"propertyType"_>() & 0x80);
			
			if (modifications.size() != dto.template value<"modifications"_>().size()) {
				return nullptr;
			}
			
			return CreateTransaction<TImpl>(dto, binaryData, propertyRule, std::move(modifications));
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    AddressAliasTransactionImpl,
			                                    EmbeddedAddressAliasTransactionImpl>>
		std::unique_ptr<TImpl> CreateAddressAliasTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"aliasAction"_>(), dto.template value<"namespaceId"_>(),
				dto.template value<"address"_>());
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    MosaicAliasTransactionImpl,
			                                    EmbeddedMosaicAliasTransactionImpl>>
		std::unique_ptr<TImpl> CreateMosaicAliasTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"aliasAction"_>(), dto.template value<"namespaceId"_>(),
				dto.template value<"mosaicId"_>());
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    LockFundsTransactionImpl,
			                                    EmbeddedLockFundsTransactionImpl>>
		std::unique_ptr<TImpl> CreateLockFundsTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				Mosaic{ dto.template value<"lockedMosaic"_>().template value<"id"_>(),
				        dto.template value<"lockedMosaic"_>().template value<"amount"_>() },
				dto.template value<"lockDuration"_>(), dto.template value<"lockHash"_>());
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    ModifyMultisigAccountTransactionImpl,
			                                    EmbeddedModifyMultisigAccountTransactionImpl>>
		std::unique_ptr<TImpl> CreateModifyMultisigAccountTransaction(const TDto& dto, RawBuffer binaryData)
		{
			std::unordered_map<Key, CosignatoryModificationType> cosignatoryModifications;
			
			for (const auto& modificationDTO: dto.template value<"modifications"_>()) {
				cosignatoryModifications[modificationDTO.template value<"publicKey"_>()] =
					modificationDTO.template value<"modificationType"_>();
			}
			
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"minRemovalDelta"_>(), dto.template value<"minApprovalDelta"_>(),
				cosignatoryModifications);
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    MosaicDefinitionTransactionImpl,
			                                    EmbeddedMosaicDefinitionTransactionImpl>>
		std::unique_ptr<TImpl> CreateMosaicDefinitionTransaction(const TDto& dto, RawBuffer binaryData)
		{
			MosaicPropertyContainer properties;
			
			for (const auto& propertyDTO: dto.template value<"optionalProperties"_>()) {
				properties.insert(MosaicProperty{ propertyDTO.template value<"id"_>(),
				                                  propertyDTO.template value<"value"_>() });
			}
			
			if (properties.size() != dto.template value<"optionalProperties"_>().size()) {
				return nullptr;
			}

			properties.insert(MosaicProperty{ MosaicPropertyId::Flags, to_underlying_type(dto.template value<"flags"_>()) });
			properties.insert(MosaicProperty{ MosaicPropertyId::Divisibility, dto.template value<"divisibility"_>() });
			
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"mosaicNonce"_>(),
				dto.template value<"mosaicId"_>(),
				MosaicProperties(std::move(properties))
            );
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    MosaicSupplyChangeTransactionImpl,
			                                    EmbeddedMosaicSupplyChangeTransactionImpl>>
		std::unique_ptr<TImpl> CreateMosaicSupplyChangeTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"mosaicId"_>(), dto.template value<"direction"_>(), dto.template value<"delta"_>());
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    RegisterNamespaceTransactionImpl,
			                                    EmbeddedRegisterNamespaceTransactionImpl>>
		std::unique_ptr<TImpl> CreateRegisterNamespaceTransaction(const TDto& dto, RawBuffer binaryData)
		{
			NamespaceId parentId = Namespace_Base_Id;
			BlockDuration duration = 0;
			
			if (dto.template value<"namespaceType"_>() == NamespaceType::Child) {
				parentId = dto.template value<"durationOrParentId"_>();
			} else if (dto.template value<"namespaceType"_>() == NamespaceType::Root) {
				duration = dto.template value<"durationOrParentId"_>();
			}
			
			Namespace ns(dto.template value<"namespaceName"_>(), parentId);
			
			if (ns.id() != dto.template value<"namespaceId"_>()) {
				return nullptr;
			}
			
			return CreateTransaction<TImpl>(dto, binaryData, std::move(ns), duration);
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    SecretLockTransactionImpl,
			                                    EmbeddedSecretLockTransactionImpl>>
		std::unique_ptr<TImpl> CreateSecretLockTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"hashAlgorithm"_>(), dto.template value<"secret"_>(),
				Mosaic { dto.template value<"mosaicId"_>(), dto.template value<"amount"_>() },
				dto.template value<"duration"_>(), dto.template value<"recipient"_>());
		}
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    SecretProofTransactionImpl,
			                                    EmbeddedSecretProofTransactionImpl>>
		std::unique_ptr<TImpl> CreateSecretProofTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"hashAlgorithm"_>(),
                    dto.template value<"secret"_>(),
                    dto.template value<"proof"_>());
		}

        template<
                typename TDto,
                typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
                        StoragePaymentTransactionImpl,
                        EmbeddedStoragePaymentTransactionImpl>>
        std::unique_ptr<TImpl> CreateStoragePaymentTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"driveKey"_>(),
                    dto.template value<"storageUnits"_>());
        }
		
		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    TransferTransactionImpl,
			                                    EmbeddedTransferTransactionImpl>>
		std::unique_ptr<TImpl> CreateTransferTransaction(const TDto& dto, RawBuffer binaryData)
		{
			MosaicContainer mosaics;
			
			for (const auto& mosaicDTO:  dto.template value<"mosaics"_>()) {
				mosaics.insert(Mosaic{ mosaicDTO.template value<"id"_>(), mosaicDTO.template value<"amount"_>() });
			}
			
			if (mosaics.size() != dto.template value<"mosaics"_>().size()) {
				return nullptr;
			}
			
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"recipient"_>(), std::move(mosaics), dto.template value<"message"_>());
		}
		
		std::unique_ptr<AggregateTransactionImpl> CreateAggregateTransaction(const AggregateTransactionDTO& dto, 
		                                                                     RawBuffer binaryData,
		                                                                     bool isComplete)
		{
			EmbeddedTransactions embeddedTransactions;
			std::unordered_map<Key, Signature> cosignatures;
			
			for (const auto& cosignatureDTO: dto.value<"cosignatures"_>()) {
				cosignatures[cosignatureDTO.value<"publicKey"_>()] = cosignatureDTO.value<"signature"_>();
			}
			
			if (cosignatures.size() != dto.value<"cosignatures"_>().size() ||
			    !ReadEmbeddedTransactions(dto.value<"payload"_>(), embeddedTransactions)) {
				return nullptr;
			}
			
			return CreateTransaction<AggregateTransactionImpl>(
				dto, binaryData, isComplete, embeddedTransactions, cosignatures, dto.value<"payload"_>().size());
		}

		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    PrepareBcDriveTransactionImpl,
			                                    EmbeddedPrepareBcDriveTransactionImpl>>
		std::unique_ptr<TImpl> CreatePrepareBcDriveTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
                dto.template value<"driveSize"_>(),
                dto.template value<"verificationFeeAmount"_>(),
                dto.template value<"replicatorCount"_>());
		}

        template<
            typename TDto,
            typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
                                                CreateLiquidityProviderTransactionImpl,
                                                EmbeddedCreateLiquidityProviderTransactionImpl>>
        std::unique_ptr<TImpl> CreateCreateLiquidityProviderTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"providerMosaicId"_>(),
                    dto.template value<"currencyDeposit"_>(),
                    dto.template value<"initialMosaicsMinting"_>(),
                    dto.template value<"slashingPeriod"_>(),
                    dto.template value<"windowSize"_>(),
                    dto.template value<"slashingAccount"_>(),
                    dto.template value<"alpha"_>(),
                    dto.template value<"beta"_>());
        }

        template<
            typename TDto,
            typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
                                                ManualRateChangeTransactionImpl,
                                                EmbeddedManualRateChangeTransactionImpl>>
        std::unique_ptr<TImpl> CreateManualRateChangeTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"providerMosaicId"_>(),
                    dto.template value<"currencyBalanceIncrease"_>(),
                    dto.template value<"currencyBalanceChange"_>(),
                    dto.template value<"mosaicBalanceIncrease"_>(),
                    dto.template value<"mosaicBalanceChange"_>());
        }

		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    DataModificationTransactionImpl,
			                                    EmbeddedDataModificationTransactionImpl>>
        std::unique_ptr<TImpl> CreateDataModificationTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"driveKey"_>(),
                    dto.template value<"downloadDataCdi"_>(),
                    dto.template value<"uploadSize"_>(),
                    dto.template value<"feedbackFeeAmount"_>());
        }

		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    DownloadTransactionImpl,
			                                    EmbeddedDownloadTransactionImpl>>
		std::unique_ptr<TImpl> CreateDownloadTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"driveKey"_>(),
                    dto.template value<"downloadSize"_>(),
                    dto.template value<"feedbackFeeAmount"_>(),
                    dto.template value<"listOfPublicKeysSize"_>(),
                    dto.template value<"listOfPublicKeys"_>());
		}

        template<
                typename TDto,
                typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
                        DownloadPaymentTransactionImpl,
                        EmbeddedDownloadPaymentTransactionImpl>>
        std::unique_ptr<TImpl> CreateDownloadPaymentTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"downloadChannelId"_>(),
                    dto.template value<"downloadSize"_>(),
                    dto.template value<"feedbackFeeAmount"_>());
        }

        template<
                typename TDto,
                typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
                        DriveClosureTransactionImpl,
                        EmbeddedDriveClosureTransactionImpl>>
        std::unique_ptr<TImpl> CreateDriveClosureTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"driveKey"_>());
        }

		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    DataModificationApprovalTransactionImpl,
			                                    EmbeddedDataModificationApprovalTransactionImpl>>
		std::unique_ptr<TImpl> CreateDataModificationApprovalTransaction(const TDto& dto, RawBuffer binaryData)
		{
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"driveKey"_>(),
                    dto.template value<"dataModificationId"_>(),
                    dto.template value<"fileStructureCdi"_>(),
                    dto.template value<"modificationStatus"_>(),
                    dto.template value<"fileStructureSizeBytes"_>(),
                    dto.template value<"metaFilesSizeBytes"_>(),
                    dto.template value<"usedDriveSizeBytes"_>(),
                    dto.template value<"judgingKeysCount"_>(),
                    dto.template value<"overlappingKeysCount"_>(),
                    dto.template value<"judgedKeysCount"_>(),
                    dto.template value<"opinionElementCount"_>(),
                    dto.template value<"publicKeys"_>(),
                    dto.template value<"signatures"_>(),
                    dto.template value<"presentOpinions"_>(),
                    dto.template value<"opinions"_>());
		}

		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    DataModificationCancelTransactionImpl,
			                                    EmbeddedDataModificationCancelTransactionImpl>>
		std::unique_ptr<TImpl> CreateDataModificationCancelTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"driveKey"_>(), dto.template value<"dataModificationId"_>());
		}

        template<
                typename TDto,
                typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
                        FinishDownloadTransactionImpl,
                        EmbeddedFinishDownloadTransactionImpl>>
        std::unique_ptr<TImpl> CreateFinishDownloadTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"downloadChannelId"_>(),
                    dto.template value<"feedbackFeeAmount"_>());
        }

		template<
			typename TDto,
			typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
			                                    ReplicatorOnboardingTransactionImpl,
			                                    EmbeddedReplicatorOnboardingTransactionImpl>>
		std::unique_ptr<TImpl> CreateReplicatorOnboardingTransaction(const TDto& dto, RawBuffer binaryData)
		{
			return CreateTransaction<TImpl>(
				dto, binaryData,
				dto.template value<"capacity"_>());
		}

        template<
                typename TDto,
                typename TImpl = std::conditional_t<std::is_base_of_v<TransactionDTO, TDto>,
                        ReplicatorOffboardingTransactionImpl,
                        EmbeddedReplicatorOffboardingTransactionImpl>>
        std::unique_ptr<TImpl> CreateReplicatorOffboardingTransaction(const TDto& dto, RawBuffer binaryData)
        {
            return CreateTransaction<TImpl>(
                    dto, binaryData,
                    dto.template value<"driveKey"_>());
        }
		
		bool ReadEmbeddedTransactions(RawBuffer data, EmbeddedTransactions& embeddedTransactions)
		{
			EmbeddedTransactionDTO header;
			
			size_t startPos = 0;
			ParseResult result = Parser::Read(header, data, startPos);
			
			while (result) {
				
				if (ExtractTransactionVersion(header.value<"version"_>()) != GetTransactionVersion(header.value<"type"_>())) {
					return false;
				}
				
				EmbeddedTransactionPtr embeddedTransaction;
				
				switch (header.value<"type"_>()) {
				case TransactionType::Account_Link:
					{
						EmbeddedAccountLinkTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateAccountLinkTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Address_Property:
					{
						EmbeddedAccountAddressPropertyTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateAccountPropertyTransaction<EmbeddedAccountAddressPropertyTransactionImpl>(
								dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Mosaic_Property:
					{
						EmbeddedAccountMosaicPropertyTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateAccountPropertyTransaction<EmbeddedAccountMosaicPropertyTransactionImpl>(
								dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Transaction_Property:
					{
						EmbeddedAccountTransactionPropertyTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateAccountPropertyTransaction<EmbeddedAccountTransactionPropertyTransactionImpl>(
								dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Address_Alias:
					{
						EmbeddedAddressAliasTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateAddressAliasTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Mosaic_Alias:
					{
						EmbeddedMosaicAliasTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateMosaicAliasTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Lock_Funds:
					{
						EmbeddedLockFundsTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateLockFundsTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Modify_Multisig_Account:
					{
						EmbeddedModifyMultisigAccountTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateModifyMultisigAccountTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Mosaic_Definition:
					{
						EmbeddedMosaicDefinitionTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateMosaicDefinitionTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Mosaic_Supply_Change:
					{
						EmbeddedMosaicSupplyChangeTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateMosaicSupplyChangeTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Register_Namespace:
					{
						EmbeddedRegisterNamespaceTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateRegisterNamespaceTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Secret_Lock:
					{
						EmbeddedSecretLockTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateSecretLockTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Secret_Proof:
					{
						EmbeddedSecretProofTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateSecretProofTransaction(dto, RawBuffer{});
						}
						
						break;
					}
                case TransactionType::Storage_Payment:
                    {
                        EmbeddedStoragePaymentTransactionDTO dto;
                        result = Parser::Read(dto, data, startPos);

                        if (result) {
                            embeddedTransaction = CreateStoragePaymentTransaction(dto, RawBuffer{});
                        }

                        break;
                    }
				case TransactionType::Transfer:
					{
						EmbeddedTransferTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);
						
						if (result) {
							embeddedTransaction = CreateTransferTransaction(dto, RawBuffer{});
						}
						
						break;
					}
				case TransactionType::Prepare_Bc_Drive:
					{
						EmbeddedPrepareBcDriveTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);

						if (result) {
							embeddedTransaction = CreatePrepareBcDriveTransaction(dto, RawBuffer{});
						}

						break;
					}
                case TransactionType::Create_Liquidity_Provider:
                    {
                        EmbeddedCreateLiquidityProviderTransactionDTO dto;
                        result = Parser::Read(dto, data, startPos);

                        if (result) {
                            embeddedTransaction = CreateCreateLiquidityProviderTransaction(dto, RawBuffer{});
                        }

                        break;
                    }
                case TransactionType::Manual_Rate_Change:
                    {
                        EmbeddedManualRateChangeTransactionDTO dto;
                        result = Parser::Read(dto, data, startPos);

                        if (result) {
                            embeddedTransaction = CreateManualRateChangeTransaction(dto, RawBuffer{});
                        }

                        break;
                    }
				case TransactionType::Data_Modification:
					{
						EmbeddedDataModificationTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);

						if (result) {
							embeddedTransaction = CreateDataModificationTransaction(dto, RawBuffer{});
						}

						break;
					}
				case TransactionType::Download:
					{
						EmbeddedDownloadTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);

						if (result) {
							embeddedTransaction = CreateDownloadTransaction(dto, RawBuffer{});
						}

						break;
					}
                case TransactionType::Download_Payment:
                    {
                        EmbeddedDownloadPaymentTransactionDTO dto;
                        result = Parser::Read(dto, data, startPos);

                        if (result) {
                            embeddedTransaction = CreateDownloadPaymentTransaction(dto, RawBuffer{});
                        }

                        break;
                    }
                case TransactionType::Drive_Closure:
                    {
                        EmbeddedDriveClosureTransactionDTO dto;
                        result = Parser::Read(dto, data, startPos);

                        if (result) {
                            embeddedTransaction = CreateDriveClosureTransaction(dto, RawBuffer{});
                        }

                        break;
                    }
				case TransactionType::Data_Modification_Approval:
					{
						EmbeddedDataModificationApprovalTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);

						if (result) {
							embeddedTransaction = CreateDataModificationApprovalTransaction(dto, RawBuffer{});
						}

						break;
					}
				case TransactionType::Data_Modification_Cancel:
					{
						EmbeddedDataModificationCancelTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);

						if (result) {
							embeddedTransaction = CreateDataModificationCancelTransaction(dto, RawBuffer{});
						}

						break;
					}
                case TransactionType::Finish_Download:
                    {
                        EmbeddedFinishDownloadTransactionDTO dto;
                        result = Parser::Read(dto, data, startPos);

                        if (result) {
                            embeddedTransaction = CreateFinishDownloadTransaction(dto, RawBuffer{});
                        }

                        break;
                    }
				case TransactionType::Replicator_Onboarding:
					{
						EmbeddedReplicatorOnboardingTransactionDTO dto;
						result = Parser::Read(dto, data, startPos);

						if (result) {
							embeddedTransaction = CreateReplicatorOnboardingTransaction(dto, RawBuffer{});
						}

						break;
					}
                case TransactionType::Replicator_Offboarding:
                    {
                        EmbeddedReplicatorOffboardingTransactionDTO dto;
                        result = Parser::Read(dto, data, startPos);

                        if (result) {
                            embeddedTransaction = CreateReplicatorOffboardingTransaction(dto, RawBuffer{});
                        }

                        break;
                    }
				default:
					{
						return false;
					}
				}
				
				if (embeddedTransaction) {
					embeddedTransactions.push_back(std::move(embeddedTransaction));
					startPos += result.processedSize();
					
					if (startPos != data.size()) {
						result = Parser::Read(header, data, startPos);
					} else {
						break;
					}
				} else {
					return false;
				}
			}
			
			return result;
		}
	}
	
	
	std::vector<uint8_t> TransactionToBinary(const Transaction* transaction)
	{
		return transaction->binary();
	}
	
	TransactionPtr TransactionFromBinary(RawBuffer data)
	{
		TransactionPtr transaction;
		ReadResult result = TryCreateTransactionFromBinary(data, transaction);
		
		if (!result || result.consumed() != data.size()) {
			XPX_CHAIN_SDK_THROW_1(serialization_error, "failed to deserialize transaction from binary data",
			                 result.description());
		}
		
		return transaction;
	}
	
	ReadResult TryCreateTransactionFromBinary(RawBuffer data, TransactionPtr& transaction)
	{
		TransactionDTO header;
		ParseResult parseResult = Parser::Read(header, data);
		
		if (!parseResult || header.value<"size"_>() < data.size()) {
			return { ReadResultCode::Not_Enough_Data, 0, "not enough data" };
		}
		
		MakeString description;
		description << "'" << GetTransactionName(header.value<"type"_>()) << "': ";
		
		if (ExtractTransactionVersion(header.value<"version"_>()) != GetTransactionVersion(header.value<"type"_>())) {
			return { ReadResultCode::Failure, header.value<"size"_>(), (description << "incorrect version").str() };
		}
		
		switch (header.value<"type"_>()) {
		case TransactionType::Account_Link:
			{
				AccountLinkTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateAccountLinkTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Address_Property:
			{
				AccountAddressPropertyTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateAccountPropertyTransaction<AccountAddressPropertyTransactionImpl>(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Mosaic_Property:
			{
				AccountMosaicPropertyTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateAccountPropertyTransaction<AccountMosaicPropertyTransactionImpl>(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Transaction_Property:
			{
				AccountTransactionPropertyTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateAccountPropertyTransaction<AccountTransactionPropertyTransactionImpl>(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Address_Alias:
			{
				AddressAliasTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateAddressAliasTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Mosaic_Alias:
			{
				MosaicAliasTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateMosaicAliasTransaction(dto, binaryData);
				}
				
				break;
			}
		
		case TransactionType::Aggregate_Complete:
		case TransactionType::Aggregate_Bonded:
			{
				AggregateTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateAggregateTransaction(
						dto, binaryData, header.value<"type"_>() == TransactionType::Aggregate_Complete);
				}
				
				break;
			}
		case TransactionType::Lock_Funds:
			{
				LockFundsTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateLockFundsTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Modify_Multisig_Account:
			{
				ModifyMultisigAccountTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateModifyMultisigAccountTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Mosaic_Definition:
			{
				MosaicDefinitionTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateMosaicDefinitionTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Mosaic_Supply_Change:
			{
				MosaicSupplyChangeTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateMosaicSupplyChangeTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Register_Namespace:
			{
				RegisterNamespaceTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateRegisterNamespaceTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Secret_Lock:
			{
				SecretLockTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateSecretLockTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Secret_Proof:
			{
				SecretProofTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateSecretProofTransaction(dto, binaryData);
				}
				
				break;
			}
        case TransactionType::Storage_Payment:
            {
                StoragePaymentTransactionDTO dto;
                parseResult = Parser::Read(dto, data);
                RawBuffer binaryData(data.data(), parseResult.processedSize());

                if (binaryData.size() == dto.value<"size"_>()) {
                    transaction = CreateStoragePaymentTransaction(dto, binaryData);
                }

                break;
            }
		case TransactionType::Transfer:
			{
				TransferTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());
				
				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateTransferTransaction(dto, binaryData);
				}
				
				break;
			}
		case TransactionType::Prepare_Bc_Drive:
			{
				PrepareBcDriveTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());

				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreatePrepareBcDriveTransaction(dto, binaryData);
				}

				break;
			}
        case TransactionType::Create_Liquidity_Provider:
            {
                CreateLiquidityProviderTransactionDTO dto;
                parseResult = Parser::Read(dto, data);
                RawBuffer binaryData(data.data(), parseResult.processedSize());

                if (binaryData.size() == dto.value<"size"_>()) {
                    transaction = CreateCreateLiquidityProviderTransaction(dto, binaryData);
                }

                break;
            }
        case TransactionType::Manual_Rate_Change:
            {
                ManualRateChangeTransactionDTO dto;
                parseResult = Parser::Read(dto, data);
                RawBuffer binaryData(data.data(), parseResult.processedSize());

                if (binaryData.size() == dto.value<"size"_>()) {
                    transaction = CreateManualRateChangeTransaction(dto, binaryData);
                }

                break;
            }
		case TransactionType::Data_Modification:
			{
				DataModificationTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());

				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateDataModificationTransaction(dto, binaryData);
				}

				break;
			}
		case TransactionType::Download:
			{
				DownloadTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());

				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateDownloadTransaction(dto, binaryData);
				}

				break;
			}
        case TransactionType::Download_Payment:
            {
                DownloadPaymentTransactionDTO dto;
                parseResult = Parser::Read(dto, data);
                RawBuffer binaryData(data.data(), parseResult.processedSize());

                if (binaryData.size() == dto.value<"size"_>()) {
                    transaction = CreateDownloadPaymentTransaction(dto, binaryData);
                }

                break;
            }
        case TransactionType::Drive_Closure:
            {
                DriveClosureTransactionDTO dto;
                parseResult = Parser::Read(dto, data);
                RawBuffer binaryData(data.data(), parseResult.processedSize());

                if (binaryData.size() == dto.value<"size"_>()) {
                    transaction = CreateDriveClosureTransaction(dto, binaryData);
                }

                break;
            }
		case TransactionType::Data_Modification_Approval:
			{
				DataModificationApprovalTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());

				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateDataModificationApprovalTransaction(dto, binaryData);
				}

				break;
			}
		case TransactionType::Data_Modification_Cancel:
			{
				DataModificationCancelTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());

				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateDataModificationCancelTransaction(dto, binaryData);
				}

				break;
			}
        case TransactionType::Finish_Download:
            {
                FinishDownloadTransactionDTO dto;
                parseResult = Parser::Read(dto, data);
                RawBuffer binaryData(data.data(), parseResult.processedSize());

                if (binaryData.size() == dto.value<"size"_>()) {
                    transaction = CreateFinishDownloadTransaction(dto, binaryData);
                }

                break;
            }
		case TransactionType::Replicator_Onboarding:
			{
				ReplicatorOnboardingTransactionDTO dto;
				parseResult = Parser::Read(dto, data);
				RawBuffer binaryData(data.data(), parseResult.processedSize());

				if (binaryData.size() == dto.value<"size"_>()) {
					transaction = CreateReplicatorOnboardingTransaction(dto, binaryData);
				}

				break;
			}
        case TransactionType::Replicator_Offboarding:
            {
                ReplicatorOffboardingTransactionDTO dto;
                parseResult = Parser::Read(dto, data);
                RawBuffer binaryData(data.data(), parseResult.processedSize());

                if (binaryData.size() == dto.value<"size"_>()) {
                    transaction = CreateReplicatorOffboardingTransaction(dto, binaryData);
                }

                break;
            }
		default:
			{
				break;
			}
		}
		
		if (transaction) {
			return { ReadResultCode::Success, header.value<"size"_>() };
		} else {
			return { ReadResultCode::Failure, header.value<"size"_>(), (description << "incorrect data").str() };
		}
	}
}
