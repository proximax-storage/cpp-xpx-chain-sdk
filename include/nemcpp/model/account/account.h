
#pragma once

#include <nemcpp/types.h>
#include <nemcpp/config.h>
#include <nemcpp/crypto/private_key.h>
#include <nemcpp/crypto/key_pair.h>
#include <nemcpp/model/account/address.h>
#include <nemcpp/model/account/public_account.h>
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/model/transaction/aggregate_transaction.h>

#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

using xpx_sdk::Transaction;
//using xpx_sdk::AggregateTransaction;
namespace xpx_sdk {
	
	/// Private key supplier call reason.
	enum class PrivateKeySupplierReason: uint8_t {
		Public_Account_Init,
		Private_Key_Requested,
		Transaction_Signing,
		Aggregate_Transaction_Cosigning
	};
	
	/// Variant private key supplier param.
	using PrivateKeySupplierParam = std::variant<const Transaction*>;
	
	/// Private key supplier.
	/// \note Second parameter in supplier call depends on private key request reason and can be one of the following:
	///       1. empty - for \c Public_Account_Init and \c Private_Key_Requested
	///       2. \c Transaction*  - for Transaction_Signing and Aggregate_Transaction_Cosigning
	/// \note Supplier can never be called with \c Public_Account_Init reason because public account information
	///       is initialized whenever private key is available. After public account data is initialized, supplier
	///       is never called again with this reason.
	using PrivateKeySupplier = std::function<std::optional<PrivateKey> (PrivateKeySupplierReason, PrivateKeySupplierParam)>;
	
	/// Blockchain account.
	/// \note This class does not store private key for security reasons. Instead of this it calls private
	///       key supplier function each time private key is required.
	/// \note If private key supplier fails to obtain private key, \c Account methods requiring it throw
	///       \c account_error exception.
	class Account: private NonCopyable {
	public:
		/// Creates account for \a networtId.
		Account(PrivateKeySupplier privateKeySupplier, NetworkIdentifier networkId = GetConfig().NetworkId);
		
		/// Returns network id for which account was created.
		NetworkIdentifier networkId() const;
		
		/// Returns account private key.
		PrivateKey privateKey() const;
		
		/// Returns account key pair.
		KeyPair keyPair() const;
		
		/// Returns account public key.
		const Key& publicKey() const;
		
		/// Returns account address.
		const Address& address() const;
		
		/// Returns public account.
		const PublicAccount& publicAccount() const;
		
		/// Signs transactions.
		/// \note Throws \c account_error if account and transaction network ids do not match.
		void signTransaction(Transaction* transaction) const;
		
		/// Calculates cosignature for aggregate transaction.
//		Signature cosignAggregateTransaction(const AggregateTransaction* transaction) const;
		
		
	private:
		PrivateKey requestPrivateKey(PrivateKeySupplierReason reason, PrivateKeySupplierParam param = {}) const;
		
	private:
		PrivateKeySupplier privateKeySupplier_;
		NetworkIdentifier networkId_;
		mutable std::optional<PublicAccount> publicAccount_;
	};
}
