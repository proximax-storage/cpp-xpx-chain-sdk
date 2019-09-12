
#pragma once

#include <xpxchaincpp/model/account/account_property.h>
#include <xpxchaincpp/model/transaction/account_property_transaction_types.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>


namespace xpx_chain_sdk {
	
	/// Account property transaction template base class.
	template<typename TBase, typename TProperty>
	class TAccountPropertyTransaction: public TBase {
	public:
		/// Property type.
		using PropertyType = TProperty;

		/// Property modifications type.
		using PropertyModifications = AccountPropertyModifications<typename PropertyType::ValueType>;

	public:
		/// Creates account property transaction.
		/// \note Throws \c transaction_error if \a propertyModifications size is invalid.
		template<typename... TArgs>
		explicit TAccountPropertyTransaction(AccountPropertyRule propertyRule,
		                                     PropertyModifications propertyModifications,
		                                     TArgs&&... args):
			TBase(getTransactionType(), std::forward<TArgs>(args)...),
			propertyRule_(propertyRule),
			propertyModifications_(std::move(propertyModifications))
		{ }

		/// Returns account property rule.
		AccountPropertyRule propertyRule() const;

		/// Returns account property modifications.
		const PropertyModifications& propertyModifications() const;

	private:
		static TransactionType getTransactionType();

	private:
		AccountPropertyRule propertyRule_;
		PropertyModifications propertyModifications_;
	};

	extern template class TAccountPropertyTransaction<Transaction, AccountAddressProperty>;
	extern template class TAccountPropertyTransaction<EmbeddedTransaction, AccountAddressProperty>;
	extern template class TAccountPropertyTransaction<Transaction, AccountMosaicProperty>;
	extern template class TAccountPropertyTransaction<EmbeddedTransaction, AccountMosaicProperty>;
	extern template class TAccountPropertyTransaction<Transaction, AccountTransactionProperty>;
	extern template class TAccountPropertyTransaction<EmbeddedTransaction, AccountTransactionProperty>;

	using AccountAddressPropertyTransaction = TAccountPropertyTransaction<Transaction, AccountAddressProperty>;
	using EmbeddedAccountAddressPropertyTransaction = TAccountPropertyTransaction<EmbeddedTransaction, AccountAddressProperty>;
	using AccountMosaicPropertyTransaction = TAccountPropertyTransaction<Transaction, AccountMosaicProperty>;
	using EmbeddedAccountMosaicPropertyTransaction = TAccountPropertyTransaction<EmbeddedTransaction, AccountMosaicProperty>;
	using AccountTransactionPropertyTransaction = TAccountPropertyTransaction<Transaction, AccountTransactionProperty>;
	using EmbeddedAccountTransactionPropertyTransaction = TAccountPropertyTransaction<EmbeddedTransaction, AccountTransactionProperty>;

	/// Creates account address property transaction.
	/// \note Throws \c transaction_error if \a propertyModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AccountAddressPropertyTransaction>
	CreateAccountAddressPropertyTransaction(AccountPropertyRule propertyRule,
	                                        AccountAddressPropertyModifications propertyModifications,
	                                        std::optional<Amount> maxFee = std::nullopt,
	                                        std::optional<NetworkDuration> deadline = std::nullopt,
	                                        std::optional<NetworkIdentifier> networkId = std::nullopt);

	/// Creates embedded account address property transaction.
	/// \note Throws \c transaction_error if \a propertyModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedAccountAddressPropertyTransaction>
	CreateEmbeddedAccountAddressPropertyTransaction(AccountPropertyRule propertyRule,
	                                                AccountAddressPropertyModifications propertyModifications,
	                                                const Key& signer,
	                                                std::optional<NetworkIdentifier> networkId = std::nullopt);

	/// Creates account mosaic property transaction.
	/// \note Throws \c transaction_error if \a propertyModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AccountMosaicPropertyTransaction>
	CreateAccountMosaicPropertyTransaction(AccountPropertyRule propertyRule,
	                                       AccountMosaicPropertyModifications propertyModifications,
	                                       std::optional<Amount> maxFee = std::nullopt,
	                                       std::optional<NetworkDuration> deadline = std::nullopt,
	                                       std::optional<NetworkIdentifier> networkId = std::nullopt);

	/// Creates embedded account mosaic property transaction.
	/// \note Throws \c transaction_error if \a propertyModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedAccountMosaicPropertyTransaction>
	CreateEmbeddedAccountMosaicPropertyTransaction(AccountPropertyRule propertyRule,
	                                               AccountMosaicPropertyModifications propertyModifications,
	                                               const Key& signer,
	                                               std::optional<NetworkIdentifier> networkId = std::nullopt);

	/// Creates account transaction property transaction.
	/// \note Throws \c transaction_error if \a propertyModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AccountTransactionPropertyTransaction>
	CreateAccountTransactionPropertyTransaction(AccountPropertyRule propertyRule,
	                                            AccountTransactionPropertyModifications propertyModifications,
	                                            std::optional<Amount> maxFee = std::nullopt,
	                                            std::optional<NetworkDuration> deadline = std::nullopt,
	                                            std::optional<NetworkIdentifier> networkId = std::nullopt);

	/// Creates embedded account transaction property transaction.
	/// \note Throws \c transaction_error if \a propertyModifications size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedAccountTransactionPropertyTransaction>
	CreateEmbeddedAccountTransactionPropertyTransaction(AccountPropertyRule propertyRule,
	                                                    AccountTransactionPropertyModifications propertyModifications,
	                                                    const Key& signer,
	                                                    std::optional<NetworkIdentifier> networkId = std::nullopt);
}
