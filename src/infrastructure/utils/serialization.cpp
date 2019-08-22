////
//// Created by vulfke on 16.08.19.
////
//
//#include "serialization.h"
//
//using namespace xpx_sdk;
//using namespace internal;
//using namespace json;
//using namespace dto;
//
//using namespace xpx_sdk::internal::json::dto;
//
//
//#define COMPOSE_TRANSACTION(transaction, dto) \
//dto.set<"size"_>(transaction.size); \
//dto.set<"signature"_>(transaction.signature); \
//dto.set<"signer"_>(transaction.signer); \
//dto.set<"version"_>(transaction.version); \
//dto.set<"type"_>(transaction.type); \
//dto.set<"maxFee"_>(transaction.maxFee); \
//dto.set<"deadline"_>(transaction.deadline);
//
//#define COMPOSE_EMBEDDED_TRANSACTION(transaction, dto) \
//dto.set<"size"_>(transaction.size); \
//dto.set<"signer"_>(transaction.signer); \
//dto.set<"version"_>(transaction.version); \
//dto.set<"type"_>(transaction.type);
//
//// Transactions
//
//namespace xpx_sdk::internal::json::dto {
//	template<>
//	CosignatureDto toDto<Cosignature, CosignatureDto>(const Cosignature &transaction) {
//		CosignatureDto dto;
//
//		dto.set<"publicKey"_>(transaction.publicKey);
//		dto.set<"signature"_>(transaction.signature);
//
//		return dto;
//	}
//
//	template<>
//	MosaicDto toDto<Mosaic, MosaicDto>(const Mosaic& mosaic) {
//		MosaicDto dto;
//		dto.set<"id"_>(mosaic.id);
//		dto.set<"amount"_>(mosaic.amount);
//		return dto;
//	}
//
//	template<>
//	CosignatoryModificationDto
//	toDto<CosignatoryModification, CosignatoryModificationDto>(const CosignatoryModification &transaction) {
//		CosignatoryModificationDto dto;
//		dto.set<"publicKey"_>(transaction.publicKey);
//		dto.set<"modificationType"_>(transaction.modificationType);
//		return dto;
//	}
//
//	template<>
//	TransactionPropertyModificationDto
//	toDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(
//			const AccountTransactionPropertyModification &transaction) {
//		TransactionPropertyModificationDto dto;
//
//		dto.set<"value"_>(transaction.value);
//		dto.set<"modificationType"_>(transaction.modificationType);
//
//		return dto;
//	}
//
//	template<>
//	MosaicPropertyModificationDto toDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(
//			const AccountMosaicPropertyModification &transaction) {
//		MosaicPropertyModificationDto dto;
//		dto.set<"value"_>(transaction.value);
//		dto.set<"modificationType"_>(transaction.modificationType);
//		return dto;
//	}
//
//	template<>
//	AddressPropertyModificationDto toDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(
//			const AccountAddressPropertyModification &transaction) {
//		AddressPropertyModificationDto dto;
//		dto.set<"value"_>(transaction.value);
//		dto.set<"modificationType"_>(transaction.modificationType);
//		return dto;
//	}
//
//	template<>
//	MosaicPropertyDto toDto<MosaicProperty, MosaicPropertyDto>(const MosaicProperty &transaction) {
//		MosaicPropertyDto dto;
//		dto.set<"id"_>(transaction.id);
//		dto.set<"value"_>(transaction.value);
//
//		return dto;
//	}
//
//	template<>
//	TransactionDto toDto<Transaction, TransactionDto>(const Transaction &transaction) {
//		TransactionDto dto;
//		COMPOSE_TRANSACTION(transaction, dto)
//		return dto;
//	}
//
//	template<>
//	EmbeddedTransactionDto toDto<EmbeddedTransaction, EmbeddedTransactionDto>(const EmbeddedTransaction &transaction) {
//		EmbeddedTransactionDto dto;
//		COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
//		return dto;
//	}
//
//	template<>
//	AggregateTransactionDto
//	toDto<AggregateTransaction, AggregateTransactionDto>(const AggregateTransaction &transaction) {
//		AggregateTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"payloadSize"_>(transaction.payloadSize);
////		dto.set<"payload"_>(transaction.payload);
//		std::vector<CosignatureDto> cosignatureDtos;
//		for (auto &cosignature : transaction.cosignatures) {
//			cosignatureDtos.push_back(toDto<Cosignature, CosignatureDto>(cosignature));
//		}
//		dto.set<"cosignatures"_>(std::move(cosignatureDtos));
//		return dto;
//	}
//
//
//	template<>
//	AccountLinkTransactionDto
//	toDto<AccountLinkTransaction, AccountLinkTransactionDto>(const AccountLinkTransaction &transaction) {
//		AccountLinkTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"linkAction"_>(transaction.linkAction);
//		dto.set<"remoteAccountKey"_>(transaction.remoteAccountKey);
//		return dto;
//	}
//
//
//	template<>
//	LockFundsTransactionDto
//	toDto<LockFundsTransaction, LockFundsTransactionDto>(const LockFundsTransaction &transaction) {
//		LockFundsTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"lockedMosaic"_>(toDto<Mosaic, MosaicDto>(transaction.lockedMosaic));
//		dto.set<"lockDuration"_>(transaction.lockDuration);
//		dto.set<"lockHash"_>(transaction.lockHash);
//		return dto;
//	}
//
//
//	template<>
//	ModifyMultisigAccountTransactionDto toDto<ModifyMultisigAccountTransaction, ModifyMultisigAccountTransactionDto>(
//			const ModifyMultisigAccountTransaction &transaction) {
//
//		ModifyMultisigAccountTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"minRemovalDelta"_>(transaction.minRemovalDelta);
//		dto.set<"minApprovalDelta"_>(transaction.minApprovalDelta);
//		dto.set<"modificationsCount"_>(transaction.modificationsCount);
//
//		std::vector<CosignatoryModificationDto> cosModificationsDtos;
//		for (auto &cosignatoryModification : transaction.modifications) {
//			cosModificationsDtos.push_back(
//					toDto<CosignatoryModification, CosignatoryModificationDto>(cosignatoryModification));
//		}
//		dto.set<"modifications"_>(std::move(cosModificationsDtos));
//		return dto;
//	}
//
//
//	template<>
//	MosaicDefinitionTransactionDto
//	toDto<MosaicDefinitionTransaction, MosaicDefinitionTransactionDto>(const MosaicDefinitionTransaction &transaction) {
//		MosaicDefinitionTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//
//		dto.set<"nonce"_>(transaction.nonce);
//		dto.set<"mosaicId"_>(transaction.mosaicId);
//		dto.set<"optionalPropertiesCount"_>(transaction.optionalPropertiesCount);
//		dto.set<"flags"_>(transaction.flags);
//		dto.set<"divisibility"_>(transaction.divisibility);
//		std::vector<MosaicPropertyDto> mosaicPropertyDtos;
//		for (auto &mosaicProperty : transaction.optionalProperties) {
//			mosaicPropertyDtos.push_back(toDto<MosaicProperty, MosaicPropertyDto>(mosaicProperty));
//		}
//		dto.set<"optionalProperties"_>(std::move(mosaicPropertyDtos));
//		return dto;
//	}
//
//
//	template<>
//	MosaicSupplyChangeTransactionDto toDto<MosaicSupplyChangeTransaction, MosaicSupplyChangeTransactionDto>(
//			const MosaicSupplyChangeTransaction &transaction) {
//		MosaicSupplyChangeTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"mosaicId"_>(transaction.mosaicId);
//		dto.set<"direction"_>(transaction.direction);
//		dto.set<"delta"_>(transaction.delta);
//		return dto;
//	}
//
//
//	template<>
//	RegisterNamespaceTransactionDto toDto<RegisterNamespaceTransaction, RegisterNamespaceTransactionDto>(
//			const RegisterNamespaceTransaction &transaction) {
//		RegisterNamespaceTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"namespaceType"_>(transaction.namespaceType);
//		dto.set<"durationOrParentId"_>(transaction.durationOrParentId);
//		dto.set<"namespaceId"_>(transaction.namespaceId);
//		dto.set<"namespaceNameSize"_>(transaction.namespaceNameSize);
//		dto.set<"namespaceName"_>(transaction.namespaceName);
//
//		return dto;
//	}
//
//	template<>
//	SecretLockTransactionDto
//	toDto<SecretLockTransaction, SecretLockTransactionDto>(const SecretLockTransaction &transaction) {
//		SecretLockTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"mosaicId"_>(transaction.mosaicId);
//		dto.set<"amount"_>(transaction.amount);
//		dto.set<"duration"_>(transaction.duration);
//		dto.set<"hashAlgorithm"_>(transaction.hashAlgorithm);
//		dto.set<"secret"_>(transaction.secret);
//		dto.set<"recipient"_>(transaction.recipient);
//
//
//		return dto;
//	}
//
//
//	template<>
//	SecretProofTransactionDto
//	toDto<SecretProofTransaction, SecretProofTransactionDto>(const SecretProofTransaction &transaction) {
//		SecretProofTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"hashAlgorithm"_>(transaction.hashAlgorithm);
//		dto.set<"secret"_>(transaction.secret);
//		dto.set<"proofSize"_>(transaction.proofSize);
//		dto.set<"proof"_>(transaction.proof);
//
//		return dto;
//	}
//
//
//	template<>
//	TransferTransactionDto toDto<TransferTransaction, TransferTransactionDto>(const TransferTransaction &transaction) {
//		TransferTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"recipient"_>(transaction.recipient);
//		dto.set<"messageSize"_>(transaction.messageSize);
//		dto.set<"mosaicsCount"_>(transaction.mosaicsCount);
//		dto.set<"message"_>(transaction.message);
//
//		std::vector<MosaicDto> mosaicDtos;
//		for (auto &mosaic : transaction.mosaics) {
//			mosaicDtos.push_back(toDto<Mosaic, MosaicDto>(mosaic));
//		}
//		dto.set<"mosaics"_>(std::move(mosaicDtos));
//		return dto;
//	}
//
//
//	template<>
//	AliasTransactionBaseDto
//	toDto<AliasTransactionBase, AliasTransactionBaseDto>(const AliasTransactionBase &transaction) {
//		AliasTransactionBaseDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"aliasAction"_>(transaction.aliasAction);
//		dto.set<"namespaceId"_>(transaction.namespaceId);
//
//		return dto;
//	}
//
//	template<>
//	AddressAliasTransactionDto
//	toDto<AddressAliasTransaction, AddressAliasTransactionDto>(const AddressAliasTransaction &transaction) {
//		AddressAliasTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"address"_>(transaction.address);
//		return dto;
//	}
//
//	template<>
//	MosaicAliasTransactionDto
//	toDto<MosaicAliasTransaction, MosaicAliasTransactionDto>(const MosaicAliasTransaction &transaction) {
//		MosaicAliasTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"mosaicId"_>(transaction.mosaicId);
//		return dto;
//	}
//
///// Account Property Transactions
//	template<>
//	AccountTransactionPropertyTransactionDto
//	toDto<AccountTransactionPropertyTransaction, AccountTransactionPropertyTransactionDto>(
//			const AccountTransactionPropertyTransaction &transaction) {
//		AccountTransactionPropertyTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//		dto.set<"propertyType"_>(transaction.propertyType);
//		dto.set<"modificationsCount"_>(transaction.modificationsCount);
//
//		std::vector<TransactionPropertyModificationDto> dtos;
//		for (auto &accountTransactionProperty : transaction.modifications) {
//			dtos.push_back(toDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(
//					accountTransactionProperty));
//		}
//		dto.set<"modifications"_>(std::move(dtos));
//		return dto;
//	}
//
//	template<>
//	AccountMosaicPropertyTransactionDto toDto<AccountMosaicPropertyTransaction, AccountMosaicPropertyTransactionDto>(
//			const AccountMosaicPropertyTransaction &transaction) {
//		AccountMosaicPropertyTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//
//		dto.set<"propertyType"_>(transaction.propertyType);
//		dto.set<"modificationsCount"_>(transaction.modificationsCount);
//
//		std::vector<MosaicPropertyModificationDto> dtos;
//		for (auto &accountTransactionProperty : transaction.modifications) {
//			dtos.push_back(toDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(
//					accountTransactionProperty));
//		}
//		dto.set<"modifications"_>(std::move(dtos));
//		return dto;
//	}
//
//	template<>
//	AccountAddressPropertyTransactionDto toDto<AccountAddressPropertyTransaction, AccountAddressPropertyTransactionDto>(
//			const AccountAddressPropertyTransaction &transaction) {
//		AccountAddressPropertyTransactionDto dto;
//
//		COMPOSE_TRANSACTION(transaction, dto)
//
//
//		dto.set<"propertyType"_>(transaction.propertyType);
//		dto.set<"modificationsCount"_>(transaction.modificationsCount);
//
//		std::vector<AddressPropertyModificationDto> dtos;
//		for (auto &accountTransactionProperty : transaction.modifications) {
//			dtos.push_back(toDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(
//					accountTransactionProperty));
//		}
//		dto.set<"modifications"_>(std::move(dtos));
//		return dto;
//	}
//}
///////Embedded transactions
////
////template<>
////EmbeddedAccountLinkTransactionDto toDto<EmbeddedAccountLinkTransaction, EmbeddedAccountLinkTransactionDto >(const EmbeddedAccountLinkTransaction & transaction) {
////	EmbeddedAccountLinkTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.linkAction = dto.set<"linkAction"_>();
////	transaction.remoteAccountKey = dto.set<"remoteAccountKey"_>();
////	return dto;
////}
////
////
////template<>
////EmbeddedLockFundsTransactionDto toDto<EmbeddedLockFundsTransaction, EmbeddedLockFundsTransactionDto >(const EmbeddedLockFundsTransaction & transaction) {
////	EmbeddedLockFundsTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.lockedMosaic = toDto<Mosaic, MosaicDto>(dto.set<"lockedMosaic"_>());
////	transaction.lockDuration = dto.set<"lockDuration"_>();
////	transaction.lockHash = dto.set<"lockHash"_>();
////	return dto;
////}
////
////
////template<>
////EmbeddedModifyMultisigAccountTransactionDto toDto<EmbeddedModifyMultisigAccountTransaction, EmbeddedModifyMultisigAccountTransactionDto >(const EmbeddedModifyMultisigAccountTransaction & transaction) {
////	EmbeddedModifyMultisigAccountTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.minRemovalDelta = dto.set<"minRemovalDelta"_>();
////	transaction.minApprovalDelta = dto.set<"minApprovalDelta"_>();
////	transaction.modificationsCount = dto.set<"modificationsCount"_>();
////	for(auto& cosignatoryModificationDto : dto.set<"modifications"_>()) {
////		transaction.modifications.push_back(toDto<CosignatoryModification, CosignatoryModificationDto>(cosignatoryModificationDto));
////	}
////
////	return dto;
////}
////
////
////template<>
////EmbeddedMosaicDefinitionTransactionDto toDto<EmbeddedMosaicDefinitionTransaction, EmbeddedMosaicDefinitionTransactionDto >(const EmbeddedMosaicDefinitionTransaction & transaction) {
////	EmbeddedMosaicDefinitionTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////
////	transaction.nonce = dto.set<"nonce"_>();
////	transaction.mosaicId = dto.set<"mosaicId"_>();
////	transaction.optionalPropertiesCount = dto.set<"optionalPropertiesCount"_>();
////	transaction.flags = dto.set<"flags"_>();
////	transaction.divisibility = dto.set<"divisibility"_>();
////	for(auto & mosaicProperty : dto.set<"optionalProperties"_>()) {
////		transaction.optionalProperties.push_back(toDto<MosaicProperty, MosaicPropertyDto>(mosaicProperty)) ;
////	}
////	return dto;
////}
////
////
////template<>
////EmbeddedMosaicSupplyChangeTransactionDto toDto<EmbeddedMosaicSupplyChangeTransaction, EmbeddedMosaicSupplyChangeTransactionDto >(const EmbeddedMosaicSupplyChangeTransaction & transaction) {
////	EmbeddedMosaicSupplyChangeTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.mosaicId = dto.set<"mosaicId"_>();
////	transaction.direction = dto.set<"direction"_>();
////	transaction.delta = dto.set<"delta"_>();
////	return dto;
////}
////
////
////template<>
////EmbeddedRegisterNamespaceTransactionDto toDto<EmbeddedRegisterNamespaceTransaction, EmbeddedRegisterNamespaceTransactionDto >(const EmbeddedRegisterNamespaceTransaction & transaction) {
////	EmbeddedRegisterNamespaceTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.namespaceType = dto.set<"namespaceType"_>();
////	transaction.durationOrParentId = dto.set<"durationOrParentId"_>();
////	transaction.namespaceId = dto.set<"namespaceId"_>();
////	transaction.namespaceNameSize = dto.set<"namespaceNameSize"_>();
////	transaction.namespaceName = dto.set<"namespaceName"_>();
////
////	return dto;
////}
////
////template<>
////EmbeddedSecretLockTransactionDto toDto<EmbeddedSecretLockTransaction, EmbeddedSecretLockTransactionDto >(const EmbeddedSecretLockTransaction & transaction) {
////	EmbeddedSecretLockTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////
////	transaction.mosaicId = dto.set<"mosaicId"_>();
////	transaction.amount = dto.set<"amount"_>();
////	transaction.duration = dto.set<"duration"_>();
////	transaction.hashAlgorithm = dto.set<"hashAlgorithm"_>();
////	transaction.secret = dto.set<"secret"_>();
////	transaction.recipient = dto.set<"recipient"_>();
////
////
////	return dto;
////}
////
////
////template<>
////EmbeddedSecretProofTransactionDto toDto<EmbeddedSecretProofTransaction, EmbeddedSecretProofTransactionDto >(const EmbeddedSecretProofTransaction & transaction) {
////	EmbeddedSecretProofTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.hashAlgorithm = dto.set<"hashAlgorithm"_>();
////	transaction.secret = dto.set<"secret"_>();
////	transaction.proofSize = dto.set<"proofSize"_>();
////	transaction.proof = dto.set<"proof"_>();
////
////
////	return dto;
////}
////
////
////template<>
////EmbeddedTransferTransactionDto toDto<EmbeddedTransferTransaction, EmbeddedTransferTransactionDto >(const EmbeddedTransferTransaction & transaction) {
////	EmbeddedTransferTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////
////	transaction.recipient = dto.set<"recipient"_>();
////	transaction.messageSize = dto.set<"messageSize"_>();
////	transaction.mosaicsCount = dto.set<"mosaicsCount"_>();
////	transaction.message = dto.set<"message"_>();
////
////	for(auto& mosaicDto : dto.set<"mosaics"_>()) {
////		transaction.mosaics.push_back(toDto<Mosaic, MosaicDto>(mosaicDto));
////	}
////
////	return dto;
////}
////
////
////template<>
////EmbeddedAliasTransactionBaseDto toDto<EmbeddedAliasTransactionBase, EmbeddedAliasTransactionBaseDto >(const EmbeddedAliasTransactionBase & transaction) {
////	EmbeddedAliasTransactionBaseDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////
////	transaction.aliasAction = dto.set<"aliasAction"_>();
////	transaction.namespaceId = dto.set<"namespaceId"_>();
////
////	return dto;
////}
////
////template<>
////EmbeddedAddressAliasTransactionDto toDto<EmbeddedAddressAliasTransaction, EmbeddedAddressAliasTransactionDto >(const EmbeddedAddressAliasTransaction & transaction) {
////	EmbeddedAddressAliasTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.address = dto.set<"address"_>();
////	return dto;
////}
////
////template<>
////EmbeddedMosaicAliasTransactionDto toDto<EmbeddedMosaicAliasTransaction, EmbeddedMosaicAliasTransactionDto >(const EmbeddedMosaicAliasTransaction & transaction) {
////	EmbeddedMosaicAliasTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.mosaicId = dto.set<"mosaicId"_>();
////	return dto;
////}
////
/////// Account Property Transactions
////template<>
////EmbeddedAccountTransactionPropertyTransactionDto toDto<EmbeddedAccountTransactionPropertyTransaction, EmbeddedAccountTransactionPropertyTransactionDto >(const EmbeddedAccountTransactionPropertyTransaction & transaction) {
////	EmbeddedAccountTransactionPropertyTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.propertyType = dto.set<"propertyType"_>();
////	transaction.modificationsCount = dto.set<"modificationsCount"_>();
////	for(auto& accountTransactionPropertyDto : dto.set<"modifications"_>()){
////		transaction.modifications.push_back(toDto<AccountTransactionPropertyModification, TransactionPropertyModificationDto>(accountTransactionPropertyDto));
////	}
////
////	return dto;
////}
////
////template<>
////EmbeddedAccountMosaicPropertyTransactionDto toDto<EmbeddedAccountMosaicPropertyTransaction, EmbeddedAccountMosaicPropertyTransactionDto >(const EmbeddedAccountMosaicPropertyTransaction & transaction) {
////	EmbeddedAccountMosaicPropertyTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////
////	transaction.propertyType = dto.set<"propertyType"_>();
////	transaction.modificationsCount = dto.set<"modificationsCount"_>();
////	for(auto& accountTransactionPropertyDto : dto.set<"modifications"_>()){
////		transaction.modifications.push_back(toDto<AccountMosaicPropertyModification, MosaicPropertyModificationDto>(accountTransactionPropertyDto));
////	}
////
////	return dto;
////}
////
////template<>
////EmbeddedAccountAddressPropertyTransactionDto toDto<EmbeddedAccountAddressPropertyTransaction, EmbeddedAccountAddressPropertyTransactionDto >(const EmbeddedAccountAddressPropertyTransaction & transaction) {
////	EmbeddedAccountAddressPropertyTransactionDto dto;
////
////	COMPOSE_EMBEDDED_TRANSACTION(transaction, dto)
////
////	transaction.propertyType = dto.set<"propertyType"_>();
////	transaction.modificationsCount = dto.set<"modificationsCount"_>();
////	for(auto& accountTransactionPropertyDto : dto.set<"modifications"_>()){
////		transaction.modifications.push_back(toDto<AccountAddressPropertyModification, AddressPropertyModificationDto>(accountTransactionPropertyDto));
////	}
////
////	return dto;
////}