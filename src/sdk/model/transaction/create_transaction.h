
#pragma once

#include <nemcpp/model/transaction/account_link_transaction.h>
#include <nemcpp/model/transaction/account_property_transaction.h>
#include <nemcpp/model/transaction/address_alias_transaction.h>
#include <nemcpp/model/transaction/aggregate_transaction.h>
#include <nemcpp/model/transaction/lock_funds_transaction.h>
#include <nemcpp/model/transaction/modify_multisig_account_transaction.h>
#include <nemcpp/model/transaction/mosaic_alias_transaction.h>
#include <nemcpp/model/transaction/mosaic_definition_transaction.h>
#include <nemcpp/model/transaction/mosaic_supply_change_transaction.h>
#include <nemcpp/model/transaction/register_namespace_transaction.h>
#include <nemcpp/model/transaction/secret_lock_transaction.h>
#include <nemcpp/model/transaction/secret_proof_transaction.h>
#include <nemcpp/model/transaction/transfer_transaction.h>

#include <utility>
using namespace xpx_sdk::difficult_transactions;
namespace xpx_sdk { namespace internal {
	
	template<typename TTransaction>
	class TTransactionImpl: public TTransaction {
	public:
		template<typename... TArgs>
		TTransactionImpl(TArgs&&... args): TTransaction(std::forward<TArgs>(args)...)
		{ }
		
		~TTransactionImpl() override
		{ }
	};
	
	using AccountLinkTransactionImpl = TTransactionImpl<AccountLinkTransaction>;
	using EmbeddedAccountLinkTransactionImpl = TTransactionImpl<EmbeddedAccountLinkTransaction>;
	
	using AccountAddressPropertyTransactionImpl = TTransactionImpl<AccountAddressPropertyTransaction>;
	using EmbeddedAccountAddressPropertyTransactionImpl = TTransactionImpl<EmbeddedAccountAddressPropertyTransaction>;
	
	using AccountMosaicPropertyTransactionImpl = TTransactionImpl<AccountMosaicPropertyTransaction>;
	using EmbeddedAccountMosaicPropertyTransactionImpl = TTransactionImpl<EmbeddedAccountMosaicPropertyTransaction>;
	
	using AccountTransactionPropertyTransactionImpl = TTransactionImpl<AccountTransactionPropertyTransaction>;
	using EmbeddedAccountTransactionPropertyTransactionImpl = TTransactionImpl<EmbeddedAccountTransactionPropertyTransaction>;
	
	using AddressAliasTransactionImpl = TTransactionImpl<AddressAliasTransaction>;
	using EmbeddedAddressAliasTransactionImpl = TTransactionImpl<EmbeddedAddressAliasTransaction>;
	
	using MosaicAliasTransactionImpl = TTransactionImpl<MosaicAliasTransaction>;
	using EmbeddedMosaicAliasTransactionImpl = TTransactionImpl<EmbeddedMosaicAliasTransaction>;
	
	using AggregateTransactionImpl = TTransactionImpl<AggregateTransaction>;
	
	using LockFundsTransactionImpl = TTransactionImpl<LockFundsTransaction>;
	using EmbeddedLockFundsTransactionImpl = TTransactionImpl<EmbeddedLockFundsTransaction>;
	
	using ModifyMultisigAccountTransactionImpl = TTransactionImpl<ModifyMultisigAccountTransaction>;
	using EmbeddedModifyMultisigAccountTransactionImpl = TTransactionImpl<EmbeddedModifyMultisigAccountTransaction>;
	
	using MosaicDefinitionTransactionImpl = TTransactionImpl<MosaicDefinitionTransaction>;
	using EmbeddedMosaicDefinitionTransactionImpl = TTransactionImpl<EmbeddedMosaicDefinitionTransaction>;
	
	using MosaicSupplyChangeTransactionImpl = TTransactionImpl<MosaicSupplyChangeTransaction>;
	using EmbeddedMosaicSupplyChangeTransactionImpl = TTransactionImpl<EmbeddedMosaicSupplyChangeTransaction>;
	
	using RegisterNamespaceTransactionImpl = TTransactionImpl<RegisterNamespaceTransaction>;
	using EmbeddedRegisterNamespaceTransactionImpl = TTransactionImpl<EmbeddedRegisterNamespaceTransaction>;
	
	using SecretLockTransactionImpl = TTransactionImpl<SecretLockTransaction>;
	using EmbeddedSecretLockTransactionImpl = TTransactionImpl<EmbeddedSecretLockTransaction>;
	
