#include <vector>
#include <nemcpp/model/transaction_simple/transaction.h>


namespace xpx_sdk {
	class TransactionContainer {
	public:
		union AbstractTransaction {
			AccountLinkTransaction  accountLinkTransaction;
			EmbeddedAccountLinkTransaction  embeddedAccountLinkTransaction>;

			LockFundsTransaction  lockFundsTransaction;
			EmbeddedLockFundsTransaction  embeddedLockFundsTransaction;

			ModifyMultisigAccountTransaction  modifyMultisigAccountTransaction;
			EmbeddedModifyMultisigAccountTransaction  embeddedModifyMultisigAccountTransaction;

			MosaicDefinitionTransaction mosaicDefinitionTransaction;
			EmbeddedMosaicDefinitionTransaction  embeddedMosaicDefinitionTransaction;

			MosaicSupplyChangeTransaction  mosaicSupplyChangeTransaction;
			EmbeddedMosaicSupplyChangeTransaction embeddedMosaicSupplyChangeTransaction;

			RegisterNamespaceTransaction  registerNamespaceTransaction;
			EmbeddedRegisterNamespaceTransaction  embeddedRegisterNamespaceTransaction;

			SecretLockTransaction  secretLockTransaction;
			EmbeddedSecretLockTransaction  embeddedSecretLockTransaction;

			SecretProofTransaction  secretProofTransaction;
			EmbeddedSecretProofTransaction embeddedSecretProofTransaction;

			TransferTransaction  transferTransaction;
			EmbeddedTransferTransaction  embeddedTransferTransaction;

			AddressAliasTransaction  addressAliasTransaction;
			EmbeddedAddressAliasTransaction embeddedAddressAliasTransaction;

			MosaicAliasTransaction mosaicAliasTransaction;
			EmbeddedMosaicAliasTransaction embeddedMosaicAliasTransaction;

			AccountAddressPropertyTransaction accountAddressPropertyTransaction;
			EmbeddedAccountAddressPropertyTransaction  embeddedAccountAddressPropertyTransaction;

			AccountMosaicPropertyTransaction  accountMosaicPropertyTransaction;
			EmbeddedAccountMosaicPropertyTransaction embeddedAccountMosaicPropertyTransaction;

			AccountTransactionPropertyTransaction  accountTransactionPropertyTransaction;
			EmbeddedAccountTransactionPropertyTransaction  embeddedAccountTransactionPropertyTransaction;

			TransactionType type;

			AbstractTransaction(const AccountLinkTransaction& transaction): accountLinkTransaction(transaction){}
			AbstractTransaction(const EmbeddedAccountLinkTransaction& transaction): embeddedAccountLinkTransaction(transaction){}

			AbstractTransaction(const LockFundsTransaction& transaction): lockFundsTransaction(transaction){}
			AbstractTransaction(const EmbeddedLockFundsTransaction& transaction): embeddedLockFundsTransaction(transaction){}

			AbstractTransaction(const ModifyMultisigAccountTransaction& transaction): modifyMultisigAccountTransaction(transaction){}
			AbstractTransaction(const EmbeddedModifyMultisigAccountTransaction& transaction): embeddedModifyMultisigAccountTransaction(transaction){}

			AbstractTransaction(const MosaicDefinitionTransaction& transaction): mosaicDefinitionTransaction(transaction){}
			AbstractTransaction(const EmbeddedMosaicDefinitionTransaction& transaction): embeddedMosaicDefinitionTransaction(transaction){}

			AbstractTransaction(const MosaicSupplyChangeTransaction& transaction): mosaicSupplyChangeTransaction(transaction){}
			AbstractTransaction(const EmbeddedMosaicSupplyChangeTransaction& transaction): embeddedMosaicSupplyChangeTransaction(transaction){}

			AbstractTransaction(const RegisterNamespaceTransaction& transaction): registerNamespaceTransaction(transaction){}
			AbstractTransaction(const EmbeddedRegisterNamespaceTransaction& transaction): embeddedRegisterNamespaceTransaction(transaction){}

			AbstractTransaction(const SecretLockTransaction& transaction): secretLockTransaction(transaction){}
			AbstractTransaction(const EmbeddedSecretLockTransaction& transaction): embeddedSecretLockTransaction(transaction){}

			AbstractTransaction(const SecretProofTransaction& transaction): secretProofTransaction(transaction){}
			AbstractTransaction(const EmbeddedSecretProofTransaction& transaction): embeddedSecretProofTransaction(transaction){}

			AbstractTransaction(const TransferTransaction& transaction): transferTransaction(transaction){}
			AbstractTransaction(const EmbeddedTransferTransaction& transaction): embeddedTransferTransaction(transaction){}

			AbstractTransaction(const AddressAliasTransaction& transaction): addressAliasTransaction(transaction){}
			AbstractTransaction(const EmbeddedAddressAliasTransaction& transaction): embeddedAddressAliasTransaction(transaction){}

			AbstractTransaction(const MosaicAliasTransaction& transaction): mosaicAliasTransaction(transaction){}
			AbstractTransaction(const EmbeddedMosaicAliasTransaction& transaction): embeddedMosaicAliasTransaction(transaction){}

			AbstractTransaction(const AccountAddressPropertyTransaction& transaction): accountAddressPropertyTransaction(transaction){}
			AbstractTransaction(const EmbeddedAccountAddressPropertyTransaction& transaction): embeddedAccountAddressPropertyTransaction(transaction){}

			AbstractTransaction(const AccountMosaicPropertyTransaction& transaction): accountMosaicPropertyTransaction(transaction){}
			AbstractTransaction(const EmbeddedAccountMosaicPropertyTransaction& transaction): embeddedAccountMosaicPropertyTransaction(transaction){}

			AbstractTransaction(const AccountTransactionPropertyTransaction& transaction): accountTransactionPropertyTransaction(transaction){}
			AbstractTransaction(const EmbeddedAccountTransactionPropertyTransaction& transaction): embeddedAccountTransactionPropertyTransaction(transaction){}
		};

		std::vector<AbstractTransaction> transactions;

	};
}