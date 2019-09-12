
#include "sdk/model/transaction/create_transaction.h"
#include "sdk/model/transaction/transaction_utils.h"
#include "infrastructure/binary/parser.h"
#include "infrastructure/binary/dto/transaction_dto.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <type_traits>

using namespace xpx_sdk::internal;
using namespace xpx_sdk::internal::binary;

namespace xpx_sdk { namespace internal {

	
	namespace {
		
		template<typename TDto>
		void AppendDtoData(const TDto& dto, std::vector<uint8_t>& data)
		{
			if (!Parser::Write(dto, data)) {
				NEM2_SDK_THROW_2(transaction_error, "failed to serialize transaction",
				                 GetTransactionName(dto.template value<"type"_>()),
				                 (std::is_base_of_v<TransactionDTO, TDto> ? "general" : "embedded"));
			}
		}
		
		template<
			typename TImpl,
			typename TDto,
			typename... TArgs,
			typename = typename std::enable_if<std::is_base_of_v<Transaction, TImpl>>::type>
		std::unique_ptr<TImpl> CreateTransaction(const TDto& dto,
		                                         const std::optional<Key>& signer,
		                                         const std::optional<Signature>& signature,
		                                         const std::optional<TransactionInfo>& info,
		                                         TArgs&&... args)
		{
			std::vector<uint8_t> binaryData;
			AppendDtoData(dto, binaryData);
			
			return std::make_unique<TImpl>(
				std::forward<TArgs>(args)...,
				dto.template value<"version"_>(), dto.template value<"maxFee"_>(), dto.template value<"deadline"_>(),
				std::move(binaryData), signer, signature, info);
		}
		
		template<
			typename TImpl,
			typename TDto,
			typename... TArgs,
			typename = typename std::enable_if<std::is_base_of_v<EmbeddedTransaction, TImpl>>::type>
		std::unique_ptr<TImpl> CreateTransaction(const TDto& dto,
		                                         TArgs&&... args)
		{
			return std::make_unique<TImpl>(
				std::forward<TArgs>(args)..., dto.template value<"version"_>(), dto.template value<"signer"_>());
		}
		
		template<
			typename TDto,
			typename = typename std::enable_if<std::is_base_of_v<TransactionDTO, TDto>>::type>
		void InitTransactionDTO(TDto& dto,
		                        TransactionType type,
		                        const std::optional<Amount>& maxFee,
		                        const std::optional<NetworkDuration>& deadline,
		                        const std::optional<NetworkIdentifier>& networkId,
		                        const std::optional<Key>& signer,
		                        const std::optional<Signature>& signature)
		{
			size_t size = Parser::CalculateSize(dto);
			
			dto.template set<"size"_>(size);
			dto.template set<"signature"_>(signature ? *signature : Signature{});
			dto.template set<"signer"_>(signer ? *signer : Key{});
			dto.template set<"version"_>(MakeTransactionFullVersion(GetTransactionVersion(type),
			                                                        networkId ? *networkId : GetConfig().NetworkId));
			dto.template set<"type"_>(type);
			
			dto.template set<"maxFee"_>(maxFee ? *maxFee : Transaction::GetDefaultFee(size));
			dto.template set<"deadline"_>(deadline ? deadline->count() : Transaction::GetDefaultDeadline().count());
		}
		
		template<
			typename TDto,
			typename = typename std::enable_if<std::is_base_of_v<EmbeddedTransactionDTO, TDto>>::type>
		void InitTransactionDTO(TDto& dto,
		                        TransactionType type,
		                        const Key& signer,
		                        const std::optional<NetworkIdentifier>& networkId)
		{
			size_t size = Parser::CalculateSize(dto);
			
			dto.template set<"size"_>(size);
			dto.template set<"signer"_>(signer);
			dto.template set<"version"_>(MakeTransactionFullVersion(GetTransactionVersion(type),
			                                                        networkId ? *networkId : GetConfig().NetworkId));
			dto.template set<"type"_>(type);
		}
		
		template<typename TDto, typename... TArgs>
		void InitAccountLinkTransactionDTO(TDto& dto,
		                                   AccountLinkTransactionAction action,
		                                   const Key& remoteAccountKey,
		                                   TArgs&&... args)
		{
			dto.template set<"linkAction"_>(action);
			dto.template set<"remoteAccountKey"_>(remoteAccountKey);
			
			InitTransactionDTO(dto, TransactionType::Account_Link, std::forward<TArgs>(args)...);
		}
		