	using SecretProofTransactionImpl = TTransactionImpl<SecretProofTransaction>;
	using EmbeddedSecretProofTransactionImpl = TTransactionImpl<EmbeddedSecretProofTransaction>;
	
	using TransferTransactionImpl = TTransactionImpl<TransferTransaction>;
	using EmbeddedTransferTransactionImpl = TTransactionImpl<EmbeddedTransferTransaction>;
	
	
	
	std::unique_ptr<AccountLinkTransaction>
	CreateAccountLinkTransactionImpl(AccountLinkTransactionAction action,
	                                 const Key& remoteAccountKey,
	                                 std::optional<Amount> maxFee,
	                                 std::optional<NetworkDuration> deadline,
	                                 std::optional<NetworkIdentifier> networkId,
	                                 const std::optional<Key>& signer = std::nullopt,
	                                 const std::optional<Signature>& signature = std::nullopt,
	                                 const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<AccountAddressPropertyTransaction>
	CreateAccountAddressPropertyTransactionImpl(AccountPropertyRule propertyRule,
	                                            AccountAddressPropertyModifications propertyModifications,
	                                            std::optional<Amount> maxFee,
	                                            std::optional<NetworkDuration> deadline,
	                                            std::optional<NetworkIdentifier> networkId,
	                                            const std::optional<Key>& signer = std::nullopt,
	                                            const std::optional<Signature>& signature = std::nullopt,
	                                            const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<AccountMosaicPropertyTransaction>
	CreateAccountMosaicPropertyTransactionImpl(AccountPropertyRule propertyRule,
	                                           AccountMosaicPropertyModifications propertyModifications,
	                                           std::optional<Amount> maxFee,
	                                           std::optional<NetworkDuration> deadline,
	                                           std::optional<NetworkIdentifier> networkId,
	                                           const std::optional<Key>& signer = std::nullopt,
	                                           const std::optional<Signature>& signature = std::nullopt,
	                                           const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<AccountTransactionPropertyTransaction>
	CreateAccountTransactionPropertyTransactionImpl(AccountPropertyRule propertyRule,
	                                                AccountTransactionPropertyModifications propertyModifications,
	                                                std::optional<Amount> maxFee,
	                                                std::optional<NetworkDuration> deadline,
	                                                std::optional<NetworkIdentifier> networkId,
	                                                const std::optional<Key>& signer = std::nullopt,
	                                                const std::optional<Signature>& signature = std::nullopt,
	                                                const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<AddressAliasTransaction>
	CreateAddressAliasTransactionImpl(AliasTransactionAction action,
	                                  NamespaceId aliasNamespaceId,
	                                  const Address& aliasedAddress,
	                                  std::optional<Amount> maxFee,
	                                  std::optional<NetworkDuration> deadline,
	                                  std::optional<NetworkIdentifier> networkId,
	                                  const std::optional<Key>& signer = std::nullopt,
	                                  const std::optional<Signature>& signature = std::nullopt,
	                                  const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<MosaicAliasTransaction>
	CreateMosaicAliasTransactionImpl(AliasTransactionAction action,
	                                 NamespaceId aliasNamespaceId,
	                                 MosaicId aliasedMosaicId,
	                                 std::optional<Amount> maxFee,
	                                 std::optional<NetworkDuration> deadline,
	                                 std::optional<NetworkIdentifier> networkId,
	                                 const std::optional<Key>& signer = std::nullopt,
	                                 const std::optional<Signature>& signature = std::nullopt,
	                                 const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<LockFundsTransaction>
	CreateLockFundsTransactionImpl(const Mosaic& lockedMosaic,
	                               BlockDuration lockDuration,
	                               const Hash256& lockHash,
	                               std::optional<Amount> maxFee,
	                               std::optional<NetworkDuration> deadline,
	                               std::optional<NetworkIdentifier> networkId,
	                               const std::optional<Key>& signer = std::nullopt,
	                               const std::optional<Signature>& signature = std::nullopt,
	                               const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<ModifyMultisigAccountTransaction>
	CreateModifyMultisigAccountTransactionImpl(int8_t minRemovalDelta,
	                                           int8_t minApprovalDelta,
	                                           const std::unordered_map<Key, CosignatoryModificationType>&
	                                               cosignatoryModifications,
	                                           std::optional<Amount> maxFee,
	                                           std::optional<NetworkDuration> deadline,
	                                           std::optional<NetworkIdentifier> networkId,
	                                           const std::optional<Key>& signer = std::nullopt,
	                                           const std::optional<Signature>& signature = std::nullopt,
	                                           const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<MosaicDefinitionTransaction>
	CreateMosaicDefinitionTransactionImpl(uint32_t mosaicNonce,
	                                      MosaicId mosaicId,
	                                      MosaicProperties mosaicProperties,
	                                      std::optional<Amount> maxFee,
	                                      std::optional<NetworkDuration> deadline,
	                                      std::optional<NetworkIdentifier> networkId,
	                                      const std::optional<Key>& signer = std::nullopt,
	                                      const std::optional<Signature>& signature = std::nullopt,
	                                      const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<MosaicSupplyChangeTransaction>
	CreateMosaicSupplyChangeTransactionImpl(MosaicId mosaicId,
	                                        MosaicSupplyChangeDirection changeDirection,
	                                        Amount changeDelta,
	                                        std::optional<Amount> maxFee,
	                                        std::optional<NetworkDuration> deadline,
	                                        std::optional<NetworkIdentifier> networkId,
	                                        const std::optional<Key>& signer = std::nullopt,
	                                        const std::optional<Signature>& signature = std::nullopt,
	                                        const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterRootNamespaceTransactionImpl(std::string_view namespaceName,
	                                           BlockDuration namespaceDuration,
	                                           std::optional<Amount> maxFee,
	                                           std::optional<NetworkDuration> deadline,
	                                           std::optional<NetworkIdentifier> networkId,
	                                           const std::optional<Key>& signer = std::nullopt,
	                                           const std::optional<Signature>& signature = std::nullopt,
	                                           const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<RegisterNamespaceTransaction>
	CreateRegisterChildNamespaceTransactionImpl(std::string_view namespaceName,
	                                            NamespaceId namespaceParentId,
	                                            std::optional<Amount> maxFee,
	                                            std::optional<NetworkDuration> deadline,
	                                            std::optional<NetworkIdentifier> networkId,
	                                            const std::optional<Key>& signer = std::nullopt,
	                                            const std::optional<Signature>& signature = std::nullopt,
	                                            const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<SecretLockTransaction>
	CreateSecretLockTransactionImpl(SecretHashAlgorithm secretHashAlgorithm,
	                                const Hash256& secretHash,
	                                const Mosaic& lockedMosaic,
	                                BlockDuration lockDuration,
	                                const Address& lockedMosaicRecipient,
	                                std::optional<Amount> maxFee,
	                                std::optional<NetworkDuration> deadline,
	                                std::optional<NetworkIdentifier> networkId,
	                                const std::optional<Key>& signer = std::nullopt,
	                                const std::optional<Signature>& signature = std::nullopt,
	                                const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<SecretProofTransaction>
	CreateSecretProofTransactionImpl(SecretHashAlgorithm secretHashAlgorithm,
	                                 const Hash256& secretHash,
	                                 RawBuffer secret,
	                                 std::optional<Amount> maxFee,
	                                 std::optional<NetworkDuration> deadline,
	                                 std::optional<NetworkIdentifier> networkId,
	                                 const std::optional<Key>& signer = std::nullopt,
	                                 const std::optional<Signature>& signature = std::nullopt,
	                                 const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<TransferTransaction>
	CreateTransferTransactionImpl(const Address& recipient,
	                              MosaicContainer mosaics,
	                              RawBuffer message,
	                              std::optional<Amount> maxFee,
	                              std::optional<NetworkDuration> deadline,
	                              std::optional<NetworkIdentifier> networkId,
	                              const std::optional<Key>& signer = std::nullopt,
	                              const std::optional<Signature>& signature = std::nullopt,
	                              const std::optional<TransactionInfo>& info = std::nullopt);
	
	std::unique_ptr<AggregateTransaction>
	CreateAggregateTransactionImpl(bool isComplete,
	                               const EmbeddedTransactions& embeddedTransactions,
	                               const std::unordered_map<Key, Signature>& cosignatures,
	                               std::optional<Amount> maxFee,
	                               std::optional<NetworkDuration> deadline,
	                               std::optional<NetworkIdentifier> networkId,
	                               const std::optional<Key>& signer = std::nullopt,
	                               const std::optional<Signature>& signature = std::nullopt,
	                               const std::optional<TransactionInfo>& info = std::nullopt);
}}