		template<typename TProperty, typename TDto, typename... TArgs>
		void InitAccountPropertyTransactionDTO(TDto& dto,
		                                       TransactionType transactionType,
		                                       AccountPropertyRule propertyRule,
		                                       const AccountPropertyModifications<typename TProperty::ValueType>&
		                                           propertyModifications,
		                                       TArgs&&... args)
		{
			dto.template set<"propertyType"_>(
				to_underlying_type(TProperty::GetType()) | to_underlying_type(propertyRule));
			dto.template set<"modificationsCount"_>(propertyModifications.size());
			
			for (const auto& modification: propertyModifications) {
				dto.template value<"modifications"_>().push_back(
					TAccountPropertyModificationDTO<typename TProperty::ValueType>{ modification.value,
					                                                                modification.modificationType });
			}
			
			InitTransactionDTO(dto, transactionType, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitAddressAliasTransactionDTO(TDto& dto,
		                                    AliasTransactionAction action,
		                                    NamespaceId aliasNamespaceId,
		                                    const Address& aliasedAddress,
		                                    TArgs&&... args)
		{
			dto.template set<"aliasAction"_>(action);
			dto.template set<"namespaceId"_>(aliasNamespaceId);
			dto.template set<"address"_>(aliasedAddress.binary());
			
			InitTransactionDTO(dto, TransactionType::Address_Alias, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitMosaicAliasTransactionDTO(TDto& dto,
		                                   AliasTransactionAction action,
		                                   NamespaceId aliasNamespaceId,
		                                   MosaicId aliasedMosaicId,
		                                   TArgs&&... args)
		{
			dto.template set<"aliasAction"_>(action);
			dto.template set<"namespaceId"_>(aliasNamespaceId);
			dto.template set<"mosaicId"_>(aliasedMosaicId);
			
			InitTransactionDTO(dto, TransactionType::Mosaic_Alias, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitLockFundsTransactionDTO(TDto& dto,
		                                 const Mosaic& lockedMosaic,
		                                 BlockDuration lockDuration,
		                                 const Hash256& lockHash,
		                                 TArgs&&... args)
		{
			dto.template set<"lockedMosaic"_>(MosaicDTO{ lockedMosaic.id, lockedMosaic.amount });
			dto.template set<"lockDuration"_>(lockDuration);
			dto.template set<"lockHash"_>(lockHash);
			
			InitTransactionDTO(dto, TransactionType::Lock_Funds, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename TAccount, typename... TArgs>
		void InitModifyMultisigAccountTransactionDTO(TDto& dto,
		                                             int8_t minRemovalDelta,
		                                             int8_t minApprovalDelta,
		                                             const std::unordered_map<TAccount, CosignatoryModificationType>&
		                                                 cosignatoryModifications,
		                                             TArgs&&... args)
		{
			dto.template set<"minRemovalDelta"_>(minRemovalDelta);
			dto.template set<"minApprovalDelta"_>(minApprovalDelta);
			dto.template set<"modificationsCount"_>(cosignatoryModifications.size());
			
			for (const auto& [account, modType]: cosignatoryModifications) {
				if constexpr (std::is_same_v<TAccount, PublicAccount>) {
					dto.template value<"modifications"_>().push_back(
						CosignatoryModificationDTO{ account.publicKey(), modType });
				} else {
					dto.template value<"modifications"_>().push_back(CosignatoryModificationDTO{ account, modType });
				}
			}
			dto.template isSet<"modifications"_>() = true;
			
			InitTransactionDTO(dto, TransactionType::Modify_Multisig_Account, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitMosaicDefinitionTransactionDTO(TDto& dto,
		                                        uint32_t mosaicNonce,
		                                        MosaicId mosaicId,
		                                        MosaicFlags flags,
		                                        const MosaicProperties& mosaicProperties,
		                                        TArgs&&... args)
		{
			dto.template set<"nonce"_>(mosaicNonce);
			dto.template set<"mosaicId"_>(mosaicId);
			dto.template set<"optionalPropertiesCount"_>(mosaicProperties.size() - 2);
			dto.template set<"flags"_>(flags);// TODO:(mosaicProperties.flags());
			dto.template set<"divisibility"_>(mosaicProperties.divisibility());
			
			std::for_each(mosaicProperties.optionalBegin(), mosaicProperties.end(), [&dto](const auto& property) {
				dto.template value<"optionalProperties"_>().push_back(MosaicPropertyDTO{ property.id, property.value });
			});
			dto.template isSet<"optionalProperties"_>() = true;
			
			InitTransactionDTO(dto, TransactionType::Mosaic_Definition, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitMosaicSupplyChangeTransactionDTO(TDto& dto,
		                                          MosaicId mosaicId,
		                                          MosaicSupplyChangeDirection changeDirection,
		                                          Amount changeDelta,
		                                          TArgs&&... args)
		{
			dto.template set<"mosaicId"_>(mosaicId);
			dto.template set<"direction"_>(changeDirection);
			dto.template set<"delta"_>(changeDelta);
			
			InitTransactionDTO(dto, TransactionType::Mosaic_Supply_Change, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitRegisterNamespaceTransactionDTO(TDto& dto,
		                                         const Namespace& ns,
		                                         uint64_t durationOrParentId,
		                                         TArgs&&... args)
		{
			dto.template set<"namespaceType"_>(ns.type());
			dto.template set<"durationOrParentId"_>(durationOrParentId);
			dto.template set<"namespaceId"_>(ns.id());
			dto.template set<"namespaceNameSize"_>(ns.name().size());
			dto.template set<"namespaceName"_>(ns.name());
			
			InitTransactionDTO(dto, TransactionType::Register_Namespace, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitSecretLockTransactionDTO(TDto& dto,
		                                  SecretHashAlgorithm secretHashAlgorithm,
		                                  const Hash256& secretHash,
		                                  const Mosaic& lockedMosaic,
		                                  BlockDuration lockDuration,
		                                  const Address& lockedMosaicRecipient,
		                                  TArgs&&... args)
		{
			dto.template set<"mosaicId"_>(lockedMosaic.id);
			dto.template set<"amount"_>(lockedMosaic.amount);
			dto.template set<"duration"_>(lockDuration);
			dto.template set<"hashAlgorithm"_>(secretHashAlgorithm);
			dto.template set<"secret"_>(secretHash);
			dto.template set<"recipient"_>(lockedMosaicRecipient.binary());
			
			InitTransactionDTO(dto, TransactionType::Secret_Proof, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitSecretProofTransactionDTO(TDto& dto,
		                                   SecretHashAlgorithm secretHashAlgorithm,
		                                   const Hash256& secretHash,
		                                   RawBuffer secret,
		                                   TArgs&&... args)
		{
			if (secret.empty()) {
				NEM2_SDK_THROW(transaction_error, "empty secret in secret proof transaction");
			}
			
			dto.template set<"hashAlgorithm"_>(secretHashAlgorithm);
			dto.template set<"secret"_>(secretHash);
			dto.template set<"proofSize"_>(secret.size());
			
			dto.template value<"proof"_>().resize(secret.size());
			std::copy(secret.begin(), secret.end(), dto.template value<"proof"_>().begin());
			dto.template isSet<"proof"_>() = true;
			
			InitTransactionDTO(dto, TransactionType::Secret_Proof, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitTransferTransactionDTO(TDto& dto,
		                                const Address& recipient,
		                                const MosaicContainer& mosaics,
		                                RawBuffer message,
		                                TArgs&&... args)
		{
			if (mosaics.empty()) {
				NEM2_SDK_THROW(transaction_error, "empty mosaics in transfer transaction");
			}
			
			if (message.empty()) {
				NEM2_SDK_THROW(transaction_error, "empty message in transfer transaction");
			}
			
			std::vector<MosaicDTO> dtoMosaics;
			for (const auto& mosaic: mosaics) {
				dtoMosaics.push_back(MosaicDTO{ mosaic.id, mosaic.amount });
			}
			
			dto.template set<"recipient"_>(recipient.binary());
			dto.template set<"messageSize"_>(message.size());
			dto.template set<"mosaicsCount"_>(mosaics.size());
			dto.template set<"message"_>(std::vector<uint8_t>(message.begin(), message.end()));
			dto.template set<"mosaics"_>(std::move(dtoMosaics));
			
			InitTransactionDTO(dto, TransactionType::Transfer, std::forward<TArgs>(args)...);
		}
		
		template<typename TDto, typename... TArgs>
		void InitAggregateTransactionDTO(TDto& dto,
		                                 bool isComplete,
		                                 const EmbeddedTransactions& embeddedTransactions,
		                                 const std::unordered_map<Key, Signature>& cosignatures,
		                                 TArgs&&... args)
		{
			if (embeddedTransactions.empty()) {
				NEM2_SDK_THROW(transaction_error, "no embedded transactions in aggregate transaction");
			}
			
			// Serializing embedded transactions and creating aggregate transaction binary payload.
			
			std::vector<uint8_t> payload;
			
			for (const auto& embedded: embeddedTransactions) {
				switch (embedded->type()) {
				case TransactionType::Account_Link:
					{
						auto tx = dynamic_cast<const EmbeddedAccountLinkTransaction*>(embedded.get());
						EmbeddedAccountLinkTransactionDTO dto;
						
						InitAccountLinkTransactionDTO(
							dto, tx->action(), tx->remotePublicAccount().publicKey(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Address_Property:
					{
						auto tx = dynamic_cast<const EmbeddedAccountAddressPropertyTransaction*>(embedded.get());
						EmbeddedAccountAddressPropertyTransactionDTO dto;
						
						InitAccountPropertyTransactionDTO<AccountAddressProperty>(
							dto, TransactionType::Address_Property, tx->propertyRule(), tx->propertyModifications(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Mosaic_Property:
					{
						auto tx = dynamic_cast<const EmbeddedAccountMosaicPropertyTransaction*>(embedded.get());
						EmbeddedAccountMosaicPropertyTransactionDTO dto;
						
						InitAccountPropertyTransactionDTO<AccountMosaicProperty>(
							dto, TransactionType::Mosaic_Property, tx->propertyRule(), tx->propertyModifications(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Transaction_Property:
					{
						auto tx = dynamic_cast<const EmbeddedAccountTransactionPropertyTransaction*>(embedded.get());
						EmbeddedAccountTransactionPropertyTransactionDTO dto;
						
						InitAccountPropertyTransactionDTO<AccountTransactionProperty>(
							dto, TransactionType::Transaction_Property, tx->propertyRule(), tx->propertyModifications(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Address_Alias:
					{
						auto tx = dynamic_cast<const EmbeddedAddressAliasTransaction*>(embedded.get());
						EmbeddedAddressAliasTransactionDTO dto;
						
						InitAddressAliasTransactionDTO(
							dto, tx->action(), tx->aliasNamespaceId(), tx->aliasedAddress(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Mosaic_Alias:
					{
						auto tx = dynamic_cast<const EmbeddedMosaicAliasTransaction*>(embedded.get());
						EmbeddedMosaicAliasTransactionDTO dto;
						
						InitMosaicAliasTransactionDTO(
							dto, tx->action(), tx->aliasNamespaceId(), tx->aliasedMosaicId(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Lock_Funds:
					{
						auto tx = dynamic_cast<const EmbeddedLockFundsTransaction*>(embedded.get());
						EmbeddedLockFundsTransactionDTO dto;
						
						InitLockFundsTransactionDTO(
							dto, tx->lockedMosaic(), tx->lockDuration(), tx->lockHash(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Modify_Multisig_Account:
					{
						auto tx = dynamic_cast<const EmbeddedModifyMultisigAccountTransaction*>(embedded.get());
						EmbeddedModifyMultisigAccountTransactionDTO dto;
						
						InitModifyMultisigAccountTransactionDTO(
							dto, tx->minRemovalDelta(), tx->minApprovalDelta(), tx->cosignatoryModifications(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Mosaic_Definition:
					{
						auto tx = dynamic_cast<const EmbeddedMosaicDefinitionTransaction*>(embedded.get());
						EmbeddedMosaicDefinitionTransactionDTO dto;
						
						InitMosaicDefinitionTransactionDTO(
							dto, tx->mosaicNonce(), tx->mosaicId(), tx -> flags(), tx->mosaicProperties(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Mosaic_Supply_Change:
					{
						auto tx = dynamic_cast<const EmbeddedMosaicSupplyChangeTransaction*>(embedded.get());
						EmbeddedMosaicSupplyChangeTransactionDTO dto;
						
						InitMosaicSupplyChangeTransactionDTO(
							dto, tx->mosaicId(), tx->changeDirection(), tx->changeDelta(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Register_Namespace:
					{
						auto tx = dynamic_cast<const EmbeddedRegisterNamespaceTransaction*>(embedded.get());
						EmbeddedRegisterNamespaceTransactionDTO dto;
						
						InitRegisterNamespaceTransactionDTO(
							dto, tx->ns(),
							tx->ns().type() == NamespaceType::Root ? tx->nsDuration() : tx->ns().parentId(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Secret_Lock:
					{
						auto tx = dynamic_cast<const EmbeddedSecretLockTransaction*>(embedded.get());
						EmbeddedSecretLockTransactionDTO dto;
						
						InitSecretLockTransactionDTO(
							dto, tx->secretHashAlgorithm(), tx->secretHash(),
							tx->lockedMosaic(), tx->lockDuration(), tx->lockedMosaicRecipient(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Secret_Proof:
					{
						auto tx = dynamic_cast<const EmbeddedSecretProofTransaction*>(embedded.get());
						EmbeddedSecretProofTransactionDTO dto;
						
						InitSecretProofTransactionDTO(
							dto, tx->secretHashAlgorithm(), tx->secretHash(), tx->secret(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				case TransactionType::Transfer:
					{
						auto tx = dynamic_cast<const EmbeddedTransferTransaction*>(embedded.get());
						EmbeddedTransferTransactionDTO dto;
						
						InitTransferTransactionDTO(
							dto, tx->recipient(), tx->mosaics(), tx->message(),
							tx->signer().publicKey(), tx->networkId());
						
						AppendDtoData(dto, payload);
						break;
					}
				default:
					{
						NEM2_SDK_THROW_1(transaction_error, "unknown embedded transaction type",
						                 to_underlying_type(embedded->type()));
					}
				}
			}
			
			dto.template set<"payloadSize"_>(payload.size());
			dto.template set<"payload"_>(std::move(payload));
			
			// Adding cosignatures.
			
			for (const auto& [publicKey, signature]: cosignatures) {
				dto.template value<"cosignatures"_>().push_back(CosignatureDTO{ publicKey, signature });
			}
			dto.template isSet<"cosignatures"_>() = true;
			
			// Finalizing aggregate transaction dto initialization.
			
			TransactionType transactionType = isComplete ? TransactionType::Aggregate_Complete
			                                             : TransactionType::Aggregate_Bonded;
			InitTransactionDTO(dto, transactionType, std::forward<TArgs>(args)...);
		}
	}
	
	
	
	std::unique_ptr<AccountLinkTransaction>
	CreateAccountLinkTransactionImpl(AccountLinkTransactionAction action,
	                                 const Key& remoteAccountKey,
	                                 std::optional<Amount> maxFee,
	                                 std::optional<NetworkDuration> deadline,
	                                 std::optional<NetworkIdentifier> networkId,
	                                 const std::optional<Key>& signer,
	                                 const std::optional<Signature>& signature,
	                                 const std::optional<TransactionInfo>& info)
	{
		AccountLinkTransactionDTO dto;
		InitAccountLinkTransactionDTO(
			dto, action, remoteAccountKey, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<AccountLinkTransactionImpl>(
			dto, signer, signature, info, action, remoteAccountKey);
	}
	
	std::unique_ptr<AccountAddressPropertyTransaction>
	CreateAccountAddressPropertyTransactionImpl(AccountPropertyRule propertyRule,
	                                            AccountAddressPropertyModifications propertyModifications,
	                                            std::optional<Amount> maxFee,
	                                            std::optional<NetworkDuration> deadline,
	                                            std::optional<NetworkIdentifier> networkId,
	                                            const std::optional<Key>& signer,
	                                            const std::optional<Signature>& signature,
	                                            const std::optional<TransactionInfo>& info)
	{
		AccountAddressPropertyTransactionDTO dto;
		InitAccountPropertyTransactionDTO<AccountAddressProperty>(
			dto, TransactionType::Address_Property, propertyRule, propertyModifications,
			maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<AccountAddressPropertyTransactionImpl>(
			dto, signer, signature, info, propertyRule, std::move(propertyModifications));
	}
	
	std::unique_ptr<AccountMosaicPropertyTransaction>
	CreateAccountMosaicPropertyTransactionImpl(AccountPropertyRule propertyRule,
	                                           AccountMosaicPropertyModifications propertyModifications,
	                                           std::optional<Amount> maxFee,
	                                           std::optional<NetworkDuration> deadline,
	                                           std::optional<NetworkIdentifier> networkId,
	                                           const std::optional<Key>& signer,
	                                           const std::optional<Signature>& signature,
	                                           const std::optional<TransactionInfo>& info)
	{
		AccountMosaicPropertyTransactionDTO dto;
		InitAccountPropertyTransactionDTO<AccountMosaicProperty>(
			dto, TransactionType::Mosaic_Property, propertyRule, propertyModifications,
			maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<AccountMosaicPropertyTransactionImpl>(
			dto, signer, signature, info, propertyRule, std::move(propertyModifications));
	}
	
	std::unique_ptr<AccountTransactionPropertyTransaction>
	CreateAccountTransactionPropertyTransactionImpl(AccountPropertyRule propertyRule,
	                                                AccountTransactionPropertyModifications propertyModifications,
	                                                std::optional<Amount> maxFee,
	                                                std::optional<NetworkDuration> deadline,
	                                                std::optional<NetworkIdentifier> networkId,
	                                                const std::optional<Key>& signer,
	                                                const std::optional<Signature>& signature,
	                                                const std::optional<TransactionInfo>& info)
	{
		AccountTransactionPropertyTransactionDTO dto;
		InitAccountPropertyTransactionDTO<AccountTransactionProperty>(
			dto, TransactionType::Transaction_Property, propertyRule, propertyModifications,
			maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<AccountTransactionPropertyTransactionImpl>(
			dto, signer, signature, info, propertyRule, std::move(propertyModifications));
	}
	
	std::unique_ptr<AddressAliasTransaction>
	CreateAddressAliasTransactionImpl(AliasTransactionAction action,
	                                  NamespaceId aliasNamespaceId,
	                                  const Address& aliasedAddress,
	                                  std::optional<Amount> maxFee,
	                                  std::optional<NetworkDuration> deadline,
	                                  std::optional<NetworkIdentifier> networkId,
	                                  const std::optional<Key>& signer,
	                                  const std::optional<Signature>& signature,
	                                  const std::optional<TransactionInfo>& info)
	{
		AddressAliasTransactionDTO dto;
		InitAddressAliasTransactionDTO(
			dto, action, aliasNamespaceId, aliasedAddress, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<AddressAliasTransactionImpl>(
			dto, signer, signature, info, action, aliasNamespaceId, aliasedAddress);
	}
	
	std::unique_ptr<MosaicAliasTransaction>
	CreateMosaicAliasTransactionImpl(AliasTransactionAction action,
	                                 NamespaceId aliasNamespaceId,
	                                 MosaicId aliasedMosaicId,
	                                 std::optional<Amount> maxFee,
	                                 std::optional<NetworkDuration> deadline,
	                                 std::optional<NetworkIdentifier> networkId,
	                                 const std::optional<Key>& signer,
	                                 const std::optional<Signature>& signature,
	                                 const std::optional<TransactionInfo>& info)
	{
		MosaicAliasTransactionDTO dto;
		InitMosaicAliasTransactionDTO(
			dto, action, aliasNamespaceId, aliasedMosaicId, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<MosaicAliasTransactionImpl>(
			dto, signer, signature, info, action, aliasNamespaceId, aliasedMosaicId);
	}
	
	std::unique_ptr<LockFundsTransaction>
	CreateLockFundsTransactionImpl(const Mosaic& lockedMosaic,
	                               BlockDuration lockDuration,
	                               const Hash256& lockHash,
	                               std::optional<Amount> maxFee,
	                               std::optional<NetworkDuration> deadline,
	                               std::optional<NetworkIdentifier> networkId,
	                               const std::optional<Key>& signer,
	                               const std::optional<Signature>& signature,
	                               const std::optional<TransactionInfo>& info)
	{
		LockFundsTransactionDTO dto;
		InitLockFundsTransactionDTO(
			dto, lockedMosaic, lockDuration, lockHash, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<LockFundsTransactionImpl>(
			dto, signer, signature, info, lockedMosaic, lockDuration, lockHash);
	}
	
	std::unique_ptr<ModifyMultisigAccountTransaction>
	CreateModifyMultisigAccountTransactionImpl(int8_t minRemovalDelta,
	                                           int8_t minApprovalDelta,
	                                           const std::unordered_map<Key, CosignatoryModificationType>& cosignatoryModifications,
	                                           std::optional<Amount> maxFee,
	                                           std::optional<NetworkDuration> deadline,
	                                           std::optional<NetworkIdentifier> networkId,
	                                           const std::optional<Key>& signer,
	                                           const std::optional<Signature>& signature,
	                                           const std::optional<TransactionInfo>& info)
	{
		ModifyMultisigAccountTransactionDTO dto;
		InitModifyMultisigAccountTransactionDTO(
			dto, minRemovalDelta, minApprovalDelta, cosignatoryModifications,
			maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<ModifyMultisigAccountTransactionImpl>(
			dto, signer, signature, info, minRemovalDelta, minApprovalDelta, cosignatoryModifications);
	}
	
	std::unique_ptr<MosaicDefinitionTransaction>
	CreateMosaicDefinitionTransactionImpl(uint32_t mosaicNonce,
	                                      MosaicId mosaicId,
	                                      MosaicFlags flags,
	                                      MosaicProperties mosaicProperties,
	                                      std::optional<Amount> maxFee,
	                                      std::optional<NetworkDuration> deadline,
	                                      std::optional<NetworkIdentifier> networkId,
	                                      const std::optional<Key>& signer,
	                                      const std::optional<Signature>& signature,
	                                      const std::optional<TransactionInfo>& info)
	{
		MosaicDefinitionTransactionDTO dto;
		InitMosaicDefinitionTransactionDTO(
			dto, mosaicNonce, mosaicId, flags, mosaicProperties, maxFee, deadline, networkId, signer, signature);
			
		return CreateTransaction<MosaicDefinitionTransactionImpl>(
			dto, signer, signature, info, mosaicNonce, mosaicId, flags, std::move(mosaicProperties));
	}
	
	std::unique_ptr<MosaicSupplyChangeTransaction>
	CreateMosaicSupplyChangeTransactionImpl(MosaicId mosaicId,
	                                        MosaicSupplyChangeDirection changeDirection,
	                                        Amount changeDelta,
	                                        std::optional<Amount> maxFee,
	                                        std::optional<NetworkDuration> deadline,
	                                        std::optional<NetworkIdentifier> networkId,
	                                        const std::optional<Key>& signer,
	                                        const std::optional<Signature>& signature,
	                                        const std::optional<TransactionInfo>& info)
	{
		MosaicSupplyChangeTransactionDTO dto;
		InitMosaicSupplyChangeTransactionDTO(
			dto, mosaicId, changeDirection, changeDelta, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<MosaicSupplyChangeTransactionImpl>(
			dto, signer, signature, info, mosaicId, changeDirection, changeDelta);
	}
	
	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterRootNamespaceTransactionImpl(std::string_view namespaceName,
	                                           BlockDuration namespaceDuration,
	                                           std::optional<Amount> maxFee,
	                                           std::optional<NetworkDuration> deadline,
	                                           std::optional<NetworkIdentifier> networkId,
	                                           const std::optional<Key>& signer,
	                                           const std::optional<Signature>& signature,
	                                           const std::optional<TransactionInfo>& info)
	{
		Namespace ns(namespaceName);
		
		RegisterNamespaceTransactionDTO dto;
		InitRegisterNamespaceTransactionDTO(
			dto, ns, namespaceDuration, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<RegisterNamespaceTransactionImpl>(
			dto, signer, signature, info, std::move(ns), namespaceDuration);
	}
	
	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterChildNamespaceTransactionImpl(std::string_view namespaceName,
	                                            NamespaceId namespaceParentId,
	                                            std::optional<Amount> maxFee,
	                                            std::optional<NetworkDuration> deadline,
	                                            std::optional<NetworkIdentifier> networkId,
	                                            const std::optional<Key>& signer,
	                                            const std::optional<Signature>& signature,
	                                            const std::optional<TransactionInfo>& info)
	{
		Namespace ns(namespaceName, namespaceParentId);
		
		RegisterNamespaceTransactionDTO dto;
		InitRegisterNamespaceTransactionDTO(
			dto, ns, namespaceParentId, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<RegisterNamespaceTransactionImpl>(dto, signer, signature, info, std::move(ns), 0);
	}
	
	std::unique_ptr<SecretLockTransaction>
	CreateSecretLockTransactionImpl(SecretHashAlgorithm secretHashAlgorithm,
	                                const Hash256& secretHash,
	                                const Mosaic& lockedMosaic,
	                                BlockDuration lockDuration,
	                                const Address& lockedMosaicRecipient,
	                                std::optional<Amount> maxFee,
	                                std::optional<NetworkDuration> deadline,
	                                std::optional<NetworkIdentifier> networkId,
	                                const std::optional<Key>& signer,
	                                const std::optional<Signature>& signature,
	                                const std::optional<TransactionInfo>& info)
	{
		SecretLockTransactionDTO dto;
		InitSecretLockTransactionDTO(
			dto, secretHashAlgorithm, secretHash, lockedMosaic, lockDuration, lockedMosaicRecipient,
			maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<SecretLockTransactionImpl>(
			dto, signer, signature, info,
			secretHashAlgorithm, secretHash, lockedMosaic, lockDuration, lockedMosaicRecipient);
	}
	
	std::unique_ptr<SecretProofTransaction>
	CreateSecretProofTransactionImpl(SecretHashAlgorithm secretHashAlgorithm,
	                                 const Hash256& secretHash,
	                                 RawBuffer secret,
	                                 std::optional<Amount> maxFee,
	                                 std::optional<NetworkDuration> deadline,
	                                 std::optional<NetworkIdentifier> networkId,
	                                 const std::optional<Key>& signer,
	                                 const std::optional<Signature>& signature,
	                                 const std::optional<TransactionInfo>& info)
	{
		SecretProofTransactionDTO dto;
		InitSecretProofTransactionDTO(
			dto, secretHashAlgorithm, secretHash, secret,  maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<SecretProofTransactionImpl>(
			dto, signer, signature, info, secretHashAlgorithm, secretHash, secret);
	}
	
	std::unique_ptr<TransferTransaction>
	CreateTransferTransactionImpl(const Address& recipient,
	                              MosaicContainer mosaics,
	                              RawBuffer message,
	                              std::optional<Amount> maxFee,
	                              std::optional<NetworkDuration> deadline,
	                              std::optional<NetworkIdentifier> networkId,
	                              const std::optional<Key>& signer,
	                              const std::optional<Signature>& signature,
	                              const std::optional<TransactionInfo>& info)
	{
		TransferTransactionDTO dto;
		InitTransferTransactionDTO(
			dto, recipient, mosaics, message, maxFee, deadline, networkId, signer, signature);
		
		return CreateTransaction<TransferTransactionImpl>(
			dto, signer, signature, info, recipient, std::move(mosaics), message);
	}
	
	std::unique_ptr<AggregateTransaction>
	CreateAggregateTransactionImpl(bool isComplete,
	                               const EmbeddedTransactions& embeddedTransactions,
	                               const std::unordered_map<Key, Signature>& cosignatures,
	                               std::optional<Amount> maxFee,
	                               std::optional<NetworkDuration> deadline,
	                               std::optional<NetworkIdentifier> networkId,
	                               const std::optional<Key>& signer,
	                               const std::optional<Signature>& signature,
	                               const std::optional<TransactionInfo>& info)
	{
		AggregateTransactionDTO dto;
		InitAggregateTransactionDTO(
			dto, isComplete, embeddedTransactions, cosignatures, maxFee, deadline, networkId, signer, signature);
		

		return CreateTransaction<AggregateTransactionImpl>(
			dto, signer, signature, info,
			isComplete, embeddedTransactions, cosignatures, dto.value<"payloadSize"_>());
	}
}}



namespace xpx_sdk {
	
	std::unique_ptr<AccountLinkTransaction>
	CreateAccountLinkTransaction(AccountLinkTransactionAction action,
	                             const Key& remoteAccountKey,
	                             std::optional<Amount> maxFee,
	                             std::optional<NetworkDuration> deadline,
	                             std::optional<NetworkIdentifier> networkId)
	{
		return CreateAccountLinkTransactionImpl(action, remoteAccountKey, maxFee, deadline, networkId);
	}
	
	std::unique_ptr<EmbeddedAccountLinkTransaction>
	CreateEmbeddedAccountLinkTransaction(AccountLinkTransactionAction action,
	                                     const Key& remoteAccountKey,
	                                     const Key& signer,
	                                     std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedAccountLinkTransactionDTO dto;
		InitAccountLinkTransactionDTO(dto, action, remoteAccountKey, signer, networkId);
		return CreateTransaction<EmbeddedAccountLinkTransactionImpl>(dto, action, remoteAccountKey);
	}
	
	std::unique_ptr<AccountAddressPropertyTransaction>
	CreateAccountAddressPropertyTransaction(AccountPropertyRule propertyRule,
	                                        AccountAddressPropertyModifications propertyModifications,
	                                        std::optional<Amount> maxFee,
	                                        std::optional<NetworkDuration> deadline,
	                                        std::optional<NetworkIdentifier> networkId)
	{
		return CreateAccountAddressPropertyTransactionImpl(
			propertyRule, std::move(propertyModifications), maxFee, deadline, networkId);
	}
	
	std::unique_ptr<EmbeddedAccountAddressPropertyTransaction>
	CreateEmbeddedAccountAddressPropertyTransaction(AccountPropertyRule propertyRule,
	                                                AccountAddressPropertyModifications propertyModifications,
	                                                const Key& signer,
	                                                std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedAccountAddressPropertyTransactionDTO dto;
		InitAccountPropertyTransactionDTO<AccountAddressProperty>(
			dto, TransactionType::Address_Property, propertyRule, propertyModifications, signer, networkId);
		
		return CreateTransaction<EmbeddedAccountAddressPropertyTransactionImpl>(
			dto, propertyRule, std::move(propertyModifications));
	}
	
	std::unique_ptr<AccountMosaicPropertyTransaction>
	CreateAccountMosaicPropertyTransaction(AccountPropertyRule propertyRule,
	                                       AccountMosaicPropertyModifications propertyModifications,
	                                       std::optional<Amount> maxFee,
	                                       std::optional<NetworkDuration> deadline,
	                                       std::optional<NetworkIdentifier> networkId)
	{
		return CreateAccountMosaicPropertyTransactionImpl(
			propertyRule, std::move(propertyModifications), maxFee, deadline, networkId);
	}
	
	std::unique_ptr<EmbeddedAccountMosaicPropertyTransaction>
	CreateEmbeddedAccountMosaicPropertyTransaction(AccountPropertyRule propertyRule,
	                                               AccountMosaicPropertyModifications propertyModifications,
	                                               const Key& signer,
	                                               std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedAccountMosaicPropertyTransactionDTO dto;
		InitAccountPropertyTransactionDTO<AccountMosaicProperty>(
			dto, TransactionType::Mosaic_Property, propertyRule, propertyModifications, signer, networkId);

		return CreateTransaction<EmbeddedAccountMosaicPropertyTransactionImpl>(
			dto, propertyRule, std::move(propertyModifications));
	}
	
	std::unique_ptr<AccountTransactionPropertyTransaction>
	CreateAccountTransactionPropertyTransaction(AccountPropertyRule propertyRule,
	                                            AccountTransactionPropertyModifications propertyModifications,
	                                            std::optional<Amount> maxFee,
	                                            std::optional<NetworkDuration> deadline,
	                                            std::optional<NetworkIdentifier> networkId)
	{
		return CreateAccountTransactionPropertyTransactionImpl(
			propertyRule, std::move(propertyModifications), maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedAccountTransactionPropertyTransaction>
	CreateEmbeddedAccountTransactionPropertyTransaction(AccountPropertyRule propertyRule,
	                                                    AccountTransactionPropertyModifications propertyModifications,
	                                                    const Key& signer,
	                                                    std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedAccountTransactionPropertyTransactionDTO dto;
		InitAccountPropertyTransactionDTO<AccountTransactionProperty>(
			dto, TransactionType::Transaction_Property, propertyRule, propertyModifications, signer, networkId);

		return CreateTransaction<EmbeddedAccountTransactionPropertyTransactionImpl>(
			dto, propertyRule, std::move(propertyModifications));
	}
	
	std::unique_ptr<EmbeddedAddressAliasTransaction>
	CreateEmbeddedAddressAliasTransaction(AliasTransactionAction action,
	                                      NamespaceId aliasNamespaceId,
	                                      const Address& aliasedAddress,
	                                      const Key& signer,
	                                      std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedAddressAliasTransactionDTO dto;
		InitAddressAliasTransactionDTO(dto, action, aliasNamespaceId, aliasedAddress, signer, networkId);
		return CreateTransaction<EmbeddedAddressAliasTransactionImpl>(dto, action, aliasNamespaceId, aliasedAddress);
	}

	std::unique_ptr<MosaicAliasTransaction>
	CreateMosaicAliasTransaction(AliasTransactionAction action,
	                             NamespaceId aliasNamespaceId,
	                             MosaicId aliasedMosaicId,
	                             std::optional<Amount> maxFee,
	                             std::optional<NetworkDuration> deadline,
	                             std::optional<NetworkIdentifier> networkId)
	{
		return CreateMosaicAliasTransactionImpl(action, aliasNamespaceId, aliasedMosaicId, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedMosaicAliasTransaction>
	CreateEmbeddedMosaicAliasTransaction(AliasTransactionAction action,
	                                     NamespaceId aliasNamespaceId,
	                                     MosaicId aliasedMosaicId,
	                                     const Key& signer,
	                                     std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedMosaicAliasTransactionDTO dto;
		InitMosaicAliasTransactionDTO(dto, action, aliasNamespaceId, aliasedMosaicId, signer, networkId);
		return CreateTransaction<EmbeddedMosaicAliasTransactionImpl>(dto, action, aliasNamespaceId, aliasedMosaicId);
	}

	std::unique_ptr<LockFundsTransaction>
	CreateLockFundsTransaction(const Mosaic& lockedMosaic,
	                           BlockDuration lockDuration,
	                           const Hash256& lockHash,
	                           std::optional<Amount> maxFee,
	                           std::optional<NetworkDuration> deadline,
	                           std::optional<NetworkIdentifier> networkId)
	{
		return CreateLockFundsTransactionImpl(lockedMosaic, lockDuration, lockHash, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedLockFundsTransaction>
	CreateEmbeddedLockFundsTransaction(const Mosaic& lockedMosaic,
	                                   BlockDuration lockDuration,
	                                   const Hash256& lockHash,
	                                   const Key& signer,
	                                   std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedLockFundsTransactionDTO dto;
		InitLockFundsTransactionDTO(dto, lockedMosaic, lockDuration, lockHash, signer, networkId);
		return CreateTransaction<EmbeddedLockFundsTransactionImpl>(dto, lockedMosaic, lockDuration, lockHash);
	}

	std::unique_ptr<ModifyMultisigAccountTransaction>
	CreateModifyMultisigAccountTransaction(int8_t minRemovalDelta,
	                                       int8_t minApprovalDelta,
	                                       const std::unordered_map<Key, CosignatoryModificationType>& cosignatoryModifications,
	                                       std::optional<Amount> maxFee,
	                                       std::optional<NetworkDuration> deadline,
	                                       std::optional<NetworkIdentifier> networkId)
	{
		return CreateModifyMultisigAccountTransactionImpl(
			minRemovalDelta, minApprovalDelta, cosignatoryModifications, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedModifyMultisigAccountTransaction>
	CreateEmbeddedModifyMultisigAccountTransaction(int8_t minRemovalDelta,
	                                               int8_t minApprovalDelta,
	                                               const std::unordered_map<Key, CosignatoryModificationType>& cosignatoryModifications,
	                                               const Key& signer,
	                                               std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedModifyMultisigAccountTransactionDTO dto;
		InitModifyMultisigAccountTransactionDTO(
			dto, minRemovalDelta, minApprovalDelta, cosignatoryModifications, signer, networkId);

		return CreateTransaction<EmbeddedModifyMultisigAccountTransactionImpl>(
			dto, minRemovalDelta, minApprovalDelta, cosignatoryModifications);
	}

	std::unique_ptr<MosaicDefinitionTransaction>
	CreateMosaicDefinitionTransaction(uint32_t mosaicNonce,
	                                  MosaicId mosaicId,
	                                  MosaicFlags flags,
	                                  MosaicProperties mosaicProperties,
	                                  std::optional<Amount> maxFee,
	                                  std::optional<NetworkDuration> deadline,
	                                  std::optional<NetworkIdentifier> networkId)
	{
		return CreateMosaicDefinitionTransactionImpl(
			mosaicNonce, mosaicId, flags, std::move(mosaicProperties), maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedMosaicDefinitionTransaction>
	CreateEmbeddedMosaicDefinitionTransaction(uint32_t mosaicNonce,
	                                          MosaicId mosaicId,
	                                          MosaicFlags flags,
	                                          MosaicProperties mosaicProperties,
	                                          const Key& signer,
	                                          std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedMosaicDefinitionTransactionDTO dto;
		InitMosaicDefinitionTransactionDTO(dto, mosaicNonce, mosaicId, flags, mosaicProperties, signer, networkId);

		return CreateTransaction<EmbeddedMosaicDefinitionTransactionImpl>(
			dto, mosaicNonce, mosaicId, flags, std::move(mosaicProperties));
	}

	std::unique_ptr<MosaicSupplyChangeTransaction>
	CreateMosaicSupplyChangeTransaction(MosaicId mosaicId,
	                                    MosaicSupplyChangeDirection changeDirection,
	                                    Amount changeDelta,
	                                    std::optional<Amount> maxFee,
	                                    std::optional<NetworkDuration> deadline,
	                                    std::optional<NetworkIdentifier> networkId)
	{
		return CreateMosaicSupplyChangeTransactionImpl(mosaicId, changeDirection, changeDelta, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedMosaicSupplyChangeTransaction>
	CreateEmbeddedMosaicSupplyChangeTransaction(MosaicId mosaicId,
	                                            MosaicSupplyChangeDirection changeDirection,
	                                            Amount changeDelta,
	                                            const Key& signer,
	                                            std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedMosaicSupplyChangeTransactionDTO dto;
		InitMosaicSupplyChangeTransactionDTO(dto, mosaicId, changeDirection, changeDelta, signer, networkId);
		return CreateTransaction<EmbeddedMosaicSupplyChangeTransactionImpl>(dto, mosaicId, changeDirection, changeDelta);
	}

	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterRootNamespaceTransaction(std::string_view namespaceName,
	                                       BlockDuration namespaceDuration,
	                                       std::optional<Amount> maxFee,
	                                       std::optional<NetworkDuration> deadline,
	                                       std::optional<NetworkIdentifier> networkId)
	{
		return CreateRegisterRootNamespaceTransactionImpl(namespaceName, namespaceDuration, maxFee, deadline, networkId);
	}

	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterChildNamespaceTransaction(std::string_view namespaceName,
	                                        NamespaceId namespaceParentId,
	                                        std::optional<Amount> maxFee,
	                                        std::optional<NetworkDuration> deadline,
	                                        std::optional<NetworkIdentifier> networkId)
	{
		return CreateRegisterChildNamespaceTransactionImpl(namespaceName, namespaceParentId, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedRegisterNamespaceTransaction>
	CreateEmbeddedRegisterRootNamespaceTransaction(std::string_view namespaceName,
	                                               BlockDuration namespaceDuration,
	                                               const Key& signer,
	                                               std::optional<NetworkIdentifier> networkId)
	{
		Namespace ns(namespaceName);

		EmbeddedRegisterNamespaceTransactionDTO dto;
		InitRegisterNamespaceTransactionDTO(dto, ns, namespaceDuration, signer, networkId);
		return CreateTransaction<EmbeddedRegisterNamespaceTransactionImpl>(dto, std::move(ns), namespaceDuration);
	}

	std::unique_ptr<EmbeddedRegisterNamespaceTransaction>
	CreateEmbeddedRegisterChildNamespaceTransaction(std::string_view namespaceName,
	                                                NamespaceId namespaceParentId,
	                                                const Key& signer,
	                                                std::optional<NetworkIdentifier> networkId)
	{
		Namespace ns(namespaceName, namespaceParentId);

		EmbeddedRegisterNamespaceTransactionDTO dto;
		InitRegisterNamespaceTransactionDTO(dto, ns, namespaceParentId, signer, networkId);
		return CreateTransaction<EmbeddedRegisterNamespaceTransactionImpl>(dto, std::move(ns), 0);
	}

	std::unique_ptr<SecretLockTransaction>
	CreateSecretLockTransaction(SecretHashAlgorithm secretHashAlgorithm,
	                            const Hash256& secretHash,
	                            const Mosaic& lockedMosaic,
	                            BlockDuration lockDuration,
	                            const Address& lockedMosaicRecipient,
	                            std::optional<Amount> maxFee,
	                            std::optional<NetworkDuration> deadline,
	                            std::optional<NetworkIdentifier> networkId)
	{
		return CreateSecretLockTransactionImpl(
			secretHashAlgorithm, secretHash, lockedMosaic, lockDuration, lockedMosaicRecipient,
			maxFee, deadline, networkId);
	}

	std::unique_ptr<SecretLockTransaction>
	CreateSecretLockTransactionFromSecret(SecretHashAlgorithm secretHashAlgorithm,
	                                      RawBuffer secret,
	                                      const Mosaic& lockedMosaic,
	                                      BlockDuration lockDuration,
	                                      const Address& lockedMosaicRecipient,
	                                      std::optional<Amount> maxFee,
	                                      std::optional<NetworkDuration> deadline,
	                                      std::optional<NetworkIdentifier> networkId)
	{
		return CreateSecretLockTransaction(
			secretHashAlgorithm, CalculateSecretHash(secret, secretHashAlgorithm),
			lockedMosaic, lockDuration, lockedMosaicRecipient, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedSecretLockTransaction>
	CreateEmbeddedSecretLockTransaction(SecretHashAlgorithm secretHashAlgorithm,
	                                    const Hash256& secretHash,
	                                    const Mosaic& lockedMosaic,
	                                    BlockDuration lockDuration,
	                                    const Address& lockedMosaicRecipient,
	                                    const Key& signer,
	                                    std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedSecretLockTransactionDTO dto;
		InitSecretLockTransactionDTO(
			dto, secretHashAlgorithm, secretHash, lockedMosaic, lockDuration, lockedMosaicRecipient,
			signer, networkId);

		return CreateTransaction<EmbeddedSecretLockTransactionImpl>(
			dto, secretHashAlgorithm, secretHash, lockedMosaic, lockDuration, lockedMosaicRecipient);
	}

	std::unique_ptr<EmbeddedSecretLockTransaction>
	CreateEmbeddedSecretLockTransactionFromSecret(SecretHashAlgorithm secretHashAlgorithm,
	                                              RawBuffer secret,
	                                              const Mosaic& lockedMosaic,
	                                              BlockDuration lockDuration,
	                                              const Address& lockedMosaicRecipient,
	                                              const Key& signer,
	                                              std::optional<NetworkIdentifier> networkId)
	{
		return CreateEmbeddedSecretLockTransaction(
			secretHashAlgorithm, CalculateSecretHash(secret, secretHashAlgorithm),
			lockedMosaic, lockDuration, lockedMosaicRecipient, signer, networkId);
	}

	std::unique_ptr<SecretProofTransaction>
	CreateSecretProofTransaction(SecretHashAlgorithm secretHashAlgorithm,
	                             RawBuffer secret,
	                             std::optional<Amount> maxFee,
	                             std::optional<NetworkDuration> deadline,
	                             std::optional<NetworkIdentifier> networkId)
	{
		return CreateSecretProofTransactionImpl(
			secretHashAlgorithm, CalculateSecretHash(secret, secretHashAlgorithm), secret, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedSecretProofTransaction>
	CreateEmbeddedSecretProofTransaction(SecretHashAlgorithm secretHashAlgorithm,
	                                     RawBuffer secret,
	                                     const Key& signer,
	                                     std::optional<NetworkIdentifier> networkId)
	{
		auto secretHash = CalculateSecretHash(secret, secretHashAlgorithm);

		EmbeddedSecretProofTransactionDTO dto;
		InitSecretProofTransactionDTO(dto, secretHashAlgorithm, secretHash, secret, signer, networkId);
		return CreateTransaction<EmbeddedSecretProofTransactionImpl>(dto, secretHashAlgorithm, secretHash, secret);
	}

	std::unique_ptr<TransferTransaction>
	CreateTransferTransaction(const Address& recipient,
	                          MosaicContainer mosaics,
	                          RawBuffer message,
	                          std::optional<Amount> maxFee,
	                          std::optional<NetworkDuration> deadline,
	                          std::optional<NetworkIdentifier> networkId)
	{
		return CreateTransferTransactionImpl(recipient, mosaics, message, maxFee, deadline, networkId);
	}

	std::unique_ptr<EmbeddedTransferTransaction>
	CreateEmbeddedTransferTransaction(const Address& recipient,
	                                  MosaicContainer mosaics,
	                                  RawBuffer message,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId)
	{
		EmbeddedTransferTransactionDTO dto;
		InitTransferTransactionDTO(dto, recipient, mosaics, message, signer, networkId);
		return CreateTransaction<EmbeddedTransferTransactionImpl>(dto, recipient, mosaics, message);
	}

	std::unique_ptr<AggregateTransaction>
	CreateCompleteAggregateTransaction(const EmbeddedTransactions& embeddedTransactions,
	                                   const std::unordered_map<Key, Signature>& cosignatures,
	                                   std::optional<Amount> maxFee,
	                                   std::optional<NetworkDuration> deadline,
	                                   std::optional<NetworkIdentifier> networkId)
	{
		return CreateAggregateTransactionImpl(true, embeddedTransactions, cosignatures, maxFee, deadline, networkId);
	}

	std::unique_ptr<AggregateTransaction>
	CreateBondedAggregateTransaction(const EmbeddedTransactions& embeddedTransactions,
	                                 const std::unordered_map<Key, Signature>& cosignatures,
	                                 std::optional<Amount> maxFee,
	                                 std::optional<NetworkDuration> deadline,
	                                 std::optional<NetworkIdentifier> networkId)
	{
		return CreateAggregateTransactionImpl(false, embeddedTransactions, cosignatures, maxFee, deadline, networkId);
	}
}
